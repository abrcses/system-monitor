#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

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
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  int value;
  float memutil = 0.0f;
  int memtotal = -1;
  int memfree = -1;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memtotal = value;
        } else if (key == "MemFree:") {
          memfree = value;
        }
        if (memtotal != -1 && memfree != -1) {
          int memused = memtotal - memfree;
          memutil = static_cast<float>(memused) / memtotal;
          break;
        }
      }
    }
  }
  return memutil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  double uptime_d = 0.0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_d;
  }
  long uptime = static_cast<long>(uptime_d);
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, id;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> id;
      if (id == "cpu") {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        long active = user + nice + system + irq + softirq + steal;
        return active;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, id;
  long user, nice, system, idle, iowait;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> id;
      if (id == "cpu") {
        linestream >> user >> nice >> system >> idle >> iowait;
        long idle_total = idle + iowait;
        return idle_total;
      }
    }
  }
  return 0;
}

// TODO: Read and return values for CPU utilization
vector<long> LinuxParser::CpuUtilization(int pid) {
  string token;
  long utime = -1, stime = -1, cutime = -1, cstime = -1, starttime = -1;
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; ++i) {
      linestream >> token;
    }
    linestream >> utime >> stime >> cutime >> cstime >> token >> token >> token >> token >> starttime;
  }
  if (starttime == -1) {
    return {};
  } else {
    return {utime, stime, cutime, cstime, starttime};
  }
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return Pids().size();
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Ram(int pid) {
  string line, key;
  int ram;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmRSS:") {
        linestream >> ram;
        return ram;
      }
    }
  }
  return 0;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Uid(int pid) {
  string line;
  string key;
  int value = -1;
  std::ifstream statusfile(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (statusfile.is_open()) {
    while (std::getline(statusfile, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return -1;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  int search_uid = Uid(pid);
  if (search_uid == -1) {
    return "NA";
  }

  string line, user, pw;
  int uid;
  std::ifstream passwdfile(kPasswordPath);
  if (passwdfile.is_open()) {
    while (std::getline(passwdfile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> user >> pw >> uid) {
        if (uid == search_uid) {
          return user;
        }
      }
    }
  }

  return "NA";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string token;
  long starttime = 0;
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; ++i) {
      linestream >> token;
    }
    linestream >> starttime;
  }
  if (starttime == 0) {
    return 0;
  }
  return UpTime() - starttime / sysconf(_SC_CLK_TCK);
}
