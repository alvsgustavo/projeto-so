#include "xeu_utils/StreamParser.h"
#include "unistd.h"
#include "pwd.h"
#include "sys/wait.h"
#include "pthread.h"
#include "linux/limits.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace xeu_utils;
using namespace std;

pid_t fork_and_exec(const Command &c) {
  pid_t pid = fork();

  if (pid == -1)
    cout << "Failed to fork process" << endl;
  
  else if (pid == 0 && execvp(c.filename(), c.argv()) == -1) {
      cout << "Error whilst trying to execute " << c.filename() << endl;
  }

  return pid;
}

void run(Command &c) {
  pid_t pid = fork_and_exec(c);
  int status;

  if (pid > 0)
    waitpid(pid, &status, 0);
}


void run_bg(Command &c) {
  
}

void parse_command(Command &c) {
  string command = c.name();

  if (command == "cd")
    chdir(c.argv()[1]);

  else if (command == "exit")
    exit(EXIT_SUCCESS);

  else if (command == "bg")
    run_bg(c);

  else
    run(c);
}

void print_prompt() {
  char path[PATH_MAX];
  getcwd(path, sizeof(path));
  cout << path << endl << "$ ";
}

int main() {
  for (;;) {
    print_prompt();
    vector<Command> commands = StreamParser().parse().commands();

    for (Command& c: commands)
      parse_command(c);
    cout << endl;
  }

  return 0;
}
