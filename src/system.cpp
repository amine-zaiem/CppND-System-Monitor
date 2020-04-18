#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/*
 * A helper to return the system's CPU
 */
Processor& System::Cpu() { return cpu_; }

/*
 *  helper to return a container composed of the system's processes
 */
vector<Process>& System::Processes()
{
    // Ask mentor what is the difference between doing the following line here
    processes_ = {}; // instead of doing  it in the constructor of the process class
    // because it did not work for me.
    for ( auto pidItem : LinuxParser::Pids() )
    {
        Process sysProcess(pidItem);
        processes_.push_back( sysProcess );
    }
    std::sort(processes_.begin(), processes_.end());

    return processes_;
}

/*
 * A helper to return the system's kernel identifier (string)
 */
std::string System::Kernel()
{
    return LinuxParser::Kernel();
}

/*
 * A helper to return the system's memory utilization
 */
float System::MemoryUtilization()
{
    return LinuxParser::MemoryUtilization();
}

/*
 * A helper to return the operating system name
 */
std::string System::OperatingSystem()
{
    return LinuxParser::OperatingSystem();
}

/*
 * A helper to return the number of processes actively running on the system
 */
int System::RunningProcesses()
{
    return LinuxParser::RunningProcesses();
}

/*
 * A helper to return the total number of processes on the system
 */
// TODO: Return the total number of processes on the system
int System::TotalProcesses()
{
    return LinuxParser::TotalProcesses();
}

/*
 * A helper to return the number of seconds since the system started running
 */
long int System::UpTime()
{
    return LinuxParser::UpTime();
}
