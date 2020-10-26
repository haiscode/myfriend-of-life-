#include "gif.h"
void await_GIF_show(void)//待机进入等待
{
    for (int i = 1; i < 8; i++)
    {
        char pathename[100]={0};
        sprintf(pathename,"./gif/await/待机进入等待-%d.jpeg",i);
        lcd_draw_jpg(0,0,pathename,0);
        usleep(50000);  
    }
}
void wait_GIF_show(void) //等待回答
{

    for (int i = 1; i < 14; i++)
    {
        char pathename[100]={0};
        sprintf(pathename,"./gif/wait/01a7695af2b6a3a801207ab42d53d5-%d.jpeg",i);
        lcd_draw_jpg(0,0,pathename,0);
        usleep(50000);  
    }
}

void waitinput_GIF_show(void) //等待输入
{

    for (int i = 1; i < 14; i++)
    {
        char pathename[100]={0};
        sprintf(pathename,"./gif/waitinput/等待输入-%d.jpeg",i);
        lcd_draw_jpg(0,0,pathename,0);
        usleep(50000);  
    }

	
}

void GIF_show1(void) //唱歌
{

    for (int i = 1; i < 84; i++)
    {
        char pathename[100]={0};
        sprintf(pathename,"./gif/唱歌-%d.jpeg",i);
        lcd_draw_jpg(0,0,pathename,0);
        usleep(50000);  
    }
    
	
}

void GIF_show2(void) //正常显示2
{
    for (int i = 1; i < 33; i++)
    {
        char pathename[100]={0};
        sprintf(pathename,"./gif/myfriend%d.jpeg",i);
        lcd_draw_jpg(0,0,pathename,0);
        usleep(50000);  
    }
}