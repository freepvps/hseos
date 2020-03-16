#include <endian.h>
#include <fcntl.h>
#include <memory.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


int main(int argc, char** argv) {
    time_t cur_time;
    time(&cur_time);
    struct timeval m_cur_time;
    gettimeofday(&m_cur_time, NULL);

    printf("%d\n", (int)cur_time);
    printf(
        "%d (%d usec)\n",
        (int)m_cur_time.tv_sec,
        (int)m_cur_time.tv_usec
    );

    cur_time += 20 * 365 * 24 * 60 * 60;
    struct tm* time_info = localtime(&cur_time);

    char str[1024];
    size_t size = strftime(
        str,
        sizeof(str) - 1,
        "%Y-%m-%d-%H:%M:%S",
        time_info
    );

    printf("%s\n", str);

    char* str2 = ctime(&cur_time);
    printf("%s\n", str2);

    return 0;
}
