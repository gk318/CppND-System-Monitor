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
Process::Process(int pid) : process_id_(pid) {
    SetCpuUtilization();
    SetCommand();
    SetRam();
    SetUser();
    SetUptime();
}

// DONE: Return this process's ID
int Process::Pid() {
    return this->process_id_;    
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    return this->cpu_utilization_;
}

// DONE: Return the command that generated this process
string Process::Command() {
    return this->command_;
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    return this->ram_;
}

// DONE: Return the user (name) that generated this process
string Process::User() {
    //int process_id = Pid();
    return this->user_;
 }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    return this->uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->cpu_utilization_ > a.cpu_utilization_;
}

void Process::SetCpuUtilization() {
    vector<string> time_vector = LinuxParser::CpuUtilization(this->process_id_);
    float total_time {0};
    float elapsed_time {0};
    try {
        total_time = std::stof(time_vector[13]) + std::stof(time_vector[14]) + std::stof(time_vector[15]) +
            std::stof(time_vector[16]);
    } catch (...) {}
    try {
        elapsed_time = (LinuxParser::UpTime()) - (std::stof(time_vector[21]) / sysconf(_SC_CLK_TCK));
    } catch (...) {}
    total_time = total_time/sysconf(_SC_CLK_TCK);
    this->cpu_utilization_ = total_time / elapsed_time;
}

void Process::SetCommand() {
    this->command_ = LinuxParser::Command(this->process_id_);
}

void Process::SetRam() {
    this->ram_ = LinuxParser::Ram(this->process_id_);
}

void Process::SetUser() {
    this->user_ = LinuxParser::User(this->process_id_);
}

void Process::SetUptime(){
    long int clock_ticks = LinuxParser::UpTime(this->process_id_);
    this->uptime_ = clock_ticks / sysconf(_SC_CLK_TCK);
}