#include "modem.h"

void main(){

	check_modem();
	char teststring[10]="haha";

	check_network();
	stop_gprs();
	setup_gprs();
//	socket_create(1,"100");

	printf("main end reached\r\n");
//	printf("haha baudrate - %d\n",getbaud(fd));
}
