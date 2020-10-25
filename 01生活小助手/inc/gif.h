#ifndef __GIF_H__
#define __GIF_H__

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>
#include "lcd.h"
#include "font.h"

void await_GIF_show(void);//待机进入等待
void wait_GIF_show(void);//等待回答
void waitinput_GIF_show(void);//等待输入
void GIF_show1(void);//正常显示1
void GIF_show2(void);//正常显示2
#endif