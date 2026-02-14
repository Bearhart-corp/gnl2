#define BUF_SIZE 10000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int sep_in(char *s, char c, ssize_t byte, int *pos_n)
{
    int i = 0;
    while (i < byte)
    {
        if (s[i] == c)
        {
            *pos_n = i;
            return 1;
        }
        i++;
    }
    return 0;
}

void ft_memcpy(char *dest, char *src, int n)
{
    while (n--)
        dest[n] = src[n];
}

ssize_t helper(char *buf, char *buf_static, int *pos_n)
{
    int len;

    len = 0;
    while (buf_static[len])
        len++;
    ft_memcpy(buf, buf_static, len);
    buf[len] = 0;
    *pos_n = len - 1;
    buf_static[0] = 0;
    return len;
}

char *recur(int fd, char *buf_static, int real_size, int *pos_n)
{
    ssize_t     byte;
    char        buf[BUF_SIZE];
    char        *line;

    if (*buf_static != 0)
        byte = helper(buf, buf_static, pos_n);
    else
    {
        byte = read(fd, buf, BUF_SIZE);
        if (byte == 0 || sep_in(buf, '\n', byte, pos_n) || byte == -1)
        {
            if(byte == 0 && real_size == 0)
                return NULL;
            line = malloc(real_size + byte + 1);
            if (!line)
                exit(1);
            *pos_n = *pos_n + real_size;
            line[real_size + byte] = 0;
            return (ft_memcpy(line + real_size, buf, byte), line);
        }
    }
    line = recur(fd, buf_static, real_size + byte, pos_n);
    return (ft_memcpy(line + real_size, buf, byte), line);
}

char *gnl(int fd)
{
    static char buf[BUF_SIZE];
    char        *line;
    int         pos_n;
    int         j;
    const char  trik[BUF_SIZE] = {0};

    (void)fd;
    line = recur(fd, buf, 0, &pos_n);
    if (line == NULL)
        return NULL;
    j = 0;
    if (line[pos_n] != '\n')
        return line;
    while(line[++pos_n] )
    {
        buf[j] = line[pos_n];
        j++;
        line[pos_n] = 0;
    }
    ft_memcpy(buf + j, (char*)trik, BUF_SIZE - j);
    return line;
}

int main()
{
    int fd;
    fd = open("test.txt", O_RDONLY);
    char *test;
    test = gnl(fd);
    while (test != NULL)
    {
        printf("%s", test);
        free(test);
        test = gnl(fd);
    }
    close(fd);
}
