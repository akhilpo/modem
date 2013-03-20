#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include "mircam_definitions.h"

int setup(void);                        //function to capture all the set up, once off tasks.
int writeport(int fd, char *chars);
int readport(int fd, char *result);
int getbaud(int fd);
int initport(int fd) ;
int uart_init(int fd);
void sendAT(char *data, int delay);

extern int fd;
extern char sBuffer[1024];
extern char sResult[1024];



