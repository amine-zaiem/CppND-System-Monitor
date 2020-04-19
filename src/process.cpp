#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>

#include "process.h"

using namespace std;


/*
 *  Helper to return this process's ID
 */
int Process::Pid()
{
    return pid_;
}

/*
 *  Helper to return this process's CPU utilization
 */
float Process::CpuUtilization()
{
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);;
    long upTime = LinuxParser::UpTime(pid_);
    return (float)activeJiffies/upTime;
//    return LinuxParser::CpuUtilization(pid_)/100
}

/*
 *  Helper to return the command that generated this process
 */
string Process::Command()
{
    string cmdline = LinuxParser::Command(pid_);
    string outputCmd;

    // testing if command line contains more than 40 characters
    if ( cmdline.size() < 40)
    {
       outputCmd = cmdline;
    }
    else
    {
       outputCmd = cmdline.substr(0, 40)+"...";

    }
    return outputCmd;
}

/*
 *  Helper to return this process's memory utilization
 */
string Process::Ram()
{
    return std::to_string(LinuxParser::Ram(pid_));
}

/*
 * Helper to return the user (name) that generated this process
 */
string Process::User()
{
    return LinuxParser::User(pid_);
}

/*
 * Helper to return the age of this process (in seconds)
 */
long int Process::UpTime()
{
    return LinuxParser::UpTime(pid_);
}

/*
 * Helper to Overload the "less than" comparison operator for Process objects
 */
bool Process::operator<(Process const& a) const
{

    //float currentCpuUtil = this->CpuUtilization();
    //float nextProcCpuUtil = a.CpuUtilization();
    // why the previous lines are giving an error
    bool cpuRes = false;
    if ( a.cpu_proc_utiliz_ < cpu_proc_utiliz_ )
    {
        cpuRes = true;
    }
    else if ( a.cpu_proc_utiliz_ > cpu_proc_utiliz_ )
    {
        cpuRes = false;
    }
    else
    {
        cpuRes = a.cpu_ram_utiliz_ < cpu_ram_utiliz_;
    }

    return cpuRes;
}
