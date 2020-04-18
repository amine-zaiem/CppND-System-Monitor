#include "processor.h"
#include <chrono>
#include <thread>
#include <iostream>

/*
 *  Helper to return the aggregate CPU utilization
 */
float Processor::Utilization()
{
    const float prevSnapAllJf = (float)allJiffies_;
    const float prevSnapActiveJf = (float)activeJiffies_;
    float cpuUtilization = 0.0;


    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_));


    const float currSnapAllJf = (float)LinuxParser::Jiffies();
    const float currSnapActiveJf = (float)LinuxParser::ActiveJiffies();

    //try
    {
       cpuUtilization = ((currSnapActiveJf - prevSnapActiveJf) / (currSnapAllJf-prevSnapAllJf));
    }
    //catch (std::exception &e)
    {

       //LinuxParser::exception_text = e.what();

    }
    return cpuUtilization;
}
