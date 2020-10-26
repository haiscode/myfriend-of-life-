#include "main.h"

bool show_futuerweather(cJSON * obj)
{
    
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    
    bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,0,128,255));
    struct data_fun weather_inf_display[6];
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        weather_inf_display[i] = *(weather_show(obj,i));
    }

    fontSetSize(f,30);
    //城市名称+更新时间+小贴士
    fontPrint(f,bm,0,0,weather_inf_display[0].first,getColor(0,255,255,255),0);
    
    for (i = 0; i < 6; i++)
    {
    //显示星期
        
        fontPrint(f,bm,35+i*130,50,weather_inf_display[i].week,getColor(0,255,255,255),0);
    
    //显示月日
    
        fontPrint(f,bm,20+i*130,80,weather_inf_display[i].md,getColor(0,255,255,255),0);
    
    //显示低高温
    
        fontPrint(f,bm,10+i*135,256,weather_inf_display[i].wendu,getColor(0,255,255,255),0);
    
    //显示天气情况
    
        fontPrint(f,bm,40+i*135,306,weather_inf_display[i].type,getColor(0,255,255,255),0);
    
    //显示风向风级
    
        fontPrint(f,bm,15+i*135,356,weather_inf_display[i].fxfl,getColor(0,255,255,255),0);
    

    //显示空气质量
    
        fontPrint(f,bm,40+135*i,406,weather_inf_display[i].aqi,api_level(weather_inf_display[i].aqilv),0);
    }
    show_font_to_lcd(mmap_fd,0,0,bm);
    //显示图片
    for (i = 0; i < 6; i++)
    {
        show_typepic(weather_inf_display[i].type,20+i*130,120);
    }
    
    fontUnload(f);
    destroyBitmap(bm);
    //显示延时20s
    sleep(20);
    return true;
}
#if 0
void say(int arg)//歇后语
{
    int say_socket;
    while (1)
    {
        say_socket = socket_get("v1.alapi.cn");
                if (say_socket == -1)
                {
                    continue;
                }else
                {
                    break;
                }          
    }

    char *http_head = "GET /api/xhy HTTP/1.1\r\nHOST:v1.alapi.cn\r\n\r\n";

        write(say_socket,http_head,strlen(http_head));

        char  head[2048*100] ={0};
        read(say_socket,head,sizeof(head));
    
    
            

}
#endif

void chat_init(void)
{
    /* signal(14,say); */

    //初始化对话框显示
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f,32);

    bitmap *bm = createBitmapWithInit(800,160,4,getColor(0,0,0,0));

    //添加到点阵图
    fontPrint(f,bm,0,0,"可以跟我聊天噢 ~( ゜- ゜)つロ!",getColor(0,0,255,0),0);
    show_font_to_lcd(mmap_fd,0,320,bm);
    fontUnload(f);
    destroyBitmap(bm);



}
void *myfriend(void *arg)
{
    int flag = *((int *)arg);


    switch (flag)
    {
        case 1:
            while (1)
            {
                GIF_show2(); //正常显示2
            }
            break;
        case 2:
            while (1)
            {
                GIF_show1();//唱歌
            } 
        default:
            break;
    }
    
    
}


int main(int argc, char const *argv[])
{
    lcd_open();

    //显示机器人表情
    int set = 1;
    pthread_t PID;
    pthread_create(&PID,NULL,myfriend,&set);
    //设置离线模式
    pthread_detach(PID);
    
    chat_init();
    while (1)
    {
        int tcp_socket1 =  socket_get("api.qingyunke.com");
                    if (tcp_socket1 == -1)
                    {
                        continue;
                    }
        char cmd[1024] = {0};
        int  flag_re = 0;
        printf("请输入内容\n");
        scanf("%s",cmd);
        while ('\n' != getchar());

        if(strstr(cmd,"歌") || strstr(cmd,"网易云") || strstr(cmd,"音乐"))
        {
            pthread_cancel(PID);
            set = 2;
            pthread_create(&PID,NULL,myfriend,&set);
            pthread_detach(PID);
            struct mp3_inf * new_mp3 = NULL;
            while (1)
            {
                new_mp3 = get_music();
                    if(new_mp3 == NULL)
                    {
                        continue;
                    }else
                    {
                        break;
                    }
            }
            char play_music_cmd[200] ={0};
            sprintf(play_music_cmd,"mplayer -slave  -quiet ./loadmusic/%s.mp3 &",new_mp3->name);
            
            system(play_music_cmd);
            pthread_cancel(PID);
            set = 1;
            pthread_create(&PID,NULL,myfriend,&set);
            pthread_detach(PID);
                
            continue;
        }
        
        if (strstr(cmd,"天气"))
        {
            pthread_cancel(PID);
            int tcp_socket2;
            while (1)
            {
                tcp_socket2 =  socket_get("t.weather.itboy.net");
                    if (tcp_socket2 == -1)
                    {
                        continue;
                    }else
                    {
                        break;//成功连接
                    }    
            }
            cJSON * obj= txt_get(cmd,tcp_socket2);
            if (obj == NULL)
            {
                for (int  re= 0; re < 3; re++)
                {
                    obj = txt_get(cmd,tcp_socket2);
                    if (obj != NULL)
                    {
                        flag_re == 1;
                        break;
                    } 
                }
                if (flag_re == 1)
                {
                    flag_re = 0;
                    printf("获取信息成功\n");
                }else
                {
                    show_error();//显示
                    close(tcp_socket2);
                    pthread_create(&PID,NULL,myfriend,&set);
                    pthread_detach(PID);
                    chat_init();
                    continue;
                }
            }else
            {
                printf("获取信息成功\n");
            }
            //显示未来5天天气信息
            show_futuerweather(obj);
            close(tcp_socket2);
            //开启线程
            pthread_create(&PID,NULL,myfriend,&set);
            pthread_detach(PID);
            chat_init();
            continue;
        }

        if (strstr(cmd,"图片") || strstr(cmd,"二次元") || strstr(cmd,"周浩"))
        {
            pthread_cancel(PID);
            while (1)
            {
                if(get_jpg() == -1) //从网络随机抓取1张二次元图片下载到本地
                {
                    continue;
                }else
                {
                    lcd_draw_jpg(0,0,"./inter_load.jpeg",1);//显示抓取到的图片
                    break;
                }
            }
            sleep(10);
            pthread_create(&PID,NULL,myfriend,&set);
            pthread_detach(PID);
            chat_init();
            continue;
        }
        

        if(chat_get(cmd,tcp_socket1) == false)
        {
            close(tcp_socket1);
            printf("connect again\n");
            continue;
        }
        
        close(tcp_socket1);
        
    }

    lcd_close();

    return 0;
}