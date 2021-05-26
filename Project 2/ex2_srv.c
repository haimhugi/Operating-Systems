#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

int isConnected = 0;

void whenSignalCome();
int my_strlen(const char *str);
void tostring(char str[], int num);
void my_strcat(char *destination, char *source);
void my_strcpy(char *destination, char *source);
int myAtoi(char *str);
float convertStrToFloat(const char *s);
void ifServerDosntGetRequest();

int main()
{
    unlink("to_srv");
    signal(SIGUSR1, whenSignalCome);
    signal(SIGALRM, ifServerDosntGetRequest);
    printf("end of stage 5.c\n");
    alarm(60);
    while (isConnected == 0)
    {
        ;
    }
}
void ifServerDosntGetRequest()
{
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    isConnected = 1;
}

void whenSignalCome()
{
    alarm(60);
    int parentPid = getpid();
    pid_t chiled;
    int z;
    if ((chiled = fork()) == 0)
    {
        printf("end of stage 5.f\n");
        // int errorFD = open("errorFile.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        signal(SIGUSR1, whenSignalCome);
        char tav;
        int lineCount = 0;
        int pidServer = getpid();
        char *childPid = (char *)calloc(1, sizeof(char));
        int childPidBuffSize = 0;
        char *firstNum = (char *)calloc(1, sizeof(char));
        int firstNumBuffSize = 0;
        char *Operator = (char *)calloc(1, sizeof(char));
        int OperatorBuffSize = 1;
        Operator[OperatorBuffSize - 1] = '\0';
        char *secondNum = (char *)calloc(1, sizeof(char));
        int secondNumBuffSize = 1;
        secondNum[secondNumBuffSize - 1] = '\0';

        int FDsrv = open("to_srv", O_RDONLY);
        if (FDsrv == -1)
        {
            free(secondNum);
            free(Operator);
            free(firstNum);
            free(childPid);
            perror("ERROR FROM_EX2.\n");
            exit(1);
        }

        int readChar = 1;

        while (readChar > 0)
        {
            readChar = read(FDsrv, &tav, 1);
            if (readChar > 0)
            {
                if (tav == '\n')
                {
                    lineCount++;
                }
                else
                {
                    if (lineCount == 0)
                    {
                        childPidBuffSize++;
                        childPid = (char *)realloc(childPid, childPidBuffSize);
                        childPid[childPidBuffSize - 1] = tav;
                    }
                    else if (lineCount == 1)
                    {
                        firstNumBuffSize++;
                        firstNum = (char *)realloc(firstNum, firstNumBuffSize);
                        firstNum[firstNumBuffSize - 1] = tav;
                    }
                    else if (lineCount == 2)
                    {
                        OperatorBuffSize++;
                        Operator = (char *)realloc(Operator, OperatorBuffSize);
                        Operator[OperatorBuffSize - 2] = tav;
                        Operator[OperatorBuffSize - 1] = '\0';
                    }
                    else if (lineCount == 3)
                    {
                        secondNumBuffSize++;
                        secondNum = (char *)realloc(secondNum, secondNumBuffSize);
                        secondNum[secondNumBuffSize - 2] = tav;
                        secondNum[secondNumBuffSize - 1] = '\0';
                    }
                }
            }
        }
        close(FDsrv);
        unlink("to_srv");
        printf("end of stage 5.g\n");
        char *toClinetStr = (char *)calloc(my_strlen("to_client_"), sizeof(char));
        my_strcpy(toClinetStr, "to_client_");
        char *nameFileChild = (char *)calloc(10 + my_strlen(childPid), sizeof(char));
        my_strcpy(nameFileChild, toClinetStr);
        my_strcat(nameFileChild, childPid);
        int toClientChiledFD = open(nameFileChild, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (toClientChiledFD == -1)
        {
            free(secondNum);
            free(Operator);
            free(firstNum);
            free(childPid);
            perror("ERROR FROM_EX2.\n");
            exit(1);
        }
        printf("end of stage 5.h\n");

        int intOperator = Operator[0] - '0';
        free(Operator);
        float result;
        float intfirstNum = convertStrToFloat(firstNum);
        free(firstNum);
        float intsecondNum = convertStrToFloat(secondNum);
        free(secondNum);

        switch (intOperator)
        {
        case 1:
            result = intfirstNum + intsecondNum;
            break;
        case 2:
            result = intfirstNum - intsecondNum;
            break;
        case 3:
            result = intfirstNum * intsecondNum;
            break;
        case 4:
            result = intfirstNum / intsecondNum;
            break;

        default:
            free(childPid);
            perror("ERROR FROM_EX2.\n");
            exit(1);
        }

        write(toClientChiledFD, &result, sizeof(result));
        int childPidint = myAtoi(childPid);
        int size11 = my_strlen(childPid);
        free(childPid);
        close(toClientChiledFD);
        kill(childPidint, SIGUSR2);
        printf("end of stage 5.i\n");
        exit(1);
    }
    else
    {
        wait(&z);
    }
}

int myAtoi(char *str)
{
    int res = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}

int my_strlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
void my_strcpy(char *destination, char *source)
{
    char *start = destination;

    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0';
}

void my_strcat(char *destination, char *source)
{

    char *ptr = destination + my_strlen(destination);

    while (*source != '\0')
        *ptr++ = *source++;

    *ptr = '\0';
}

float convertStrToFloat(const char *s)
{
    float rez = 0, fact = 1;
    if (*s == '-')
    {
        s++;
        fact = -1;
    };
    for (int point_seen = 0; *s; s++)
    {
        if (*s == '.')
        {
            point_seen = 1;
            continue;
        };
        int d = *s - '0';
        if (d >= 0 && d <= 9)
        {
            if (point_seen)
                fact /= 10.0f;
            rez = rez * 10.0f + (float)d;
        };
    };
    return rez * fact;
};
