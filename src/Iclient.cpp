#include "Iclient.h"

using std::string;


bool comp(const struct student &s1, const struct student &s2){
    
    if(s1.number == s2.number){
        return s1.grade > s2.grade;
    }
    else{
        return s1.number > s2.number;
    }
}

void serialStudent(const struct student &s, char* buffer){
    size_t offset = s.name.size();
    
    strcpy(buffer, s.name.c_str());
    buffer[offset++] = '\0';

    memcpy(buffer + offset, &s.number, sizeof(long));
    offset += sizeof(long);

    memcpy(buffer + offset, &s.grade, sizeof(long));
}

void unserialStudent(struct student &s, char* const buffer){
    size_t offset = strlen(buffer);
    s.name = string(buffer, offset++);

    memcpy(&s.number, buffer + offset, sizeof(long));
    offset += sizeof(long);

    memcpy(&s.grade, buffer + offset, sizeof(long));
}

void InfoStu2stu(struct student &s, const InfoStudent &infoS){
    s.name = infoS.name;
    s.grade = infoS.grade;
    s.number = infoS.number;
}

void stu2InfoStu(const struct student &s, InfoStudent &infoS){
    s.name.copy(infoS.name, sizeof(infoS.name)-1);
    infoS.grade = s.grade;
    infoS.number = s.number;
}

