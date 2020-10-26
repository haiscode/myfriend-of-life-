#include "get_txt.h"


int socket_get(char * service_ip)
{
    struct hostent * addr = gethostbyname(service_ip);
                        if (addr == NULL)
                        {
                            return -1;
                        }
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_socket == -1)
                {
                    perror("socket()");
                    return -1;
                }
    struct sockaddr_in service_addr;
    service_addr.sin_port = htons(80);
    service_addr.sin_family = AF_INET;
    service_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)addr->h_addr_list[0]));

    if(connect(tcp_socket,(struct sockaddr *)&service_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        return -1;
    }else
    {
        return tcp_socket;
    }
    
} 

bool find_city_id(char *cmd,city *c)//获取城市ID
{
    //city.txt存放城市ID
    FILE *fp=fopen("./city.txt","r");//打开文件
    if(fp==NULL)
    {
        printf("文件打开错误！\n");

        sleep(5);

        return false;
    }
    while(1)
    {
        bzero(c->name,50);
        bzero(c->id,20);
        
        
        fscanf(fp,"%s %s\n",c->id,c->name);  //格式化读取文件内容并存到对应的结构体变量中
        if(strstr(cmd,c->name))         //比较是否为要查询的城市
        {  
            fclose(fp);
            return true;
        }

        if(feof(fp))        //读取到末尾失败返回
        {
            fclose(fp); 
            return false;
        }
    }
}
void show_error(void) //错误显示
{
    
    //初始化对话框显示
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f,32);

    bitmap *bm = createBitmapWithInit(800,160,4,getColor(0,0,0,0));
    //添加到点阵图
    fontPrint(f,bm,0,0,"俺不懂你说啥  o(￣┰￣*)ゞ！！！",getColor(0,0,255,0),0);
    show_font_to_lcd(mmap_fd,0,320,bm);
    fontUnload(f);
    destroyBitmap(bm);
    sleep(1);
}

cJSON * txt_get(char * cmd,int tcp_socket) //weather API文本获得
{
    //获取城市ID
    city city_fuc;
    if(find_city_id(cmd,&city_fuc) == false)
    {
        show_error();
        return NULL;    
    }
    char http_head[1024] ={0};

    //发送请求
    sprintf(http_head,"GET /api/weather/city/%s HTTP/1.1\r\nHOST:t.weather.itboy.net\r\n\r\n",city_fuc.id);
    write(tcp_socket,http_head,strlen(http_head));

    char  head[1024*10] ={0};
    int file_size = 0,size_one = 0;
    
    //读取回应内容
    size_one = read(tcp_socket,head,sizeof(head));
    
    //偏移到数据区
    char *head_tail = strstr(head,"{\"message\":");
    //取得数据
    cJSON * obj = cJSON_Parse(head_tail);
    if(obj == NULL)
    {
        return NULL; //成功返回取得数据的JSON结构体目标 失败返回NULL
    }else
    {
        return obj;
    }
    
}

bool chat_get(char * cmd,int tcp_socket) //青云客文本获得
{

    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f,32);
    
    bitmap *bm = createBitmapWithInit(800,160,4,getColor(0,0,0,0));

    


    char http_head[1024] ={0};

    //发送请求
    sprintf(http_head,"GET /api.php?key=free&appid=0&msg=%s HTTP/1.1\r\nHOST:api.qingyunke.com\r\n\r\n",cmd);
    write(tcp_socket,http_head,strlen(http_head));

    char head[1024] ={0};
    
    read(tcp_socket,head,sizeof(head));
    

    if (strlen(head) == 0)
    {
        return false;
    }
    
    char *p = strstr(head,"\"result\":0,\"content\":\"");
    p += strlen("\"result\":0,\"content\":\"");
    char *ch = "{br}";
    char *p1 = strstr(p,"\"");

    int content_len = (int) (p1-p);
    char content[1024] ={0};
    char content_buf[1024] ={0};
    strncpy(content,p,content_len);
    

    char *p2 = strtok(content,ch);
    strcpy(content_buf,p2);
    
    fontPrint(f,bm,0,0,content_buf,getColor(0,0,255,0),0);
    int i =0;
    while (1)
    {
        
        p2 = strtok(NULL,ch);
        if (p2 == NULL)
        {
            
            show_font_to_lcd(mmap_fd,0,320,bm);
            fontUnload(f);
            destroyBitmap(bm);
            return true;
        } 
        i=i+30;
        bzero(content_buf,1024);
        strcpy(content_buf,p2);
        fontPrint(f,bm,0,i,content_buf,getColor(0,0,255,0),0);
        
    }

}




