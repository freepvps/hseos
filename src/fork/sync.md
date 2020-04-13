# Signal sync

1. master SIGUSR1 -> child1 SIGUSR1 handle: write
2. child1 SIGUSR2 -> master SIGUSR2 handle: next
3. master -> child2 write
4. child2 -> master finish

```c
int kill(pid_t pid, int sig);
// pid - process id
// sig - SIGUSR1/SIGUSR2/SIGKILL


typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
```

# Eventfd sync
1. master spawn N event-fd
2. master spawn N children
3. master write child1-efd 0
4. child1 write file
5. child1 write child2-efd 0
...
100. master wait

# Atomic sync
1. master allocate shared memory (sizeof(size_t)) ACTIVE
2. child (while (...) { if (atomic_read(ACTIVE) == current_id) {...} else sched_yeld })
