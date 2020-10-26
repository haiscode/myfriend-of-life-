#include "show_weather.h"


struct ymds  *get_ym(char *ymd) //获得年月日
{
    struct ymds *new = calloc(1,sizeof(struct ymds));
    char * ch ="-";
    char *p = strtok(ymd,ch);
    strcpy(new->year,p); 
    p = strtok(NULL,ch); 
    strcpy(new->month,p);
    p = strtok(NULL,ch); 
    strcpy(new->data,p);
    return new;
}


int api_level(int aqilv)
{
    switch (aqilv)
    {
    case 1:
        return getColor(0,0,255,0);
    case 2:
        return getColor(0,0,255,0);
    case 3:
        return getColor(0,255,255,0);
    case 4:
        return getColor(0,255,0,0);
    case 5:
        return getColor(0,255,0,0);
    case 6:
        return getColor(0,255,0,0);
    }
}


void show_typepic(char *type,int x,int y)
{
    if (strstr(type,"晴"))
    {
        lcd_draw_jpg(x,y,"./typepic/qing.jpeg",0); 
    }else if (strstr(type,"阵雨"))
    {
        lcd_draw_jpg(x,y,"./typepic/zhenyu.jpeg",0); 
    }else if (strstr(type,"暴雨"))
    {
        lcd_draw_jpg(x,y,"./typepic/baoyu.jpeg",0); 
    }else if (strstr(type,"阴"))
    {
        lcd_draw_jpg(x,y,"./typepic/yin.jpeg",0); 
    }else if (strstr(type,"多云"))
    {
        lcd_draw_jpg(x,y,"./typepic/duoyun.jpeg",0); 
    }else if (strstr(type,"阵雪"))
    {
        lcd_draw_jpg(x,y,"./typepic/zhenyue.jpeg",0); 
    }else if (strstr(type,"雨"))
    {
        lcd_draw_jpg(x,y,"./typepic/yu.jpeg",0); 
    }

}




struct data_fun * weather_show(cJSON * obj,int n)
{
    //today资料
    //城市名字
    cJSON * value_cityinf= cJSON_GetObjectItem(obj,"cityInfo");
    

    //进入data对象
    cJSON * value_data= cJSON_GetObjectItem(obj,"data");
    //进入forecast数组
    cJSON * forecast_arry = cJSON_GetObjectItem(value_data,"forecast");
    //获得数组大小
    int size = cJSON_GetArraySize(forecast_arry);
    
    
    struct data_fun * inf = malloc(sizeof(struct data_fun));
    struct ymds ymds;
    char cityname[20]={0};
    char time[20]={0};

    strcpy(cityname,cJSON_GetObjectItem(value_cityinf,"city")->valuestring);
    strcpy(time,cJSON_GetObjectItem(obj,"time")->valuestring);
    sprintf(inf->first,"%s  系统更新时间:%s",cityname,time);

    //获取今天和未来4天的数据
    cJSON  * new_obj =  cJSON_GetArrayItem(forecast_arry,n);
            
    char high[14] ={0};
    char low[14] ={0};
    char fx[9] ={0};
    char fl[6] ={0};
    int aqi =0;
    //获得年月日
    ymds = *(get_ym(cJSON_GetObjectItem(new_obj,"ymd")->valuestring));
    //获得星期
    strcpy(inf->week,cJSON_GetObjectItem(new_obj,"week")->valuestring);
    //高温
    strcpy(high,cJSON_GetObjectItem(new_obj,"high")->valuestring);
    //低温
    strcpy(low,cJSON_GetObjectItem(new_obj,"low")->valuestring);
    //风向
    strcpy(fx,cJSON_GetObjectItem(new_obj,"fx")->valuestring);
    //风级
    strcpy(fl,cJSON_GetObjectItem(new_obj,"fl")->valuestring);
    //天气情况
    strcpy(inf->type,cJSON_GetObjectItem(new_obj,"type")->valuestring);
    //注意
    strcpy(inf->notice,cJSON_GetObjectItem(new_obj,"notice")->valuestring);
    sprintf(inf->first,"%s (%s)",inf->first,inf->notice);
    
    aqi = cJSON_GetObjectItem(new_obj,"aqi")->valueint;
    //aqi空气质量指数 0-50 良好 51-100中等 101-150轻度污染 151-200中度污染 201-250重度污染 251-300严重污染
    if (aqi>0 && aqi<=50)
    {
        strcpy(inf->aqi,"优");
        inf->aqilv =1;
    }else if (aqi>50 && aqi<=100)
    {
        strcpy(inf->aqi,"良好");
        inf->aqilv =2;
    }else if (aqi>100 && aqi<=150)
    {
        strcpy(inf->aqi,"轻度污染");
        inf->aqilv =3;
    }else if (aqi>150 && aqi<=200)
    {
        strcpy(inf->aqi,"中度污染");
        inf->aqilv =4;
    }else if (aqi>200 && aqi<=250)
    {
        strcpy(inf->aqi,"重度污染");
        inf->aqilv =5;
    }else if (aqi>250 && aqi<=300)
    {
        strcpy(inf->aqi,"严重污染");
        inf->aqilv =6;
    }
    
 
    //月日
    sprintf(inf->md,"%s月%s日",ymds.month,ymds.data);

    char * ch =" ℃";
    char * ch1 =" ℃";
    char *high1 = strtok(high,ch);
    high1 = strtok(NULL,ch);
    char *low1 = strtok(low,ch1);
    low1 = strtok(NULL,ch1);
    //低温~高温
    sprintf(inf->wendu,"%s~%s℃",low1,high1);
    //风向+风级
    sprintf(inf->fxfl,"%s%s",fx,fl);

    
    return inf; 
}