#include "modem.h"

#define ETX 3
#define DLE 16


int check_modem(){

	int count = 0;
	uart_init(fd);
	char *stringlocator;

	sendAT("AT",1000);
	do {

		#ifdef DEBUG
			printf("\nCount = %d\r\n",count);
		#endif
		usleep(50*1000); // sleep 30 milliseconds
//		readport(fd,sResult);
		stringlocator = strstr(sResult,"OK");
		if (stringlocator != NULL)
			break;
		sendAT("AT",10);

	} while (++count < 20);

	if (count == 20){
		printf("Check_modem failed\r\n");
		return -1;
	}
	else
		printf("Check_modem success\r\n");
	return 0;
}

int check_network() {

	char *charloc;
	int temp_count = 0;
	sendAT("AT+CMEE=1",30);	//Enable error codes
//	sendAT("AT&W 0 E 0",30) //Disable echo
	sendAT("AT+CREG=0",20);	//Disable unsolicited network info


	do {
		#ifdef DEBUG
			printf("c_network count = %d\r\n",temp_count);
		#endif
		sendAT("AT+CREG?",30);
		charloc = strstr(sResult,"+CREG: "); //+CREG: 0,1
		if (*(charloc+9) == '1') break; //
	}	while (++temp_count<20);
	if (temp_count > 19) {
		printf("Network initialization failed\r\n");
		return -1;
	}
	printf("Network initialized\r\n");
	return 0;	//Zero means good
}

int setup_gprs() {

	sendAT("AT+WIPCFG=1",30);	//Start IP Stack
	sendAT("AT+WIPBR=1,6",30);		//Open GPRS Bearer

	char AT_APN[100] = "AT+WIPBR=2,6,11,\""; //Configure APN
	strcat(AT_APN,APN);
	strcat(AT_APN,"\"");
	sendAT(AT_APN,30);

	sendAT("AT+WIPBR=4,6,0",30);	//Start GPRS

	return 0;
}

int stop_gprs() {

	sendAT("AT+WIPCFG=0",30);
	if (check_for_match(sResult,"OK"))
		return 0;
	else
		return -1;

}

int socket_create(int socket_no, const char* port) {

	char temp_buffer[254] = "AT+WIPCREATE=2,";
	append_char(temp_buffer,dec2ascii(socket_no));
	strcat(temp_buffer,",\"");
	strcat(temp_buffer,SERVER_IP);
	strcat(temp_buffer,"\",");
	strcat(temp_buffer,port);
	sendAT(temp_buffer,150);

	int count = 0;
	char *stringlocator;
	do {
		#ifdef DEBUG
			printf("socket create count: %d and sResult: %s\r\n", count,sResult);
		#endif
//		stringlocator = strstr(sResult,"+WIPREADY: ");
//		if (stringlocator != NULL)
//			return 0;
		if (check_for_match(sResult,"+WIPREADY: "))
			return 0;
		read(fd,sResult,254);
//		usleep(50*1000); 		//sleep for 10 milliseconds
	} while (++count < 20);

#ifdef DEBUG
	printf("Socket creation failed\r\n");
#endif
	return -1;
}

int socket_send(int socket_no, int no_bytes) {

	char temp_buffer[2048];				//Parsing send_buffer to temp_buffer
	int send_buffer_count = 0;
	int temp_buffer_count = 0;
	while (send_buffer_count < no_bytes) {
		if (send_buffer[send_buffer_count] == ETX) {
			temp_buffer[temp_buffer_count++] = DLE;
			temp_buffer[temp_buffer_count++] = ETX;
		}
		else if (send_buffer[send_buffer_count] == DLE) {
			temp_buffer[temp_buffer_count++] = DLE;
			temp_buffer[temp_buffer_count++] = DLE;
		}
		else temp_buffer[temp_buffer_count++] = send_buffer[send_buffer_count];
		send_buffer_count++;
	}
	temp_buffer[temp_buffer_count++] = ETX; 	//Adding ETX to return to AT mode

	char AT_buffer[100] = "AT+WIPDATA=2,";	//Send AT+WIPDATA=2,soc_no,1 - Start data on specified socket
	append_char(AT_buffer,dec2ascii(socket_no));
	strcat(AT_buffer,",1");
	sendAT(AT_buffer,50);

	if (check_for_match(sResult,"CONNECT")) {
		write(fd,temp_buffer,temp_buffer_count);
		return 0;			//Writing data if CONNECT is received
	}
	else {
		usleep(50*1000);
		if (check_for_match(sResult,"CONNECT")) {
			writeport(fd,temp_buffer);
			return 0;
		}
		else {
			printf("Sending through socket failed\r\n");
			return -1;
		}
	}
}



int socket_receive(int socket_no) {


	return 0;
}

int socket_status(int socket_no) {

	return 0;
}

int socket_shutdown(int socket_no) {

	return 0;
}

void append_char(char *temp_string, char temp_char) {

	temp_string[strlen(temp_string)] = temp_char;
	temp_string[strlen(temp_string) + 1] = '\0';
}

int check_for_match(char *temp_string, const char *check_string) {

	char *stringlocation;
	stringlocation = strstr(temp_string,check_string);
	if (stringlocation == NULL)
		return 0;				//Zero means string not found
	else
		return 1;				//One means string found
}

int parse_error(const char* string_buffer) {

	char *stringlocation;
	stringlocation = strstr(string_buffer,"+CME ERROR: ");
	if (stringlocation == NULL) {
		strncpy(last_error,"",5);
		return 0;
	}
	else {
		strncpy(last_error,(stringlocation + 12),5);
		stringlocation = strstr(last_error,"\n");
		*stringlocation = '\0';
		#ifdef DEBUG
			printf("#####ERROR: %s#######\r\n",last_error);
		#endif
		return -1;
	}

}
