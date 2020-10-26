#ifndef __GET_TXT__
#define __GET_TXT__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "cJSON.h"
#include "lcd.h"
#include "font.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
typedef struct city
{
    char id[20];
    char name[50];

}city;

struct mp3_inf
{
    char name[50];
    char artistsname[50];
};


int socket_get(char * service_ip);
bool find_city_id(char *cmd,city *c);
void show_error(void);
cJSON * txt_get(char * cmd,int tcp_socket);
bool  chat_get(char * cmd,int tcp_socket);
int get_jpg(void);
struct mp3_inf* get_music(void);
#endif