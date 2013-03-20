
#define DEBUG

//###########Global Variables#############

char send_buffer[1024];
char read_buffer[1024];
char last_error[5];
int read_buffer_size;

//###########Network Settings#############

#define APN "airtelgprs.com"
#define SERVER_IP "119.82.100.154"

//###########Uart Settings################

#define gsm_tty "/dev/ttyUSB0"
#define baudrate B115200


//###########Error Codes##################

#define SUCCESS 0
#define ERROR 	-1


//################Macros###################

#define ascii2dec(x) (x-0x30)
#define dec2ascii(x) (x+0x30)
