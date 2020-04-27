#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <stdlib.h>     /* strtof */
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
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

// Read and return the system memory utilization
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
          try {
            mem_total = std::stof(value);
          } catch (...) {}
        }
        else if (key == "MemFree") {
          try {
            mem_free = std::stof(value);
          } catch (...) {}
        }
        else {
          continue;
        }
      }
    }
  }
  return (mem_total - mem_free)/mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string system_time, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> system_time >> idle_time) {
      try {
        return std::stol(system_time);
      } catch (const std::invalid_argument& arg) {
        return 0;
      }
    }
  }
  return 0;
}

// CANDO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// CANDO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// CANDO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// CANDO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string title, time;
  vector<string> time_vector;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> title;
    if (title == "cpu") {
      while(linestream >> time) {
        time_vector.push_back(time);
      }
        return time_vector;
    }
  }
  return time_vector;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string title, total_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> title >> total_processes) {
        if (title == "processes") {
          try {
            return std::stoi(total_processes);
          } catch (const std::invalid_argument& arg) {
            return 0;
          }       
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string title, running_processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> title >> running_processes;
      if (title == "procs_running") {
        try {
          return std::stoi(running_processes);
        } catch (const std::invalid_argument& arg) {
          return 0;
        }
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) { 
    while(std::getline(stream, line)) {
      return line;
    }
  }
  return "";
}

// Read and return the memory used by a process
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
          try {
            return to_string(std::stoi(ram)/1024);
          } catch (const std::invalid_argument& arg) {
            return "";
          }
        }
      }
    }
  }
  
  return "";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string title, uid;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> title) {
        if (title == "Uid") {
          linestream >> uid;
          return uid;
        }
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  int userID {0};
  try {
    userID = std::stoi(Uid(pid));
  } catch (...) {
      return "";
  }
  string username, x, uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      if (linestream >> username >> x >> uid) {
        try {
          if (std::stoi(uid) == userID) {
            return username;
         } 
        } catch (const std::invalid_argument& arg) {
          continue;
        }
      }
    }
  }
  return "";
}

// Read and return the uptime of a process
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
  try {
      return std::stol(uptime);
  } catch (const std::invalid_argument& arg) {
      return 0;
  }
}

// Return cpu utlization vector for a given process
vector<string> LinuxParser::CpuUtilization(int pid) {
  string title, time;
  vector<string> time_vector;
  string line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> time) {
        time_vector.push_back(time);
      }
    }
  }

  return time_vector;
}