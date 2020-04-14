# exec
```c
// exec[lvpe]
// l
execl("/bin/ls", "ls", "-lah", NULL);

// v
char* args[] = {"ls", "-lah", NULL};
execv("/bin/ls", args);

// lp
execlp("ls", "ls", "-lah", NULL);

// vp
char* args[] = {"ls", "-lah", NULL};
execv("ls", args);

// le
execle("/bin/ls", "ls", "-lah", NULL, "PATH=./", NULL);

// ve
char* args[] = {"ls", "-lah", NULL};
char* env[] = {"PATH=./", NULL};
execve("/bin/ls", "ls", "-lah", NULL, "PATH=./", NULL);

```

**FD_CLOEXEC** - close fd on exec
