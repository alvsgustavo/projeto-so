#include "xeu_utils/StreamParser.h"
#include "unistd.h"
#include "pwd.h"
#include "sys/wait.h"
#include <pthread.h>

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace xeu_utils;
using namespace std;

vector<struct thread_data*> bgp{};
pthread_mutex_t lock;

struct thread_data {
  Command c;
  pthread_t t;
  pid_t pid;
};


void *run_bg(void *vargp) {
  struct thread_data *args = (struct thread_data *) vargp;
  pthread_t thread = args->t;
  Command c = args->c;

  pid_t pid = fork();

  if (pid == -1) {
    cout << "Failed to fork process" << endl;

  } else if (pid == 0) {
    if (execvp(c.filename(), c.argv()) == -1)
      cout << "Error whilst trying to execute " << c.filename() << endl;

  } else {
    args->pid = pid;
    cout << "[+] " << pid << endl;
    pthread_mutex_lock(&lock);
    bgp.push_back(args);
    pthread_mutex_unlock(&lock);
    waitpid(args->pid, NULL, 0);
    pthread_mutex_lock(&lock);
    for (int i = 0; i < bgp.size(); i++) {
      struct thread_data proc = *(bgp.at(i));

      if (proc.pid == pid && proc.c.name() == c.name()) {
        bgp.erase(bgp.begin() + i);
        break;    
      }
    }

    pthread_mutex_unlock(&lock);
  }
}

void run(const Command &c) {
  pid_t pid = fork();

  if (pid == -1)
    cout << "Failed to fork process" << endl;
  else if (pid == 0) 
    if (execvp(c.filename(), c.argv()) == -1)
      cout << "Error whilst trying to execute " << c.filename() << endl;

  else
    waitpid(pid, NULL, 0);
    
}

void parse_command(Command &c) {
  if (c.name() == "cd")
    chdir(c.argv()[1]);
  else if (c.name() == "exit")
    exit(EXIT_SUCCESS);
  else if (c.name() == "xjobs") {
    cout << "XJOBS" << endl;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < bgp.size(); i++) {
      struct thread_data p = *(bgp.at(i));
      cout << p.pid << " " << p.c.name() << endl; 
    }
    pthread_mutex_unlock(&lock);

  } else if (c.name() == "bg") {
    struct thread_data vargp;
    pthread_t thread_id;
    vargp.c = c;
    vargp.t = thread_id;
    pthread_create(&thread_id, NULL, &run_bg, (void *)&vargp);
    
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

  std::cout << getcwd(NULL, sizeof(char) * 1024) << endl; // This one causes a memory leak.
  std::cout << "$ ";
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
