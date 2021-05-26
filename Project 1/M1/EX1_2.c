//Haim Hugi 207686650

// // a.out <--- config.txt
// // readdir(students)
// // for each students:
// // 	execl(gcc student_name.c -o main.out)
// // 	open(test_input.qa, O_RDONLY)
// // 	open(program_output.txt, O_WRONLY | O_CREAT | O_TRUNC )
// // 	dup2(test_input.qa , keyboard)
// // 	dup2(program_output.txt , screen)
// // 	exccl(main.out)
// // 	open(results.csv, O_WRONLY | O_CREAT | O_APPEND)
// // 	dup2(results.csv , screen)
// // 	execl(comp.out program_output.txt expected_output.qa)
// // dup2(screen, results.csv)
// // printout on screen results.csv

#include <stdio.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "string.h"
#include <sys/wait.h>

void my_strcat(char *destination, char *source);
void my_strcpy(char *destination, char *source);
int my_strlen(const char *str);

int main(int argc, char *argv[])
{
    //char *tempBdika = "/home/haim/Homework/M1/setup/config.txt";
    int configFileFD = open(argv[1], O_RDONLY);
    char *studentsPath = (char *)calloc(1, sizeof(char));
    int studentBuffSize = 0;
    char *testInputPath = (char *)calloc(1, sizeof(char));
    int testInputBuffSize = 1;
    testInputPath[testInputBuffSize - 1] = '\0';
    char *expectedOutputPath = (char *)calloc(1, sizeof(char));
    int expectedOutputBuffSize = 1;
    expectedOutputPath[expectedOutputBuffSize - 1] = '\0';

    char tav;
    int lineCount = 1;

    if (configFileFD == -1)
    {
        perror("Cant open  file");
        exit(1);
    }
    int readChar = 1;
    while (readChar > 0)
    {
        readChar = read(configFileFD, &tav, 1);
        if (readChar > 0)
        {
            if (tav == '\n')
            {
                lineCount++;
            }
            else
            {
                if (lineCount == 1)
                {
                    studentBuffSize++;
                    studentsPath = (char *)realloc(studentsPath, studentBuffSize);
                    studentsPath[studentBuffSize - 1] = tav;
                }
                else if (lineCount == 2)
                {
                    testInputBuffSize++;
                    testInputPath = (char *)realloc(testInputPath, testInputBuffSize);
                    testInputPath[testInputBuffSize - 2] = tav;
                    testInputPath[testInputBuffSize - 1] = '\0';
                }
                else if (lineCount == 3)
                {
                    expectedOutputBuffSize++;
                    expectedOutputPath = (char *)realloc(expectedOutputPath, expectedOutputBuffSize);
                    expectedOutputPath[expectedOutputBuffSize - 2] = tav;
                    expectedOutputPath[expectedOutputBuffSize - 1] = '\0';
                }
            }
        }
    }
    pid_t chiled;
    DIR *dir;
    struct dirent *myDirent;
    dir = opendir(studentsPath);

    char slash = '/';
    studentBuffSize++;
    studentsPath = (char *)realloc(studentsPath, studentBuffSize);
    studentsPath[studentBuffSize - 1] = slash;
    char *shirshor = (char *)calloc(200, sizeof(char));
    char *shirshor2 = (char *)calloc(200, sizeof(char));
    char *temp = (char *)calloc(200, sizeof(char));

    DIR *currentDir;
    int length, j;
    struct dirent *myCurrentDirent;

    while ((myDirent = readdir(dir)) != NULL)
    {
        if (myDirent->d_name[0] != '.')
        {
            my_strcpy(shirshor, studentsPath);
            my_strcat(shirshor, (char *)myDirent->d_name);
            my_strcpy(temp, shirshor);
            currentDir = opendir(shirshor);
            int flag = 0;
            while ((myCurrentDirent = readdir(currentDir)) != NULL)
            {

                if (myCurrentDirent->d_name[0] != '.')
                {

                    if (flag == 0)
                    {
                        my_strcpy(shirshor, temp);
                        my_strcat(shirshor, "/");
                        my_strcpy(shirshor2, shirshor);
                        my_strcat(shirshor, (char *)myCurrentDirent->d_name);

                        my_strcat(shirshor2, "main.out");

                        int parentPid = getpid();

                        int z;
                        if ((chiled = fork()) == 0)
                        {
                            execlp("gcc", "gcc", shirshor, "-o", shirshor2, NULL);
                            exit(-1);
                        }
                        else
                        {
                            wait(&z);
                        }
                        flag = 1;
                    }
                }
            }
        }
    }

    DIR *dir1 = opendir(studentsPath);
    int programOutputFD;
    int screenFD = dup(1);
    int testInputPathFD;
    int expectedOutputFD;
    int resultsFD;

    char *studentName = (char *)calloc(50, sizeof(char));

    while ((myDirent = readdir(dir1)) != NULL)
    {
        if (myDirent->d_name[0] != '.')
        {
            my_strcpy(shirshor, studentsPath);
            my_strcat(shirshor, (char *)myDirent->d_name);
            my_strcpy(temp, shirshor);
            my_strcpy(studentName, (char *)myDirent->d_name);
            currentDir = opendir(shirshor);

            while ((myCurrentDirent = readdir(currentDir)) != NULL)
            {
                int parentPid1 = getpid();
                if (myCurrentDirent->d_name[0] == 'm')
                {
                    my_strcpy(shirshor, temp);
                    my_strcat(shirshor, "/");
                    my_strcat(shirshor, (char *)myCurrentDirent->d_name);
                    int w;
                    testInputPathFD = open(testInputPath, O_RDONLY);
                    programOutputFD = open("program_output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                    if (testInputPathFD == -1)
                    {
                        perror("Cant open  file");
                        exit(1);
                    }
                    if (programOutputFD == -1)
                    {
                        perror("Cant open  file 2 ");
                        exit(1);
                    }
                    if ((chiled = fork()) == 0)
                    {
                        dup2(programOutputFD, 1);
                        if (dup2(testInputPathFD, 0) == -1)
                        {
                            printf("dup input failed\n");
                            exit(1);
                        }
                        execl(shirshor, shirshor, "-l", NULL);
                        exit(-1);
                    }
                    else
                    {
                        wait(&w);
                    }
                    dup2(0, testInputPathFD);
                    int p;
                    close(programOutputFD);
                    close(testInputPathFD);
                    resultsFD = open("results.csv", O_WRONLY | O_CREAT | O_APPEND);
                    dup2(resultsFD, 1);
                    int score;

                    if ((chiled = fork()) == 0)
                    {
                        execlp("./comp.out", "./comp.out", "program_output.txt", expectedOutputPath, NULL);
                        exit(-1);
                    }
                    else
                    {
                        wait(&p);
                    }
                    score = WEXITSTATUS(p);
                    write(resultsFD, studentName, my_strlen(studentName));
                    write(resultsFD, ",", 1);
                    char t = (score + '0');
                    write(resultsFD, &t, 1);
                    write(resultsFD, "\n", 1);
                }
            }
        }
    }

    int keyboardFD = dup(0);
    dup2(screenFD, 1);
    dup2(keyboardFD, 0);

    int resultfileFD = open("results.csv", O_RDONLY);
    char *resultPrint = (char *)calloc(1, sizeof(char));
    if (resultfileFD == -1)
    {
        perror("Cant open  file");
        exit(1);
    }

    readChar = 1;
    char tav2;
    int fileBuffSize = 0;
    while (readChar > 0)
    {
        readChar = read(resultfileFD, &tav2, 1);
        if (readChar > 0)
        {
            fileBuffSize++;
            resultPrint = (char *)realloc(resultPrint, fileBuffSize);
            resultPrint[fileBuffSize - 1] = tav2;
        }
    }
    write(1, resultPrint, fileBuffSize);
    free(resultPrint);
    close(resultfileFD);
    close(screenFD);
    close(keyboardFD);
    close(resultsFD);
    close(testInputPathFD);
    close(configFileFD);
    close(programOutputFD);
    closedir(dir);
    closedir(dir1);
    free(studentsPath);
    free(testInputPath);
    free(expectedOutputPath);
    free(studentName);
    free(shirshor);
    free(shirshor2);
    free(temp);
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

int my_strlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
        ;
    return (s - str);
}
