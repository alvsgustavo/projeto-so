#include "xeu_utils/StreamParser.h"
#include "unistd.h"
#include "pwd.h"
#include "sys/wait.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace xeu_utils;
using namespace std;

// This function is just to help you learn the useful methods from Command
void io_explanation(const Command &command) {
  // Let's use this input as example: (ps aux >out >o2 <in 2>er >ou)
  // we would print "$       io(): [0] >out [1] >o2 [2] <in [3] 2>er [4] >ou"
  cout << "$       io():";
  for (size_t i = 0; i < command.io().size(); i++) {
    IOFile io = command.io()[i];
    cout << " [" << i << "] " << io.repr();
    // Other methods - if we had 2>file, then:
    // io.fd() == 2
    // io.is_input() == false ('>' is output)
    // io.is_output() == true
    // io.path() == "file"
  }
}

// This function is just to help you learn the useful methods from Command
void command_explanation(const Command &command) {

  /* Methods that return strings (useful for debugging & maybe other stuff) */

  // This prints the command in a format that can be run by our xeu. If you
  // run the printed command, you will get the exact same Command
  {
    // Note: command.repr(false) doesn't show io redirection (i.e. <in >out)
    cout << "$     repr(): " << command.repr() << endl;
    cout << "$    repr(0): " << command.repr(false) << endl;
    // cout << "$   (string): " << string(command) << endl; // does the same
    // cout << "$ operator<<: " << command << endl; // does the same
  }

  // This is just args()[0] (e.g. in (ps aux), name() is "ps")
  {
    cout << "$     name(): " << command.name() << endl;
  }

  // Notice that args[0] is the command/filename
  {
    cout << "$     args():";

    for (int i = 0; i < command.args().size(); i++) {
      cout << " [" << i << "] " << command.args()[i];
    }

    cout << endl;
  }

  /* Methods that return C-string (useful in exec* syscalls) */

  // this is just the argv()[0] (same as name(), but in C-string)
  {
    printf("$ filename(): %s\n", command.filename());
  }

  // This is similar to args, but in the format required by exec* syscalls
  // After the last arg, there is always a NULL pointer (as required by exec*)
  {
    printf("$     argv():");

    for (int i = 0; command.argv()[i]; i++) {
      printf(" [%d] %s", i, command.argv()[i]);
    }

    printf("\n");
  }

  io_explanation(command);
}

// This function is just to help you learn the useful methods from Command
void commands_explanation(const vector<Command> &commands) {
  // Shows a representation (repr) of the command you input
  // cout << "$ Command::repr(0): " << Command::repr(commands, false) << endl;
  cout << "$ Command::repr(): " << Command::repr(commands) << endl
       << endl;

  // Shows details of each command
  for (int i = 0; i < commands.size(); i++) {
    cout << "# Command " << i << endl;
    command_explanation(commands[i]);
    cout << endl;
  }
}


void run_bg(Command &c) {
  pid_t pid = fork();

  c.pop_first();

  if (pid == -1)
    exit(EXIT_FAILURE);
  else if (pid == 0) {
    execvp(c.filename(), c.argv());
  } else {
    cout << "[+] " << pid << endl;
  }
}

void run(const Command &c) {
  pid_t pid = fork();

  if (pid == -1)
    exit(EXIT_FAILURE);
  else if (pid == 0) 
    execvp(c.filename(), c.argv());
  else
    waitpid(pid, NULL, 0);
    
}

void parse_command(Command &c) {
  if (c.name() == "cd")
    chdir(c.argv()[1]);
  else if (c.name() == "exit")
    exit(EXIT_SUCCESS);
  else if (c.name() == "bg") {
    run_bg(c);
  } else
    run(c);
}

void print_prompt() {
  /*
   * char* cwd;
   * getcwd(cwd, sizeof(char) * 1024);
   * 
   * THE CODE ABOVE CAUSES A SEGMENTATION FAULT, BUT WHY?
   */

  cout << getcwd(NULL, sizeof(char) * 1024) << endl; // This one causes a memory leak.
  cout << "$ ";
}

int main() {

  for (;;) {
    print_prompt();
    vector<Command> commands = StreamParser().parse().commands();

    for (int i = 0; i < commands.size(); i++)
      parse_command(commands[i]);
    cout << endl;
  }

  return 0;
}
