#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
    return pid;
}

void Process::CalcCpuUtilization() {
    vector<long> times = LinuxParser::CpuUtilization(pid);
    if (times.empty()) {
        cpu_util_last = -0.1f;
        return;
    }
    long total_time = times[0] + times[1] + times[2] + times[3];
    long elapsed = LinuxParser::UpTime() - times[4] / sysconf(_SC_CLK_TCK);
    int total_time_diff = total_time - total_time_prev;
    int elapsed_diff = elapsed - elapsed_prev;
    if (total_time_diff < 0 || elapsed_diff <= 0) {
        cpu_util_last = -0.1f;
        return;
    }
    cpu_util_last = (static_cast<double>(total_time_diff) / sysconf(_SC_CLK_TCK)) / elapsed_diff;
    total_time_prev = total_time;
    elapsed_prev = elapsed;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpu_util_last;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return command;
}

void Process::CalcRam() {
    ram_last = LinuxParser::Ram(pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    float ram_mb = static_cast<float>(ram_last) / 1024.0f;
    std::stringstream sstream;
    sstream << std::fixed << std::setprecision(1) << ram_mb;
    return sstream.str();
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    if (this->ram_last > a.ram_last) {
        return true;
    }
    return false;
}