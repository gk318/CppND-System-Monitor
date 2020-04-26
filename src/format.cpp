#include <string>
#include <sstream>
#include<iomanip>
#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string elapsed_time;
    std::stringstream hours, mins, secs;
    hours << std::setw(2) << std::setfill('0') << seconds/3600;
    mins << std::setw(2) << std::setfill('0') << (seconds % 3600)/60;
    secs << std::setw(2) << std::setfill('0') << (seconds % 3600)%60;

    elapsed_time = hours.str() + ":" + mins.str() + ":" + secs.str();
    return elapsed_time;
}