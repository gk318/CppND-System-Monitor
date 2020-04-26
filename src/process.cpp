#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//Constructor
Process::Process(int pid) {
    this->process_id = pid;
}

// TODO: Return this process's ID
int Process::Pid() {
    return this->process_id; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    vector<string> time_vector = LinuxParser::CpuUtilization(this->process_id);
    float total_time = (std::stof(time_vector[14]) + std::stof(time_vector[15]) + std::stof(time_vector[16]) +
        std::stof(time_vector[17])) / sysconf(_SC_CLK_TCK);
    float elapsed_time = this->UpTime() - std::stof(time_vector[22]) / sysconf(_SC_CLK_TCK);
    return 100*(total_time / elapsed_time);
}

// DONE: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(this->process_id);
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(this->process_id);
}

// DONE: Return the user (name) that generated this process
string Process::User() {
    int process_id = Pid();
    return LinuxParser::User(process_id);
 }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    long int clock_ticks = LinuxParser::UpTime(this->process_id);
    return clock_ticks / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }