#include "main.h"

bool show_futuer(cJSON * obj)
{
    
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    
    bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,0,128,255));
    //获取未来5天的天气情况
    struct data_fun *inf_0 =  weather_show(obj,0);
    struct data_fun *inf_1 =  weather_show(obj,1);
    struct data_fun *inf_2 =  weather_show(obj,2);
    struct data_fun *inf_3 =  weather_show(obj,3);
    struct data_fun *inf_4 =  weather_show(obj,4);
    struct data_fun *inf_5 =  weather_show(obj,5);

    
    fontSetSize(f,30);
    //地点+更新时间
    fontPrint(f,bm,0,0,inf_0->first,getColor(0,255,255,255),0);
    //显示星期
    fontPrint(f,bm,25,50,inf_0->week,getColor(0,255,255,255),0);
    fontPrint(f,bm,150,50,inf_1->week,getColor(0,255,255,255),0);
    fontPrint(f,bm,280,50,inf_2->week,getColor(0,255,255,255),0);
    fontPrint(f,bm,420,50,inf_3->week,getColor(0,255,255,255),0);
    fontPrint(f,bm,555,50,inf_4->week,getColor(0,255,255,255),0);
    fontPrint(f,bm,690,50,inf_5->week,getColor(0,255,255,255),0);
    //显示月日
    fontPrint(f,bm,15,80,inf_0->md,getColor(0,255,255,255),0);
    fontPrint(f,bm,140,80,inf_1->md,getColor(0,255,255,255),0);
    fontPrint(f,bm,270,80,inf_2->md,getColor(0,255,255,255),0);
    fontPrint(f,bm,410,80,inf_3->md,getColor(0,255,255,255),0);
    fontPrint(f,bm,545,80,inf_4->md,getColor(0,255,255,255),0);
    fontPrint(f,bm,680,80,inf_5->md,getColor(0,255,255,255),0);

    

    //显示低高温
    fontPrint(f,bm,25,256,inf_0->wendu,getColor(0,255,255,255),0);
    fontPrint(f,bm,150,256,inf_1->wendu,getColor(0,255,255,255),0);
    fontPrint(f,bm,280,256,inf_2->wendu,getColor(0,255,255,255),0);
    fontPrint(f,bm,420,256,inf_3->wendu,getColor(0,255,255,255),0);
    fontPrint(f,bm,555,256,inf_4->wendu,getColor(0,255,255,255),0);
    fontPrint(f,bm,690,256,inf_5->wendu,getColor(0,255,255,255),0);
    //显示天气情况
    fontPrint(f,bm,50,306,inf_0->type,getColor(0,255,255,255),0);
    fontPrint(f,bm,175,306,inf_1->type,getColor(0,255,255,255),0);
    fontPrint(f,bm,305,306,inf_2->type,getColor(0,255,255,255),0);
    fontPrint(f,bm,445,306,inf_3->type,getColor(0,255,255,255),0);
    fontPrint(f,bm,580,306,inf_4->type,getColor(0,255,255,255),0);
    fontPrint(f,bm,715,306,inf_5->type,getColor(0,255,255,255),0);

    //显示风向风级
    
    fontPrint(f,bm,15,356,inf_0->fxfl,getColor(0,255,255,255),0);
    fontPrint(f,bm,140,356,inf_1->fxfl,getColor(0,255,255,255),0);
    fontPrint(f,bm,270,356,inf_2->fxfl,getColor(0,255,255,255),0);
    fontPrint(f,bm,410,356,inf_3->fxfl,getColor(0,255,255,255),0);
    fontPrint(f,bm,545,356,inf_4->fxfl,getColor(0,255,255,255),0);
    fontPrint(f,bm,680,356,inf_5->fxfl,getColor(0,255,255,255),0);

    //显示空气质量

    fontPrint(f,bm,50,406,inf_0->aqi,api_level(inf_0->aqilv),0);
    fontPrint(f,bm,175,406,inf_1->aqi,api_level(inf_1->aqilv),0);
    fontPrint(f,bm,305,406,inf_2->aqi,api_level(inf_2->aqilv),0);
    fontPrint(f,bm,445,406,inf_3->aqi,api_level(inf_3->aqilv),0);
    fontPrint(f,bm,580,406,inf_4->aqi,api_level(inf_4->aqilv),0);
    fontPrint(f,bm,715,406,inf_5->aqi,api_level(inf_5->aqilv),0);
    show_font_to_lcd(mmap_fd,0,0,bm);
    //显示图片
    show_typepic(inf_0->type,20,120);
    show_typepic(inf_1->type,145,120);
    show_typepic(inf_2->type,275,120);
    show_typepic(inf_3->type,415,120);
    show_typepic(inf_4->type,550,120);
    show_typepic(inf_5->type,685,120);
    fontUnload(f);
    destroyBitmap(bm);

    //显示延时20s
    sleep(20);
    return true;
}
int socket_get(char * service_ip)
{
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_socket == -1)
                {
                    perror("socket()");
                    return -1;
                }
    struct sockaddr_in service_addr;
    service_addr.sin_port = htons(80);
    service_addr.sin_family = AF_INET;
    service_addr.sin_addr.s_addr = inet_addr(service_ip);

    if(connect(tcp_socket,(struct sockaddr *)&service_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        return -1;
    }else
    {
        printf("connect success!!!\n");
        return tcp_socket;
    }   

} 


void *myfriend(void *arg)
{
    
    while (1)
    {
        GIF_show2(); //正常显示2
    }
    
}


int main(int argc, char const *argv[])
{
    lcd_open();

    //显示机器人表情
    pthread_t PID;
    pthread_create(&PID,NULL,myfriend,NULL);
    //设置离线模式
    pthread_detach(PID);
    //初始化对话框显示
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f,32);

    bitmap *bm = createBitmapWithInit(800,160,4,getColor(0,0,0,0));

    //添加到点阵图
    fontPrint(f,bm,0,0,"可以跟我聊天噢~( ゜- ゜)つロ!",getColor(0,0,255,0),0);
    show_font_to_lcd(mmap_fd,0,320,bm);
    fontUnload(f);
    destroyBitmap(bm);

    while (1)
    {
        int tcp_socket1 =  socket_get("47.107.155.132");
        if (tcp_socket1 == -1)
        {
            return -1;
        }
        char cmd[1024] = {0};
        int  flag_re = 0;
        printf("请输入内容\n");
        scanf("%s",cmd);

        if (strstr(cmd,"天气"))
        {
            pthread_cancel(PID);
            int tcp_socket2 =  socket_get("1.81.5.176");
            if (tcp_socket2 == -1)
            {
                return -1;
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
                    pthread_create(&PID,NULL,myfriend,NULL);
                    pthread_detach(PID);
                    continue;
                }
            }else
            {
                printf("获取信息成功\n");
            }
            //显示未来5天天气信息
            show_futuer(obj);
            close(tcp_socket2);
            //开启线程
            pthread_create(&PID,NULL,myfriend,NULL);
            pthread_detach(PID);
            continue;
        }

        if (strstr(cmd,"图片"))
        {
            pthread_cancel(PID);
            while (1)
            {
                if(get_jpg() == -1) //从网络随机抓取1张二次元图片下载到本地
                {
                    continue;
                }else
                {
                    lcd_draw_jpg(0,0,"./1.jpeg",1);//显示抓取到的图片
                    break;
                }
            }
            sleep(10);
            pthread_create(&PID,NULL,myfriend,NULL);
            pthread_detach(PID);
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