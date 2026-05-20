#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long active = LinuxParser::ActiveJiffies();
    long idle = LinuxParser::IdleJiffies();
    long active_diff = active - active_prev;
    long idle_diff = idle - idle_prev;
    float util = static_cast<float>(active_diff) / (active_diff + idle_diff);
    active_prev = active;
    idle_prev = idle;
    return util;
}