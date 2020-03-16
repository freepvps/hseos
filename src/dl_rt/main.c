#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void check_error(int status, const char* msg) {
    if (!status) {
        fprintf(stderr, "%s\n", msg);
        exit(-1);
    }
}

struct Module {
    const char* path;
    void* handle;
    time_t mod_time;
    int (*run)(int);
};

void init_module(struct Module* ptr, const char* path) {
    ptr->path = path;
    ptr->handle = NULL;
    ptr->mod_time = 0;
    ptr->run = NULL;
}

void close_module(struct Module* ptr) {
    if (ptr->handle != NULL) {
        dlclose(ptr->handle);
        ptr->handle = NULL;
    }
}

void update_module(struct Module* ptr) {
    struct stat mod_stat;
    stat(ptr->path, &mod_stat);
    int need_open = (
        ptr->handle == NULL
        || mod_stat.st_mtime != ptr->mod_time
    );
    printf("mtime: %d %d\n", (int)mod_stat.st_mtime, need_open);
    if (need_open) {
        close_module(ptr);
    }

    void* handle = dlopen(ptr->path, RTLD_LAZY);
    check_error(handle != NULL, dlerror());

    int (*run_sym)(int) = (int (*)(int))dlsym(handle, "run");
    check_error(run_sym != NULL, dlerror());

    ptr->handle = handle;
    ptr->run = run_sym;
    ptr->mod_time = mod_stat.st_mtime;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./name module_path\n");
        return -1;
    }
    struct Module mod;
    init_module(&mod, argv[1]);
    while (1) {
        int x;
        check_error(scanf("%d", &x) == 1, "read error");
        update_module(&mod);
        int res = mod.run(x);
        printf("res: %d\n", res);
    }
}
