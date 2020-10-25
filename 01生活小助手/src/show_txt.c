#include "font.h"


void txt_show(int *mmap,char *content,int x_font,int y_font,int x_lcd,int y_lcd,int size)
{
    font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f,size);
    //创建画板
    bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,255,255,255));
    //添加到点阵图
    fontPrint(f,bm,x_font,y_font,content,getColor(0,0,255,0),0);

    //输入到LCD
    show_font_to_lcd(mmap,x_lcd,y_lcd,bm);

    //关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}