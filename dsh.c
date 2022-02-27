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

/**
 *motd prints the message of the day if there is one
 **/
void motd()
{
    FILE *motd = fopen(".dsh_motd", "r");
    int c;
    // fopen("file.txt", "r");
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
/**
 * runCmd executes the function
 * @param argv the array of the path and args
 * @param background whether it runs in foreground or background
 **/
void runCmd(char *const argv[], int background)
{
    pid_t child = fork();
    if (child == 0)
    {
        execv(argv[0], argv);
        printf("Improper Path\n");
        exit(-1);
    }
    else if (background == 0)
    {
        int status;
        waitpid(child, &status, 0);
    }
}
/**
 * has path tells whether it has the absolute path
 * @param str the cmd line
 * @return
 **/
int hasPath(char *str)
{
    if (str[0] == '/')
    {
        return 1;
    }
    return 0;
}
/**
 *parseCmd separates the cmdline into pieces in an array
 also sees if there is an & for background
 @param input the cmd line
 @param array the array of the cmdline pieces
 @return 1 for foreground 0 for background
 **/
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
/**
 *dshCd changes the directory
 *@param str the path to change to
 **/
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
