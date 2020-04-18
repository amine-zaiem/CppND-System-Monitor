#include <string>

#include "format.h"

#define HOUR 3600
#define MIN 60

using std::string;

/* Helper that returns the elapsed time from number of seconds
 *  INPUT: Long int measuring seconds
 *  OUTPUT: HH:MM:SS
 */

string Format::ElapsedTime(long seconds) {

  long timeHour = seconds/HOUR;
  long reSeconds = seconds%HOUR;
  long timeMinute = reSeconds/MIN;
  long timeSeconds = reSeconds%MIN;


  string elapsedTime = timeToString(timeHour)+":"+timeToString(timeMinute)+
                    ":"+timeToString(timeSeconds);
  return elapsedTime;
}

/*
 * Helper that converts the long time type to std::string
 */
string Format::timeToString(long time)
{
   std::string timeStr;
   if ( time > 9)
   {
	   timeStr = std::to_string(time);
   }
   else
   {
	   timeStr = "0"+std::to_string(time);
   }
   return timeStr;
}
