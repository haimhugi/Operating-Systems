#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/random.h>
#include <time.h>

int my_strCompare(const char *s1, const char *s2);
int my_strlen(const char *str);
void tostring(char str[], int num);
int myAtoi(char *str);
void afterGetResult();
void my_strcat(char *destination, char *source);
void my_strcpy(char *destination, char *source);
void ifServerNotRespone();
int isFileExist(const char *filename);

int main(int argc, char *argv[])
{
    //Validation
    if (argc != 5)
    {
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }

    char *check = argv[3];
    if (my_strlen(check) != 1)
    {
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }
    char check1 = check[0];

    if (check1 != '1' && check1 != '2' && check1 != '3' && check1 != '4')
    {
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }

    int isDigit = 1;
    int j = 0;
    char *argvT = argv[2];
    if (argvT[0] == '-')
    {
        j++;
    }
    while (j < my_strlen(argvT))
    {
        if (argvT[j] > 57 || argvT[j] < 48)
        {
            isDigit = 0;
            break;
        }
        j++;
    }

    if (isDigit == 0)
    {
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }

    int isDigit1 = 1;
    int i = 0;
    char *argvF = argv[4];
    if (argvF[0] == '-')
    {
        i++;
    }
    while (i < my_strlen(argvF))
    {
        if (argvF[i] > 57 || argvF[i] < 48)
        {
            isDigit1 = 0;
            break;
        }
        i++;
    }

    if (isDigit1 == 0)
    {
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }

    int validationTest = open("Validetion.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR); //
    int screenFD = dup(1);
    dup2(validationTest, 1);
    int temp;
    if (fork() == 0)
    {
        execlp("ps", "ps", "-p", argv[1], "-o", "comm=", NULL);
        exit(1);
    }
    else
    {
        wait(&temp);
    }

    dup2(screenFD, 1);
    int readChar = 1;

    char tav;
    char *pidServerName = calloc(1, sizeof(char));
    int pidServerNameBuffSize = 0;
    validationTest = open("Validetion.txt", O_RDONLY, S_IRUSR | S_IWUSR); //
    while (readChar != 0)
    {
        readChar = read(validationTest, &tav, 1);

        if (readChar > 0)
        {
            pidServerNameBuffSize++;
            pidServerName = (char *)realloc(pidServerName, pidServerNameBuffSize);
            pidServerName[pidServerNameBuffSize - 1] = tav;
        }
    }

    close(validationTest);
    unlink("Validetion.txt");

    if (!((my_strCompare("ex2_srv.out\n", pidServerName)) == 0))
    {
        free(pidServerName);
        perror("ERROR_FROM_EX2.\n");
        exit(1);
    }
    free(pidServerName);

    //End Validation

    int myPid = getpid();
    int temp1 = myPid;
    int size = 0;
    while (temp1 > 0)
    {
        size++;
        temp1 = temp1 / 10;
    }

    char *strMyPid = (char *)calloc(size + 1, sizeof(char));

    tostring(strMyPid, myPid);
    char *toSrvName = "to_srv";
    int flag;
    size_t buf = 1024;
    char *buffer;
    sleep(20);
    int num;
    for (int i = 0; i < 10; i++)
    {
        flag = isFileExist(toSrvName);

        if (flag == 0)
        {
            break;
        }
        else
        {
            srand(time(NULL));
            num = rand();
            num %= 6;
            ssize_t randomNum = getrandom(buffer, buf, GRND_RANDOM);
            sleep(num);
        }
        if (i == 9)
        {
            free(strMyPid);
            perror("ERROR FROM_EX2.\n");
            exit(1);
        }
    }
    printf("end of stage 5.e\n");

    int FDsrv = open("to_srv", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (FDsrv == -1)
    {
        free(strMyPid);
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }

    write(FDsrv, strMyPid, my_strlen(strMyPid));
    free(strMyPid);
    write(FDsrv, "\n", my_strlen("\n"));
    write(FDsrv, argv[2], my_strlen(argv[2]));
    write(FDsrv, "\n", my_strlen("\n"));
    write(FDsrv, argv[3], my_strlen(argv[3]));
    write(FDsrv, "\n", my_strlen("\n"));
    write(FDsrv, argv[4], my_strlen(argv[4]));
    // write(FDsrv, "50", my_strlen("50"));
    // write(FDsrv, "\n", my_strlen("\n"));
    // write(FDsrv, "2", my_strlen("2"));
    // write(FDsrv, "\n", my_strlen("\n"));
    // write(FDsrv, "20", my_strlen("20"));

    int serverPidInt = myAtoi(argv[1]);
    printf("end of stage 5.d\n");
    close(FDsrv);

    kill(serverPidInt, SIGUSR1);
    signal(SIGUSR2, afterGetResult);
    signal(SIGALRM, ifServerNotRespone);

    alarm(30);
    pause();
}

void ifServerNotRespone()
{
    printf("Client closed because no respone was received from the server for 30 seconds\n");
    alarm(0);
}

void afterGetResult()
{
    // int errorFD = open("errorFile.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    int mypid = getpid();

    int temp = mypid;
    int size = 0;
    while (temp > 0)
    {
        size++;
        temp = temp / 10;
    }

    char *mypidStr = (char *)calloc(size, sizeof(char));
    tostring(mypidStr, mypid);
    int sizePid = my_strlen(mypidStr);

    char *nameFileChild = (char *)calloc(10 + sizePid, sizeof(char));
    my_strcpy(nameFileChild, "to_client_");
    my_strcat(nameFileChild, mypidStr);
    free(mypidStr);

    int toClientChiledFD = open(nameFileChild, O_RDONLY, S_IRUSR | S_IWUSR);
    if (toClientChiledFD == -1)
    {
        free(nameFileChild);
        perror("ERROR FROM_EX2.\n");
        exit(1);
    }
    float resultToPrint;
    read(toClientChiledFD, &resultToPrint, sizeof(resultToPrint));

    // write(errorFD, result, 2);
    printf("the result is: %.3f\n", resultToPrint);
    close(toClientChiledFD);
    unlink(nameFileChild);
    free(nameFileChild);
    printf("end of stage 5.j\n");
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
int my_strCompare(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
int isFileExist(const char *filename)
{
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if (exist == 0)
        return 1;
    else
        return 0;
}
