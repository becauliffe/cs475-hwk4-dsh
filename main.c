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
	char *history[HISTORY_LEN];
	motd();
	char cmdline[MAXBUF]; // stores user input from commmand line
	char *args[MAXBUF];
	int background;
	char buf[1000];
	int numCmd = 0;

	while (1)
	{
		printf("dsh> ");
		fgets(cmdline, MAXBUF, stdin);
		history[numCmd++] = strdup(cmdline);
		background = parseCmd(cmdline, args);
		if (hasPath(args[0]))
		{
			runCmd(args, background);
		}
		else
		{
			switch (chkBuiltin(args[0]))
			{
			case CMD_EXIT:
				return 0;

			case CMD_HIST:
				// printf("2 %s\n", history[0]);
				for (int i = 0; i < numCmd; i++)
				{
					printf("%d %s\n", i + 1, history[i]);
				}
				break;
			case CMD_CD:
				if (args[1] != NULL)
				{
					dshCd(args[1]);
				}
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
	}
	return 0;
}