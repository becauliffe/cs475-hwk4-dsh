/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"
#include "builtins.h"
int absolute(char *path[]);
int main(int argc, char **argv)
{
	motd();
	char cmdline[MAXBUF]; // stores user input from commmand line
	// char *test[] = {"/bin/ls", "-a", NULL};
	// runCmd(test);
	// printf("%s\n", parsePath("ls"));
	// strcpy(cmdline, "ls -a");
	char *args[MAXBUF];
	int background;
	char buf[1000];
	while (1)
	{
		fgets(cmdline, MAXBUF, stdin);
		background = parseCmd(cmdline, args);
		switch (chkBuiltin(args[0]))
		{
		case CMD_EXIT:
			return 0;

		case CMD_HIST:
		// do hist thing
		case CMD_CD:
			dshCd(args[1]);
			break;
		case CMD_ECHO:
			break;
		case CMD_PWD:
			printf("%s\n", getcwd(buf, 1000));
			break;
		case CMD_EXT:
			// if(hasPath(args[0]))
			args[0] = parsePath(args[0]);
			if (args[0] != NULL)
			{
				runCmd(args, background);
			}
			else
			{
				printf("cmd not found\n");
			}
			break;
		}
	}
	return 0;
}

// int absolute(char *path[])
// {
// 	if (access(path[0], F_OK | X_OK) == 0)
// 	{

// 		runCmd(path, 0);
// 	}
// 	else
// 	{
// 		printf("failed\n");
// 	}
// 	return 0;
// }

// int helper()
// {
// 	return 0;
// getInput()
// if hasPath {
// checkPath()
// runcmd()
// helper
// return 0
// }
// else{
// strtok($PATH,':')
// }
// if (isBuiltin()) {helper	return 0}
//
//}