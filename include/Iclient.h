#ifndef ICLIENT_H_
#define ICLIENT_H_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "InfoStudent.h"
#include "xrcedds_demo.hpp"

typedef struct student{
    std::string name;
    long number;
    long grade;
} student;


bool comp(const struct student &s1, const struct student &s2);

void serialStudent(const struct student &s, char* buffer);

void unserialStudent(struct student &s, char* const buffer);

void InfoStu2stu(struct student &s, const InfoStudent &infoS);

void stu2InfoStu(const struct student &s, InfoStudent &infoS);

int run(int args, char** argv);

int runServer();

#endif