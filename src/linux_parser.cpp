#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <exception>
//#include <fstream>
//#include <filesystem>

#include "linux_parser.h"

//#define HZ sysconf(_SC_CLK_TCK)
using std::stof;
using std::string;
using std::to_string;
using std::vector;

/*
 *  Helper to read data from the filesystem
 */
string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    try
    {
        std::ifstream filestream(kOSPath);
        if (filestream.is_open()) {
            while (std::getline(filestream, line)) {
                std::replace(line.begin(), line.end(), ' ', '_');
                std::replace(line.begin(), line.end(), '=', ' ');
                std::replace(line.begin(), line.end(), '"', ' ');
                std::istringstream linestream(line);
                while (linestream >> key >> value) {
                    if (key == "PRETTY_NAME")
                    {
                      if ( value.length() > 0)
                      {
                         std::replace(value.begin(), value.end(), '_', ' ');
                      }
                      else
                      {
                         value = default_os;
                      }
                      return value;
                    }
                }
            }
        }
        filestream.close();
    }
    catch (std::exception &e)
    {

        std::cout<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
    string os, kernel, version;
    string line;
    try
    {
        std::ifstream stream(kProcDirectory + kVersionFilename);
        if (stream.is_open())
        {
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> os >> version >> kernel;
        }
        stream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr)
    {
        // Is this a directory?
        if (file->d_type == DT_DIR)
        {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit))
            {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

/*
 * Helper to read and return the system memory utilization
 */
float LinuxParser::MemoryUtilization()
{
    string line;
    string key;
    uint64_t value;
    uint64_t memTotal, memFree, buffer, cachedMem, usedMem = 0;
    float memUtiliz = 0;
    try
    {
        std::ifstream filestream(kProcDirectory + kMeminfoFilename);
        if (filestream.is_open()) {
            while (std::getline(filestream, line))
            {
                std::replace(line.begin(), line.end(), ':', ' ');
                std::istringstream linestream(line);
                while (linestream >> key >> value)
                {
                    if (key == "MemTotal")
                    {
                        memTotal = value;
                    }
                    if (key == "MemFree")
                    {
                        memFree = value;
                    }
                    if (key == "Buffers")
                    {
                        buffer = value;
                    }
                    if (key == "Cached")
                    {
                        cachedMem = value;
                    }
                }
            }
        }

        usedMem = memTotal - memFree - buffer - cachedMem;
        memUtiliz = (float)usedMem / float(memTotal);
        filestream.close();
    }
    catch (std::exception &e)
    {

       std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return memUtiliz;
}

/*
 * helper to read and return the system uptime
*/
long LinuxParser::UpTime()
{
    long upTime = 0;
    string line;
    try
    {
        std::ifstream stream(kProcDirectory + kUptimeFilename);
        if (stream.is_open())
        {
            std::getline(stream, line);
            std::istringstream linestream(line);
            linestream >> upTime;
        }
        stream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return upTime;

}

/*
 * Helper to read and return the number of jiffies for the system
 * https://supportcenter.checkpoint.com/supportcenter/portal?eventSubmit_doGoviewsolutiondetails=&solutionid=sk65143
 * https://stackoverflow.com/questions/3017162/how-to-get-total-cpu-usage-in-linux-using-c
*/
long LinuxParser::Jiffies()
{
    long allJiffies = 0;
    std::vector<std::string> jiffiesList = CpuUtilization();


    std::ifstream filestream(kProcDirectory + kStatFilename);
    try
    {
       for ( auto item : jiffiesList )
       {
          allJiffies = allJiffies + std::stol(item);
       }

    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return allJiffies;
}

/*
 * Helper to read and return the number of active jiffies for a PID
 * https://supportcenter.checkpoint.com/supportcenter/portal?eventSubmit_doGoviewsolutiondetails=&solutionid=sk65143
 * https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
*/
long LinuxParser::ActiveJiffies(int pid)
{

    long activeJiffies = 0;
    //vector<long> statsList; --> To ask the mentor why it doesn't work this way
    vector<string> statsList;
    string line;
    string item;
    const long HZ = sysconf(_SC_CLK_TCK);
    //long /*item=0,*/;

    try
    {
        std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
        if (filestream.is_open())
        {
            std::getline(filestream, line);
            std::istringstream linestream(line);
            while (linestream >> item)
            {
                //            statsList.push_back(value);
                statsList.push_back(item);
            }
        }
        activeJiffies = ( std::stol(statsList[13]) + std::stol(statsList[14]) +
                std::stol(statsList[15]) + std::stol(statsList[16]) )/HZ;
        filestream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return activeJiffies;

}

/*
* Helper to read and return the number of active jiffies for the system
* https://supportcenter.checkpoint.com/supportcenter/portal?eventSubmit_doGoviewsolutiondetails=&solutionid=sk65143
*/
long LinuxParser::ActiveJiffies()
{
    string line;
    string key;
    long activeJiffies = 0;
    long allJiffies = Jiffies();
    long idleJiffies = IdleJiffies();

    activeJiffies = allJiffies - idleJiffies;
    return activeJiffies;
}

/*
 * Helper to read and return the number of idle jiffies for the system
*/
long LinuxParser::IdleJiffies()
{
    long idleJiffies = 0;

    std::vector<std::string> listOfJiffies = CpuUtilization();
    try
    {
       idleJiffies = std::stol(listOfJiffies[CPUStates::kIdle_])
                               + std::stol(listOfJiffies[CPUStates::kIOwait_]);
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return idleJiffies;
}

/*
 * Helper to read and return CPU utilization
*/
std::vector<std::string> LinuxParser::CpuUtilization()
{
    string value;
    string line;
    string key;
    vector<string> listOfJiffies;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (linestream >> key)
        {
            if (key != "cpu")
            {
                listOfJiffies.push_back(key);
            }
        }
    }
    filestream.close();
    return listOfJiffies;
}


/*
 * Helper to read and return the total number of processes
*/
int LinuxParser::TotalProcesses()
{
    int value = 0, processTotal = 0;
    processTotal =
            GetValue<int>(kProcDirectory + kStatFilename, "processes", value);

    return processTotal;
}

/*
 * Helper to read and return the number of running processes
*/
int LinuxParser::RunningProcesses()
{
    int value = 0, runningProcess = 0;
    runningProcess =
            GetValue<int>(kProcDirectory + kStatFilename, "procs_running", value);

    return runningProcess;
}

/*
 * Helper to read and return the command associated with a process
*/
string LinuxParser::Command(int pid)
{
    string line;
    string key;
    string cmdLine;

    try
    {
        std::ifstream filestream(kProcDirectory + to_string(pid).c_str() + kCmdlineFilename);
        if (filestream.is_open())
        {
            while (std::getline(filestream, line))
            {
                cmdLine = line;
            }
        }
        filestream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }


    return cmdLine;
}

/*
 * Helper to read and return the memory used by a process
*/
long LinuxParser::Ram(int pid)
{

    long value;
    int memutilization = 0;

    // i have used VmData insted of VmSize
    string filePath = kProcDirectory + to_string(pid).c_str() + kStatusFilename;
    memutilization =
            GetValue<long>( filePath, "VmData:", value)/1024;
    return memutilization;
}

/*
 * Helper to read and return the user ID associated with a process
*/
string LinuxParser::Uid(int pid)
{
    string value;
    string uid;

    string filePath = kProcDirectory + to_string(pid).c_str() + kStatusFilename;
    uid = GetValue<string>( filePath, "Uid:", value);
    return uid;
}

/*
 * Helper to read and return the user associated with a process
*/
//   https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/
string LinuxParser::User(int pid)
{
    string uid = LinuxParser::Uid(pid);
    string user, x, userUid;
    string line;

    try
    {
        std::ifstream filestream( kPasswordPath );
        if (filestream.is_open())
        {
            while (std::getline(filestream, line))
            {
                std::replace(line.begin(), line.end(), ':', ' ');
                std::istringstream linestream(line);
                while (linestream >> user >> x >> userUid)
                {
                    if ( userUid == uid )
                    {
                        return user;
                    }
                }
            }
        }
        filestream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return string();
}

/*
 * Helper to read and return the uptime of a process
*/
// https://stackoverflow.com/questions/6514378/how-do-you-get-how-long-a-process-has-been-running
long LinuxParser::UpTime(int pid )
{

    //vector<long> statsList; --> To ask the mentor why it doesn't work this way
    vector<string> statsList;
    string line;
    string item;
    const long HZ = sysconf(_SC_CLK_TCK);
    long /*item=0,*/ procStartTime = 0, processTime = 0;

    try
    {
        std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
        if (filestream.is_open())
        {
            std::getline(filestream, line);
            std::istringstream linestream(line);
            //        while (linestream >> item)
            while (linestream >> item)
            {
                //            statsList.push_back(value);
                statsList.push_back(item);
            }
        }
        //    procStartTime = statsList[21]/HZ;
        procStartTime = std::stol(statsList[21])/HZ;
        //std::cout<<"procStartTime : "<<procStartTime<<"\n";
        processTime =  LinuxParser::UpTime() - procStartTime;
        filestream.close();
    }
    catch (std::exception &e)
    {

        std::cerr<<"Error :  unknown exception, "<<e.what()<<std::endl;

    }
    return processTime;


}
