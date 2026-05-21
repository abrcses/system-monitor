#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_new, std::string user_new, std::string cmd_new)
      : pid(pid_new),
        user(user_new),
        command(cmd_new),
        total_time_prev(0),
        elapsed_prev(0),
        cpu_util_last(0.0f),
        ram_last(0) {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  void CalcCpuUtilization();
  std::string CpuUtilization();                  // TODO: See src/process.cpp
  void CalcRam();
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid;
  std::string user;
  std::string command;
  long total_time_prev;
  long elapsed_prev;
  float cpu_util_last;
  int ram_last;
};

#endif