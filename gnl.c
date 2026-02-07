#define BUF_SIZE 5
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int sep_in(char *s, char c, ssize_t byte)
{
    int i = 0;
    while (i < byte)
    {
        if (s[i] == c)
            return 1;
        i++;
    }
    return 0;
}

char **recur(int n, int fd)
{
    ssize_t     byte = 0;
    char        buf[BUF_SIZE];
    char        *line;
    char        **res;

    byte = read(fd, buf, BUF_SIZE);
    if (byte == 0 || sep_in(buf, '\n', byte) || byte == -1)
    {
        line = malloc((n + 1) * BUF_SIZE);
        res = malloc(2 * sizeof(char *));
        res[0] = line;
        res[1] = line + n * BUF_SIZE;
        return res;
    }
    res = recur(n + 1, fd);
    printf("par la");
    int i = -1;
    while (++i < byte)
        res[0][n + i] = buf[i];
    return res;
}

char *gnl(int fd)
{
    char        buffer[BUF_SIZE];
    static char buf[BUF_SIZE];
    char        **line;

    if (buf[0] == 0)
    {
        printf("ici");
        line = recur(0, fd);
    }
    return (char *)line[0];
}

int main()
{
    int fd;
    fd = open("text.txt", O_RDONLY);
    char *test;
    test = gnl(fd);
    printf("%s", test);
    free(test);
    close(fd);
}
