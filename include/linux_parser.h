#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>


namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};


//extern std::string exception_text;

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

template <typename T>
T GetValue( std::string const &filePath, std::string const &token, T& value)
{
    std::string line, key;

    try
    {
        std::ifstream filestream(filePath);
        if (filestream.is_open())
        {
            while (std::getline(filestream, line))
            {
                std::istringstream linestream(line);
                while (linestream >> key >> value)
                {
                    if (key == token)
                    {
                        return value;
                    }
                }
            }
        }
    }
    catch (std::exception &e)
    {

        std::string exceptiontext = e.what();

    }
    return value;
};

// CPU
enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
long Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

};  // namespace LinuxParser

#endif
