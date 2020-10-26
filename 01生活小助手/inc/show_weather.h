#ifndef __SHOW_WEATHER__
#define __SHOW_WEATHER__

#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "font.h"
#include "lcd.h"

struct ymds
{
    char year[50];
    char month[50];
    char data[50];
};
struct data_fun
{
    char first[100];
    char md[19];
    char week[10];
    char wendu[22];
    char aqi[13];
    int aqilv;
    char fxfl[17];
  
    char type[13];
    char notice[50];
};





void show_typepic(char *type,int x,int y);
struct ymds  *get_ym(char *ymd);//获取年月日
struct data_fun * weather_show(cJSON * obj,int n);

int api_level(int aqilv);
#endif