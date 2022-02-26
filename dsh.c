/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include "builtins.h"

// TODO: Your function definitions (declarations in dsh.h)
void motd()
{
    FILE *motd = fopen(".dsh_motd", "r");
    int c;
    fopen("file.txt", "r");
    while (1)
    {
        c = fgetc(motd);
        if (feof(motd))
        {
            break;
        }
        printf("%c", c);
    }
    printf("\n");
    fclose(motd);
}
void runCmd(char *const argv[], int background)
{
    pid_t child = fork();
    if (child == 0)
    {
        execv(argv[0], argv);
        printf("exec failed\n");
        exit(-1);
    }
    else if (background == 0)
    {
        int status;
        waitpid(child, &status, 0);
    }
}

int hasPath(char *str)
{
    if (str[0] == '/')
    {
        return 1;
    }
    return 0;
}

int parseCmd(char *input, char *array[])
{
    int background = 0;

    int lastChar = strlen(input) - 1;
    while (input[lastChar] == '\n')
    {
        if (input[lastChar] == '\n')
        {
            input[lastChar] = '\0';
            lastChar = strlen(input) - 1;
        }
        if (input[lastChar] == '&')
        {
            background = 1;
            input[lastChar] = '\0';
        }
    }
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL)
    {
        array[i++] = token;
        token = strtok(NULL, " ");
    }

    array[i] = NULL;
    return background;
}

/**
 * parsePath takes the command and appends
 * the correct path to it if there is one
 * @param the input command
 * @return the full path of for the cpmmand
 **/
char *parsePath(char *cmd)
{
    char *str = getenv("PATH");
    char tempStr[500];
    strcpy(tempStr, str);
    int i = 1;
    char *token = strtok(tempStr, ":");
    char *paths[100];
    char buf[1000];
    paths[0] = getcwd(buf, 1000);

    while (token != NULL)
    {
        paths[i++] = token;
        token = strtok(NULL, ":");
    }

    for (int j = 0; j < i; ++j)
    {
        char fullPath[1000]; // MAXPATH
        strcpy(fullPath, paths[j]);
        strcat(fullPath, "/");
        strcat(fullPath, cmd);
        if (access(fullPath, F_OK | X_OK) == 0)
        {

            return strdup(fullPath);
        }
    }

    return NULL;
}

void dshCd(char str[])
{
    char buf[1000];
    char *path = getcwd(buf, 1000);
    strcat(path, "/");
    strcat(path, str);
    if (chdir(path) != 0)
    {
        printf("No such file or directory\n");
    }
    printf("%s\n", path);
}
void makeHistory(char *history[], int numCmd, char cmdline[])
{
    if (numCmd < HISTORY_LEN)
    {
        char *temp;
        // strcpy(temp, cmdline);
        temp = cmdline;
        // printf("temp is %s\n", temp);

        history[numCmd] = temp;
        }
}