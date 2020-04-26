#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <stdlib.h>     /* strtof */

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
// CORRECTION: Added version
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float mem_total {0}, mem_free {0}, mem_available {0}, buffers {0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          mem_total = std::stof(value);
        }
        else if (key == "MemFree") {
          mem_free = std::stof(value);
        }
        else if (key == "MemAvailable") {
          mem_available = std::stof(value);
        }
        else if (key == "Buffers") {
          buffers = std::stof(value);
        }
        else {
          continue;
        }
      }
    }
  }
  return (mem_total - mem_free)/mem_total; //CHECK
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string system_time, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> system_time >> idle_time) {
      return std::stol(system_time);
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string title, time;
  vector<string> time_vector;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> title;
    while(linestream >> time) {
      time_vector.push_back(time);
    }
  }
  return time_vector;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string title, total_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> title >> total_processes) {
        if (title == "processes") {
          return std::stoi(total_processes);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string title, running_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> title >> running_processes;
      if (title == "procs_running") {
        return std::stoi(running_processes);
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
// DONE REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) { 
    while(std::getline(stream, line)) {
      return line;
    }
  }
  return "Error in LinuxParser::Command";
}

// DONE: Read and return the memory used by a process
// DONE REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string title, ram;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> title >> ram) {
        if (title == "VmSize") {
          return ram;
        }
      }
    }
  }
  return "Error in LinuxParser::Ram";
}

// DONE: Read and return the user ID associated with a process
// DONE REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string title, uid1, uid2, uid3, uid4;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> title >> uid1 >> uid2 >> uid3 >> uid4) {
        if (title == "Uid") {
          return uid1;
        }
      }
    }
  }
  return "Error in LinuxParser::Uid";
}

// DONE: Read and return the user associated with a process
// DONE REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  int userID = std::stoi(Uid(pid));
  string username, x, uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> uid) {
        if (std::stoi(uid) == userID) {
          return username;
        }
      }
    }
  }
  return "Error in LinuxParser::User(pid)";
}

// DONE: Read and return the uptime of a process
// DONE REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int idx=0; idx<22; idx++) {
        linestream >> uptime; 
      }
    }
  }
  return std::stol(uptime);
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  string title, time;
  vector<string> time_vector;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> title;
    while(linestream >> time) {
      time_vector.push_back(time);
    }
  }
  return time_vector;
}