int get_jpg(void)
{
	//
    int tcp_socket_random = socket_get("www.dmoe.cc");
                if ( tcp_socket_random == -1)
                {
                    return -1;
                }

        char *http_head_random = "GET /random.php HTTP/1.1\r\nHOST:www.dmoe.cc\r\n\r\n";

        write(tcp_socket_random,http_head_random,strlen(http_head_random));

		//获取第一次回文 
        char  head_random[2048*100] ={0};
        read(tcp_socket_random,head_random,sizeof(head_random));
        
		//截取回文中Location：的图片网址内容
		char *p = strstr(head_random,"Location: ");
        char *p1 = strstr(head_random,".jpg");
        int txt_len = (int)(p1-p);
        char content[1024] = {0};
        strncpy(content,p,txt_len);
        
        char *ch ="/";
        char *p3 = strtok(content,ch);
        char new_http_head[1024] = {0};
        char yuming[50] ={0};
        char URL[100] ={0};
        
        p3 = strtok(NULL,ch);
        
        strcpy(yuming,p3);
        
        p3 = strtok(NULL,ch);
        
        sprintf(URL,"/%s",p3);
        p3 = strtok(NULL,ch);
        
        sprintf(URL,"%s/%s.jpg",URL,p3);
        
		//建立图片网址请求
        sprintf(new_http_head,"GET %s HTTP/1.1\r\nHOST:%s\r\n\r\n",URL,yuming);
        close(tcp_socket_random);

        //通过获得的JPEG域名获得IP  ----》随意选取备用IP地址

        int tcp_socket_jpg = socket_get(yuming);
                if ( tcp_socket_random == -1)
                {
                    return -1;
                }
														
	//发HTTP 请求协议  
	write(tcp_socket_jpg,new_http_head,strlen(new_http_head));
	
	
	//新建一个文件  


       int fd=open("inter_load.jpeg",O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("");
				return -1;
			}
	
	//读取头数据  
	char  head[2048]={0}; 
	int size1 = read(tcp_socket_jpg,head,2048);  
	//取出文件的大小 
	int file_size=0;
	//下载的大小 
	int load_size=0;
	
	char *p10 =  strstr(head,"Content-Length");  //查找关键字
	
	sscanf(p10,"Content-Length:%d\r\n",&file_size);
	
	
	
	//求出头数据的长度 
	p10 = strstr(head,"\r\n\r\n");   //回文末尾  
	p10 = p10+4; 
	
  int head_len = (int)(p10 - head);
      
	
	//写入去头的一次数据 
	  int len  =size1-head_len; 
	  write(fd,p10,len);
	  load_size =+  len;
	
	
	while(1)
	{
	//回收HTTP 服务器的消息
	char  buf[4096*100]={0};
	int size=read(tcp_socket_jpg,buf,4096);
		load_size += size;
		if(file_size == load_size)
		{
			write(fd,buf,size);
			close(tcp_socket_jpg);
			close(fd);
			return 0;
		}
	    //把数据写入到本地文件中 
		write(fd,buf,size);
		//printf("buf=%s\n",buf);
	}

}


struct mp3_inf* get_music(void)
{
	
    struct mp3_inf * music_inf = malloc(sizeof(struct mp3_inf));
                if(music_inf == NULL)
                {
                    perror("malloc()");
                    return NULL;
                }
    //第一次请求
	struct hostent * random_addr = gethostbyname("api.uomg.com");
                        if (random_addr == NULL)
                        {
                            return NULL;
                        }
    int tcp_socket_random = socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_socket_random == -1)
                {
                    perror("socket()");
                    return NULL;
                }
    struct sockaddr_in service_addr;
    service_addr.sin_port = htons(80);
    service_addr.sin_family = AF_INET;
    service_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)random_addr->h_addr_list[0]));

    if(connect(tcp_socket_random,(struct sockaddr *)&service_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        return NULL;
    }

    char *http_head_random = "GET /api/rand.music?sort=热歌榜&format=json HTTP/1.1\r\nHOST:api.uomg.com\r\n\r\n";

        write(tcp_socket_random,http_head_random,strlen(http_head_random));

        char  head_random[2048*100] ={0};
        read(tcp_socket_random,head_random,sizeof(head_random));
					if(strlen(head_random) == 0)
					{
						return NULL;
					}
        
		//读取歌曲信息
		char *json_inf = strstr(head_random,"{\"code\"");


		char *json_inf_tail = strstr(head_random,"}}");
		json_inf_tail +=2;
		int json_len = (int)(json_inf_tail-json_inf);

		char json_txt[1024] ={0};
		strncpy(json_txt,json_inf,json_len);
		
        //获取JSON数据
		cJSON * obj =  cJSON_Parse(json_txt);
			if(obj == NULL)
			{
				return NULL;
			}
        
        
        
		cJSON * mp3_inf_data = cJSON_GetObjectItem(obj,"data");
		cJSON * mp3_inf_name = cJSON_GetObjectItem(mp3_inf_data,"name");
		cJSON * mp3_inf_artistsname = cJSON_GetObjectItem(mp3_inf_data,"artistsname");
		cJSON * mp3_inf_id = cJSON_GetObjectItem(mp3_inf_data,"url");

        strcpy(music_inf->name,mp3_inf_name->valuestring);
        strcpy(music_inf->artistsname,mp3_inf_artistsname->valuestring);

        char *ch ="/";
        char *p3 = strtok(mp3_inf_id->valuestring,ch);
        char new_http_head_163[1024] = {0};
        char yuming[50] ={0};
        char URL[100] ={0};
		p3 = strtok(NULL,ch);
		strcpy(yuming,p3);
		/* printf("%s\n",yuming); */
		while (1)
		{
			p3 = strtok(NULL,ch);
				if(p3 ==NULL)
				{
					/* printf("%s\n",URL); */
					break;
				}
			sprintf(URL,"%s/%s",URL,p3);
		}

       
		//第二次请求
        sprintf(new_http_head_163,"GET %s HTTP/1.1\r\nHOST:%s\r\n\r\n",URL,yuming);
		/* printf("%s\n",new_http_head_163); */
			
        

        struct hostent * random_163 = gethostbyname("music.163.com");
                        if (random_163 == NULL)
                        {
                            return NULL;
                        }
    int tcp_socket_163 = socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_socket_163 == -1)
                {
                    perror("socket()");
                    return NULL;
                }
    struct sockaddr_in service_addr_163;
    service_addr_163.sin_port = htons(80);
    service_addr_163.sin_family = AF_INET;
    service_addr_163.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)random_163->h_addr_list[0]));

    if(connect(tcp_socket_163,(struct sockaddr *)&service_addr_163,sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        return NULL;
    }
        write(tcp_socket_163,new_http_head_163,strlen(new_http_head_163));

        char  head_163[2048*100] ={0};
        read(tcp_socket_163,head_163,sizeof(head_163));
				if(strlen(head_163) == 0)
					{
						return NULL;
					}
		

		char *p4 = strstr(head_163,"Location: ");
		p4 += 10; 
        char *p5 = strstr(head_163,".mp3");
        int txt_len_1 = (int)(p5-p4);
        char content_1[1024] = {0};
        strncpy(content_1,p4,txt_len_1);
		/* printf("%s\n",content_1); */


		char *ch1 ="/";
        char *p6 = strtok(content_1,ch1);
        char new_http_head_final[1024] = {0};
        char music_yuming[50] ={0};
        char URL_1[200] ={0};
		p6 = strtok(NULL,ch1);
		strcpy(music_yuming,p6);
		
		while (1)
		{
			p6 = strtok(NULL,ch);
				if(p6 ==NULL)
				{
					sprintf(URL_1,"%s.mp3",URL_1);
					
					break;
				}
			sprintf(URL_1,"%s/%s",URL_1,p6);
		}

		
		//第三次请求
        sprintf(new_http_head_final,"GET %s HTTP/1.1\r\nHOST:%s\r\n\r\n",URL_1,music_yuming);
		

		

		struct hostent * random_mp3 = gethostbyname("m10.music.126.net");
                        if (random_mp3 == NULL)
                        {
							perror("gethostbyname()");
                            return NULL;
                        }
    int tcp_socket_mp3= socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_socket_mp3 == -1)
                {
                    perror("socket()");
                    return NULL;
                }
    struct sockaddr_in service_addr_mp3;
    service_addr_mp3.sin_port = htons(80);
    service_addr_mp3.sin_family = AF_INET;
    service_addr_mp3.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)random_mp3->h_addr_list[0]));

    if(connect(tcp_socket_mp3,(struct sockaddr *)&service_addr_mp3,sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        return NULL;
    }

    write(tcp_socket_mp3,new_http_head_final,strlen(new_http_head_final));

    char  head_mp3[4096] ={0};
    int size1 = read(tcp_socket_mp3,head_mp3,sizeof(head_mp3));
                if(strlen(head_mp3) == 0)
                {
                    return NULL;
                }
    

    char joint_name[50] ={0};

    sprintf(joint_name,"./loadmusic/%s.mp3",music_inf->name);
    

    int fd=open(joint_name,O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("open()");
				return NULL;
			}
	
	//读取头数据  
	
	
	//取出文件的大小 
	int mp3_size=0;
	//下载的大小 
	int load_size=0;
	
	char *p7 =  strstr(head_mp3,"Content-Length");  //查找关键字
	
	sscanf(p7,"Content-Length:%d\r\n",&mp3_size);
	
	

	
	//求出头数据的长度 
	p7 = strstr(head_mp3,"\r\n\r\n");   //回文末尾  
	p7 = p7+4; 
	
  int mp3head_len = (int)(p7 - head_mp3);
      
	
	//写入去头的一次数据 
	  int len  =size1-mp3head_len; 
	  write(fd,p7,len);
	  load_size =+ len;
	
	
	while(1)
	{
	//回收HTTP 服务器的消息
        char  mp3_buf[204800*10]={0};
        int size = read(tcp_socket_mp3,mp3_buf,10240*10);
        load_size += size;
        
        if(mp3_size == load_size)
        {
            
            write(fd,mp3_buf,size);
            close(tcp_socket_random);
            close(tcp_socket_mp3);
            close(tcp_socket_163);
            close(fd);
            return music_inf;
        }
        //把数据写入到本地文件中 
        write(fd,mp3_buf,size);
		//printf("buf=%s\n",buf);
	}

}