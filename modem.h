#include "mircam_definitions.h"
#include "uart.h"

void append_char(char *temp_string, char temp_char);
int check_for_match(char *temp_string, const char *check_string);
int parse_error(const char* string_buffer);

int check_modem();
int check_network();
int setup_gprs();
int stop_gprs();
//int context_act();
//int set_socket_cfg(int socket_no, int no_bytes);
//int socket_dial(int socket_no, int port);
int socket_create(int socket_no, const char* port) ;
int socket_send(int socket_no, int no_bytes);
int socket_receive(int socket_no);
int socket_status(int socket_no);
int socket_shutdown(int socket_no);
