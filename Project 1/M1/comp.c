#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int firstFileFD;
    int secondFileFD;
    char *buff1;
    char *buff2;
    buff1 = calloc(1, sizeof(char));
    buff2 = calloc(1, sizeof(char));

    firstFileFD = open(argv[1], O_RDONLY);
    if (firstFileFD == -1)
    {
        perror("Cant open first file");
        exit(1);
    }
    secondFileFD = open(argv[2], O_RDONLY);
    if (secondFileFD == -1)
    {
        perror("Cant open second file");
        exit(1);
    }

    int readFirst = 1;
    int readSecond = 1;

    while (readFirst && readSecond)
    {
        readFirst = read(firstFileFD, buff1, 1);
        readSecond = read(secondFileFD, buff2, 1);
        if (buff1[0] != buff2[0])
        {
            return 1;
        }
    }
    if (readFirst != 0 && readSecond != 0)
    {
        return 1;
    }

    free(buff1);
    free(buff2);
    close(readFirst);
    close(readSecond);
    return 2;
}
