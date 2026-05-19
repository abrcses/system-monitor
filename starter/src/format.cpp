#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int hours = seconds / 3600;
    seconds = seconds - hours * 3600;
    int mins = seconds / 60;
    seconds = seconds - mins * 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":" << mins << ":" << seconds;
    return oss.str();
}