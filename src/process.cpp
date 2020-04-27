#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//Constructor
Process::Process(int pid) : process_id(pid) {
    //this->cpu_utilization = CpuUtilization();
}

// DONE: Return this process's ID
int Process::Pid() {
    return this->process_id; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    vector<string> time_vector = LinuxParser::CpuUtilization(this->process_id);
    long int total_time = (std::stol(time_vector[13]) + std::stol(time_vector[14]) + std::stol(time_vector[15]) +
        std::stol(time_vector[16])) / sysconf(_SC_CLK_TCK);
    long int elapsed_time = (LinuxParser::UpTime()) - (std::stol(time_vector[21]) / sysconf(_SC_CLK_TCK));
    /*if (total_time) {
        std::cout << "total_time: " << total_time * sysconf(_SC_CLK_TCK) << "\n";
        std::cout << "elapsed_time " << std::stol(time_vector[21]) << "\n";
    }*/
    //std::cout << "Uptime:" << LinuxParser::UpTime()/sysconf(_SC_CLK_TCK) << "  " << "sub:" << std::stol(time_vector[21]) / sysconf(_SC_CLK_TCK) << "\n";
    return (total_time / elapsed_time);
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
    //int process_id = Pid();
    return LinuxParser::User(this->process_id);
 }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    long int clock_ticks = LinuxParser::UpTime(this->process_id);
    return clock_ticks / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->cpu_utilization > a.cpu_utilization;
}