#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int
main(int argc, char **argv)
{
  int pid;
  int sysNumber;
  int traces;
  int callCounts;

  static char* syscalls[] = {
    [SYS_fork]    "sys_fork",
    [SYS_exit]    "sys_exit",
    [SYS_wait]    "sys_wait",
    [SYS_pipe]    "sys_pipe",
    [SYS_read]    "sys_read",
    [SYS_kill]    "sys_kill",
    [SYS_exec]    "sys_exec",
    [SYS_fstat]   "sys_fstat",
    [SYS_chdir]   "sys_chdir",
    [SYS_dup]     "sys_dup",
    [SYS_getpid]  "sys_getpid",
    [SYS_sbrk]    "sys_sbrk",
    [SYS_sleep]   "sys_sleep",
    [SYS_uptime]  "sys_uptime",
    [SYS_open]    "sys_open",
    [SYS_write]   "sys_write",
    [SYS_mknod]   "sys_mknod",
    [SYS_unlink]  "sys_unlink",
    [SYS_link]    "sys_link",
    [SYS_mkdir]   "sys_mkdir",
    [SYS_close]   "sys_close",
    [SYS_getpriority] "sys_getpriority",
    [SYS_setpriority] "sys_setpriority",
    [SYS_getusage] "sys_getusage",
    [SYS_trace] "sys_trace",
    [SYS_ps] "sys_ps"
  };

  if(argc < 2){
    printf(2, "usage: trace pid optional(sysNumber)...\n");
    exit();
  }
  pid = atoi(argv[1]);
  if(argv[2]){
    sysNumber = atoi(argv[2]);
    traces = 1;
  } else {
    sysNumber = 1;
    traces = 26;
  }
    
  for (int i = 1; i <= traces; i++){
    callCounts = trace(pid, sysNumber);
    if (callCounts == -1){
      printf(2, "Something went wrong. Possibly process id invalid, syscall number invalid or system isn't tracking this syscall yet.\n");
    } else {
      printf(1, "PID(%d) has made %d %s syscalls.\n", pid, callCounts, syscalls[sysNumber]);
    }
    sysNumber++;
  }
  
  exit();
}