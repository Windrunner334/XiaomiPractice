#include "Iclient.h"


int main() {
    // 创建 socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket." << std::endl;
        return 1;
    }

    // 连接到服务器
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        return 1;
    }

    // 发送结构体数据到服务器
    srand(3);
    const std::string myName = "lixiang";
    struct student sData;
    size_t numCnt = 0;
    while(true){
        //memcpy(&d, (char*)&sData, 1024);
        sData.name   = myName;
        sData.number = rand()%100000;
        sData.grade  = 3;

        char data[256];
        serialStudent(sData, data);

        //size_t len = buffer.length();
        printf("Send %ld message, name: %s, id: %ld, grade: %ld\n", ++numCnt, sData.name, sData.number, sData.grade);
        //send(clientSocket, &len, sizeof(int), 0);
        send(clientSocket, data, 256, 0);
        //printf(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    // 关闭连接
    close(clientSocket);

    return 0;
}
