#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
 */
class Process {
public:
    Process(int pid):pid_( pid )
    {

        cpu_proc_utiliz_ = Process::CpuUtilization();
        cpu_ram_utiliz_ = LinuxParser::Ram(pid_);
    }
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const& a) const;


private:
    int pid_;
    float cpu_proc_utiliz_;
    long cpu_ram_utiliz_;
};

#endif
