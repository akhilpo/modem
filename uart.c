#include "uart.h"


int setup(void);                        //function to capture all the set up, once off tasks.
int writeport(int fd, char *chars);
int readport(int fd, char *result);
int getbaud(int fd);
int initport(int fd);
int uart_init(int fd);
void sendAT(char *data, int delay);


int     fd;
char    sBuffer[1024];
char    sResult[1024];

int setup()
{
    fd = open(gsm_tty, O_RDWR | O_NOCTTY | O_NDELAY);        //hard coded port - change the reference to "/dev/ttyUSB0" to suit your needs
    if (fd == -1) {
        printf("open_port: Unable to open ");
        printf("%s\r\n",gsm_tty);
        printf("  ");
        return 1;
    } else {
        fcntl(fd, F_SETFL, 0);
        printf("Serial connection established on ");
        printf("%s\r\n",gsm_tty);
    }
//    printf("baud=%d\n", getbaud(fd));
    initport(fd);
//    printf("baud=%d\n", getbaud(fd));
   
    return 0;        //zero means good
}



int initport(int fd) {
    struct termios options;
    // Get the current options for the port...
    tcgetattr(fd, &options);
    // Set the baud rates to whatever is needed... (Max 115200)
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    // Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag     &= ~OPOST;
	options.c_cc[VMIN]  = 0;
	options.c_cc[VTIME] = 10;

    // Set the new options for the port...
    tcsetattr(fd, TCSANOW, &options);
    return 1;
}


int writeport(int fd, char *chars) 
{
    int n = write(fd, chars, strlen(chars));
    if (n < 0) 
    {
        fputs("write failed!\r\n", stderr);
        return 1;
    }
    return 0;
}



int readport(int fd, char *result) 
{
    int n = read(fd, result, 254);
    result[n-1] = 0x00;       //This is needed or the previous contents of the string will appear after the changed characters.
    if (n < 0)
    {
        if (errno == EAGAIN) 
        {
            printf("SERIAL EAGAIN ERROR\r\n");
            return errno;
        } else 
        {
            printf("SERIAL read error %d %s\r\n", errno, strerror(errno));
            return 1;
        }
    }                    
    return 0; //Zero means good
}


int getbaud(int fd) 
{
    struct termios termAttr;
    int inputSpeed = -1;
    speed_t baudRate;
    tcgetattr(fd, &termAttr);
    /* Get the input speed.                              */
    baudRate = cfgetispeed(&termAttr);
    switch (baudRate) 
    {
        case B0:      inputSpeed = 0; break;
        case B50:     inputSpeed = 50; break;
        case B110:    inputSpeed = 110; break;
        case B134:    inputSpeed = 134; break;
        case B150:    inputSpeed = 150; break;
        case B200:    inputSpeed = 200; break;
        case B300:    inputSpeed = 300; break;
        case B600:    inputSpeed = 600; break;
        case B1200:   inputSpeed = 1200; break;
        case B1800:   inputSpeed = 1800; break;
        case B2400:   inputSpeed = 2400; break;
        case B4800:   inputSpeed = 4800; break;
        case B9600:   inputSpeed = 9600; break;
        case B19200:  inputSpeed = 19200; break;
        case B38400:  inputSpeed = 38400; break;
        case B115200: inputSpeed = 115200; break;
    }
    return inputSpeed;
}

int uart_init(int fd) {

	int i;
	for (i=0;i<20;i++) {
		if (0 == setup()) break;

	}
	if (i >= 20) {
		printf("\r\nUart Initialization failed\r\n");
		return -1;
	}
	initport(fd);
	i = getbaud(fd);
	printf("Baudrate is %d\r\n", i);

	return 0;
}

void sendAT(char *data, int delay) {		//Buffer size is 255. Appends '\r' in the end
	char buffer[255];
	strcpy(buffer,data);
	strcat(buffer,"\r");
//	printf("buffer = %s\r\n",buffer);
	tcflush(fd,TCIFLUSH); 		//flush Uart driver
//	readport(fd,sResult);
	writeport(fd, buffer);
//	usleep(delay*1000);
	readport(fd,sResult);

#ifdef DEBUG
	printf("Command sent: %s\r\n", buffer);
	printf("Reply from modem: %s\r\n", sResult);
#endif
	parse_error(sResult);
}
