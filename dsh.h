/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256
#define HISTORY_LEN 100

// TODO: Your function prototypes below
void runCmd(char *const argv[], int background);
char *parsePath(char *cmd);
void motd();
int parseCmd(char *input, char *array[]);
void dshCd(char str[]);
void makeHistory(char *history[], int numCmd, char cmdline[]);