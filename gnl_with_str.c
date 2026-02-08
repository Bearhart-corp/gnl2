#define BUF_SIZE 4
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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

char **recur(int n, char *s, char *buf_static)
{
    ssize_t     byte = 0;
    char        buf[BUF_SIZE];
    char        *line;
    char        **res;
    int len = 0;

    if (*buf_static != 0)
    {
      len = strlen(buf_static);
      memcpy(buf, buf_static, len);
      buf[len] = 0;
      n--;
      memcpy(buf_static, "\0\0\0\0\0", BUF_SIZE);
    }
    else
    {
        byte = (strlen(s) >= BUF_SIZE) ? BUF_SIZE : strlen(s);
        memcpy(buf, s, byte);
      
      if (byte < BUF_SIZE || sep_in(s, '\n', byte) || byte == -1)
      {
          line = malloc(n * BUF_SIZE + byte + 1);
          line[n * BUF_SIZE + byte] = 0;
          memcpy(line + n * BUF_SIZE, buf, byte);
          res = malloc(2 * sizeof(char *));
          res[0] = line;
          res[1] = line + n * BUF_SIZE;
          return res;
      }
    }
    res = recur(n + 1, s + byte, buf_static);
    int i = -1;
    while (++i < byte)
        res[0][n * BUF_SIZE + i] = buf[i];
    return res;
}

char *gnl(char *s)
{
    char        buffer[BUF_SIZE];
    static char buf[BUF_SIZE];
    char        **line;
    int n = 0;
    
    line = recur(n, s, buf);
    int i = 0;int j = 0;
    while(line[1][i] && line[1][i] != '\n')
      i++;
    if (i == BUF_SIZE)
      buf[0] = 0;
    else
    {
      while(line[1][++i])
      {
        buf[j] = line[1][i];
        j++;
        line[1][i] = 0;
      }
      buf[j] = 0;
    }
    return line[0];
}

int main()
{
    char *test;
    test = gnl("012301\n301230\n");
    printf("%s", test);
    free(test);
    test = NULL;
    test = gnl("01230\n");
    printf("%s", test);
    free(test);
    test = NULL;
}
