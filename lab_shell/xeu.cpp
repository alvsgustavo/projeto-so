#include "xeu_utils/StreamParser.h"
#include "unistd.h"
#include "sys/wait.h"
#include "linux/limits.h"
#include "pthread.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace xeu_utils;
using namespace std;

struct bg_process {
  pid_t pid;
  string status;
};

vector<bg_process> bg_processes;
bool is_xjobs_thread_active;
pthread_mutex_t lock;

struct thread_argv {
  pid_t pid;
  Command command;
};

pid_t fork_and_exec(const Command &c) {
  pid_t pid = fork();

  if (pid == -1)
    cout << "Failed to fork process" << endl;
  
  else if (pid == 0 && execvp(c.filename(), c.argv()) == -1)
    cout << "Error whilst trying to execute " << c.filename() << endl;
  
  return pid;
}

void print_xjobs() {
  if (bg_processes.size() == 0) {
    cout << "There are no background jobs." << endl;
  
  } else {
    pthread_mutex_lock(&lock);
    auto it = bg_processes.begin();
    while (it != bg_processes.end()) {
      cout << it->pid << " - " << it->status << endl;
      it->status == "dead"? it = bg_processes.erase(it) : it++;
    }
    if (bg_processes.size() == 0) {
      is_xjobs_thread_active = false;
      pthread_mutex_unlock(&lock);
      pthread_mutex_destroy(&lock);
    
    } else
      pthread_mutex_unlock(&lock);
  }
}

void *xjobs(void* args) {
  while (is_xjobs_thread_active) {
    pthread_mutex_lock(&lock);
    if (bg_processes.size() == 0) {
      is_xjobs_thread_active = false;
    
    } else {
      for (auto job = bg_processes.begin(); job != bg_processes.end(); job++) {
        bool is_dead;

        if (job->status == "running" && waitpid(job->pid, NULL, WNOHANG) > 0) {
          is_dead = true;
          cout << "[-] " << job->pid << " has finished." << endl;
        }
        if (is_dead) {
          job->status = "dead";
          is_dead = false;
        }
      }
    }
    pthread_mutex_unlock(&lock);
  }
}

void run_bg(Command& c) {
  c.pop_first();
  pid_t pid = fork_and_exec(c);
  int status;

  if (pid > 0 & is_xjobs_thread_active) {
    bg_process job;
    job.pid = pid;
    job.status = "running";
    pthread_mutex_lock(&lock);
    bg_processes.push_back(job);
    pthread_mutex_unlock(&lock);
    cout << "[+] " << job.pid << ": " << c.name() << endl;
  
  } else if (pid > 0) {
    pthread_t thread_id;
    bg_process job;

    pthread_create(&thread_id, NULL, *xjobs, NULL);
    is_xjobs_thread_active = true;
    job.pid = pid;
    job.status = "running";
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_lock(&lock);
    bg_processes.push_back(job);
    pthread_mutex_unlock(&lock);
    cout << "[+] " << job.pid << ": " << c.name() << endl;
  }
}

void run(Command& c) {
  pid_t pid = fork_and_exec(c);
  int status;

  if (pid > 0)
    waitpid(pid, &status, 0);
}

void parse_command(Command &c) {
  string command = c.name();

  if (command == "cd")
    chdir(c.argv()[1]);

  else if (command == "exit")
    exit(EXIT_SUCCESS);

  else if (command == "bg" && c.args().size() > 1)
    run_bg(c);

  else if (command == "bg")
    cout << "use: bg [PROGRAMME]..." << endl;

  else if (command == "xjobs")
    print_xjobs();

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
