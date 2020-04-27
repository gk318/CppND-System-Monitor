#include <vector>

#include "processor.h"
#include "linux_parser.h"


// Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> time_vector = LinuxParser::CpuUtilization();
    float idle {0}, non_idle {0};
    try {
        idle = std::stof(time_vector[3]) + std::stof(time_vector[4]);
    } catch(...) {}
    try {
        non_idle = std::stof(time_vector[0]) + std::stof(time_vector[1]) +
            std::stof(time_vector[2]) + std::stof(time_vector[5]) +
            std::stof(time_vector[6]) + std::stof(time_vector[7]);
    } catch(...) {};
    float total = idle + non_idle;
    return total ? (non_idle/total) : 0;
}