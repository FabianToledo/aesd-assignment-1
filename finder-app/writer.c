#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void usage(char *name)
{
    printf("Writes a string into a file\n");
    printf("Usage:\n");
    printf("%s <writefile> <writestr>\n", name);
    printf("\n"); 
    printf("writefile    File to write to\n");
    printf("writestr     String to write into the file\n");
}

int main(int argc, char *argv[])
{
    openlog(argv[0], LOG_CONS, LOG_USER);

    if (argc != 3)
    {
        printf("Expected 2 parameters. %d Given.\n", argc-1);
        usage(argv[0]);
        return 1;
    }

    const char *file = argv[1];
    const char *str = argv[2];

    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd == -1)
    {
        syslog(LOG_ERR, "%s", strerror(errno));
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", str, file);
    ssize_t nw = write(fd, str, strlen(str));
    if (nw == -1)
    {
        syslog(LOG_ERR, "%s", strerror(errno));
        return 1;
    }

    int res = close(fd);
    if (res == -1)
    {
        syslog(LOG_ERR, "%s", strerror(errno));
        return 1;
    }

    closelog();

    return 0;
}