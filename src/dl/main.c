#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

void check_error(int status, const char* msg) {
    if (!status) {
        fprintf(stderr, "%s\n", msg);
        exit(-1);
    }
}


void exec_module(const char* path) {
    void* handle = dlopen(path, RTLD_LAZY);
    check_error(handle != NULL, dlerror());

    void (*run_sym)() = (void (*)())dlsym(handle, "run");
    check_error(run_sym != NULL, dlerror());

    run_sym();

    dlclose(handle);
}

void combine_path(const char* a, const char* b, char* dst) {
    strcpy(dst, a);
    if (dst[strlen(dst) - 1] != '/') {
        strcpy(dst + strlen(dst), "/");
    }
    strcpy(dst + strlen(dst), b);
}

void list_modules(const char* dir_path) {
    char full_path[PATH_MAX];
    DIR* dir = opendir(dir_path);
    check_error(dir != NULL, "Open dir error");

    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);
        if (len > 3 && strncmp(&entry->d_name[len - 3], ".so", 3) == 0) {
            combine_path(dir_path, entry->d_name, full_path);
            printf("Module name: %s (%s)\n", entry->d_name, full_path);
            exec_module(full_path);
        }
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./name module_name\n");
        return -1;
    }
    list_modules(argv[1]);
    return 0;
}
