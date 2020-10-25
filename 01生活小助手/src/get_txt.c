#include "get_txt.h"

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
    
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,0,128,255));
    fontSetSize(f,72);
    fontPrint(f,bm,200,200,"请重新输入",getColor(0,255,255,255),0);
    show_font_to_lcd(mmap_fd,0,0,bm);
    fontUnload(f);
    destroyBitmap(bm);
    sleep(2);
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
	int tcp_socket_random = socket(AF_INET, SOCK_STREAM, 0);
		

	//建立跳转网址地址结构体
    struct sockaddr_in service_addr_random;
    service_addr_random.sin_port = htons(80);
    service_addr_random.sin_family = AF_INET;
    service_addr_random.sin_addr.s_addr = inet_addr("185.194.236.124");

	//与跳转网址建立连接
    int ret = connect(tcp_socket_random, (struct sockaddr *)&service_addr_random,sizeof(service_addr_random));
        if (ret == -1 )
        {
            printf("connect lost!!!\n");
            return -1;
        }else
        {
            printf("connect success!!!\n");
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



   //1.创建TCP socket  
       
	int tcp_socket_jpg = socket(AF_INET, SOCK_STREAM, 0);

  // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addr.sin_addr.s_addr = inet_addr("183.60.138.230"); //服务器的IP地址
											//图片的地址 

  // 3.链接 
  int ret1 = connect(tcp_socket_jpg,(struct sockaddr *)&server_addr,sizeof(server_addr));
	  if(ret1 < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }
														
	//发HTTP 请求协议  
	write(tcp_socket_jpg,new_http_head,strlen(new_http_head));
	
	
	//新建一个文件  


       int fd=open("1.jpeg",O_RDWR|O_CREAT|O_TRUNC,0777);
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


