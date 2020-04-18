#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
public:
    Processor (){
        allJiffies_ = LinuxParser::Jiffies();
        activeJiffies_= LinuxParser::ActiveJiffies();
    }
    float Utilization();

private:
    long allJiffies_ = 0;
    long activeJiffies_ =0;
    const uint16_t timeout_ = 10;

};

#endif
