#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <set>
#include <queue>
#include <algorithm>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Iclient.h"

std::mutex mtx;
std::condition_variable cv;
bool stopServer = false;
int numCnt = 1;
const int numMax = 100;
Agent2CentorQueue& a2c = Agent2CentorQueue::instance();

std::priority_queue<struct student, std::vector<struct student>, bool(*)(const struct student&, const struct student&)> stuPq(comp);


void handleClient(int clientId, int clientSocket){
    struct student stuData;
    char *data = new char[256];
    memset(data, 0, 256*sizeof(char));
    //std::set<struct student, bool(*)(const struct student&, const struct student&)> stuSet(comp);
    printf("Thread id: %ld ready to receive data\n", std::this_thread::get_id());

    while(recv(clientSocket, data, 256, 0)){
        // 对传输数据解串行
        unserialStudent(stuData, data);
        memset(data, 0, 256*sizeof(char));
        
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return !stopServer;});
        printf("Recevie %d message, name: %s, id: %ld, grade: %ld\n", numCnt++, stuData.name.c_str(), stuData.number, stuData.grade);
        a2c.center_read_queue.Push(stuData);
        lock.unlock();
        cv.notify_all();
        //stuPq.push(student{stuData.name, stuData.number, stuData.grade});
        /*
        stuPq.push(stuData);
        if(++numCnt >= numMax){
            cv.notify_all();
            // 接收结构体数据并保存到文件
            time_t timep;
            struct tm *p;

            time(&timep);
            p = localtime(&timep);
            
            char filename[256];
            snprintf(filename, 256, "%d%02d%02d%02d%02d%02d.txt", 1900+p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
            printf("Ready to save file\n");
            std::ofstream outFile(filename);
            if (!outFile.is_open()) {
                std::cerr << "Error opening file for writing." << std::endl;
                return;
            }
            int tmp = numMax;
            while(--tmp != -1&& !stuPq.empty()){
                struct student s = stuPq.top();
                stuPq.pop();
                outFile << "name: " << s.name << ", number: " << s.number << ", grade:" << s.grade << std::endl;
                a2c.center_read_queue.Push(s);
            }
            numCnt = 0;

            outFile.close();
            printf("Save date in file %s\n", filename);
        }
        */
    }
    delete[] data;
    printf("Thread id: %ld close\n", std::this_thread::get_id());
    close(clientSocket);
}


int runServer() {
    // 声明互联变量
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr;

    // 创建 socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket." << std::endl;
        return 1;
    }

    // 绑定 socket 到 IP 和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding server socket." << std::endl;
        return 1;
    }

    // 监听连接
    printf("Ready\n");
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on server socket." << std::endl;
        return 1;
    }

    // 为多client建立线程池
    int clientId = 1;
    std::vector<std::thread> clientThreads;
    while(!stopServer){
        clientSocket = accept(serverSocket, NULL, NULL);
        if(clientSocket == -1){
            std::cerr << "Error accepting client connection." << std::endl;
            return 1;
        }

        clientThreads.emplace_back(handleClient, clientId++, clientSocket);
    }

    // 进程结束
    std::for_each(clientThreads.begin(), clientThreads.end(),[](std::thread &t){t.join();});
    close(serverSocket);

    return 0;
}

