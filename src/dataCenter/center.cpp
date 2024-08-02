// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <uxr/agent/AgentInstance.hpp>
#include "Iclient.h"
#include <queue>

int XRCEddsAgent(int argc, char** argv){
    eprosima::uxr::AgentInstance& agent_instance = agent_instance.getInstance();

    if (!agent_instance.create(argc, argv))
    {
        return 1;
    }
    agent_instance.run();
}

int main(int argc, char** argv)
{
    std::thread ag(XRCEddsAgent, argc, argv);
    std::thread ag1(run, argc, argv);
    std::thread ag2(runServer);
    //Agent2CentorQueue& a2c = Agent2CentorQueue::instance();
    std::priority_queue<struct student, std::vector<struct student>, bool(*)(const struct student&, const struct student&)> stuPq(comp);
    size_t maxSize = 100;

    Agent2CentorQueue& a2c = Agent2CentorQueue::instance();
    
    while(true){
        if(!a2c.center_read_queue.IsEmpty()){
            //printf("queue is not empty\n");
            stuPq.push(*a2c.center_read_queue.Pop());
            if(stuPq.size() >= maxSize){
                time_t timep;
                time(&timep);
                struct tm *p = localtime(&timep);

                char filename[256];
                snprintf(filename, 256, "../output/%d%02d%02d%02d%02d%02d.txt", 1900+p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
                printf("Ready to save file\n");
                std::ofstream outFile(filename);
                if (!outFile.is_open()) {
                    std::cerr << "Error opening file for writing." << std::endl;
                    return 1;
                }
                int tmp = maxSize;
                while(--tmp != -1 && !stuPq.empty()){
                    struct student s = stuPq.top();
                    stuPq.pop();
                    outFile << "name: " << s.name << ", number: " << s.number << ", grade:" << s.grade << std::endl;
                }
                outFile.close();
                printf("Save data in file %s\n", filename);
            }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ag.join();
    ag1.join();
    ag2.join();
    a2c.center_read_queue.~XRCEQueue();
    return 0;
}
