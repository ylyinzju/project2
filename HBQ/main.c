/**********************************************/
/*               黑白棋源代码                 */
/*              TC2.0下运行通过               */
/*         采用深度搜索边角权值算法           */
/**********************************************/
# include <stdio.h>
# include <dos.h>
# include <conio.h>
# include <graphics.h>
# include <stdlib.h>
# include <time.h>

# define BGCOLOR		  1    /* 背景颜色*/
# define COLOR_WHITECHESS 0    /*白棋棋子的颜色*/
# define COLOR_BLACKCHESS 15   /*黑棋棋子的颜色*/
# define COLOR_LINE 	  15   /*棋盘线的颜色*/
# define MINBOXSIZE       30   /*棋盘每一格边长*/
# define MAXSIZE          300  /*棋盘的大小 （1+8+1）*25 */
# define SCCOL            50   /*棋盘左上角的坐标*/
# define SCROW            60

# define VK_LEFT   0x4b00		/* 按键码*/
# define VK_RIGHT  0x4d00
# define VK_DOWN   0x5000
# define VK_UP     0x4800
# define VK_SPACE  0x3920
# define VK_ESC    0x011b

# define MAX_TREE_SIZE 15
# define INF		   1000

int TREE_LEVEL=7;
int empty_num=0;
int num_user=2;              /*得分统计*/
int num_computer=2;
int gbwzc=5,gbwzr=5;         /*光标的初始位置*/
int present_p_v[10][10];     /*当前格的位置及其权值*/
char again='0';              /*用来判断游戏是否需要重新开始*/

int l=0,k=0;

/*初始棋盘布局*/
int gril[10][10]=
{{0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,2,0,0,0,0},
    {0,0,0,0,2,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};
/*初始棋盘权值*/
int gril_value[10][10]=
{{0,0,0,0,0,0,0,0,0,0},
    {0,76,-49,15,9,9,15,-49,76,0},
    {0,-49,-49,4,4,4,4,-49,-49,0},
    {0,15,4,8,4,4,8,4,15,0},
    {0,9,4,4,4,4,4,4,9,0},
    {0,9,4,4,4,4,4,4,9,0},
    {0,15,4,8,4,4,8,4,15,0},
    {0,-49,-49,4,4,4,4,-49,-49,0},
    {0,76,-49,15,9,9,15,-49,76,0},
    {0,0,0,0,0,0,0,0,0,0},
};
/*************************************************************/
/****************用到的汉字***********************************/
char zuo16S[]={
    /* 以下是 '左' 的 16点阵宋体 字模，32 byte */
    0x02,0x00,0x03,0x00,0x02,0x00,0xFF,0xFE,
    0x02,0x00,0x02,0x00,0x04,0x00,0x04,0x00,
    0x0F,0xF8,0x08,0x80,0x10,0x80,0x10,0x80,
    0x20,0x80,0x40,0x80,0xBF,0xFE,0x00,0x00,
};

char you16S[]={
    /* 以下是 '右' 的 16点阵宋体 字模，32 byte */
    0x02,0x00,0x02,0x00,0x02,0x00,0xFF,0xFE,
    0x04,0x00,0x04,0x00,0x08,0x00,0x18,0x00,
    0x2F,0xF8,0x48,0x08,0x88,0x08,0x08,0x08,
    0x08,0x08,0x0F,0xF8,0x08,0x08,0x00,0x00,
};

char shang16S[]={
    /* 以下是 '上' 的 16点阵宋体 字模，32 byte */
    0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x00,0x01,0xF8,0x01,0x00,
    0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x04,0x7F,0xFE,0x00,0x00,
};

char xia16S[]={
    /* 以下是 '下' 的 16点阵宋体 字模，32 byte */
    0x00,0x04,0x7F,0xFE,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0xC0,0x01,0x60,0x01,0x30,
    0x01,0x20,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x00,0x01,0x00,0x00,0x00,
};

char yi16S[]={
    /* 以下是 '移' 的 16点阵宋体 字模，32 byte */
    0x0C,0x40,0x70,0x7C,0x10,0x84,0x15,0x08,
    0xFE,0x50,0x30,0x20,0x30,0xD0,0x5B,0x20,
    0x54,0x7E,0x90,0x84,0x11,0x88,0x12,0x50,
    0x10,0x20,0x10,0x40,0x11,0x80,0x16,0x00,
};

char luo16S[]={
    /* 以下是 '落' 的 16点阵宋体 字模，32 byte */
    0x04,0x40,0x04,0x40,0xFF,0xFE,0x04,0x40,
    0x45,0x00,0x21,0xF8,0xA1,0x10,0x4A,0xA0,
    0x48,0x40,0x10,0xA0,0x13,0x18,0xEF,0xF6,
    0x22,0x10,0x22,0x10,0x23,0xF0,0x22,0x10,
};

char zi16S[]={
    /* 以下是 '子' 的 16点阵宋体 字模，32 byte */
    0x00,0x00,0x3F,0xF0,0x00,0x20,0x00,0x40,
    0x00,0x80,0x01,0x00,0x01,0x00,0x01,0x04,
    0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00,
};

char tui16S[]={
    /* 以下是 '退' 的 16点阵宋体 字模，32 byte */
    0x40,0x00,0x27,0xF0,0x24,0x10,0x07,0xF0,
    0x04,0x10,0x04,0x10,0xE7,0xF0,0x24,0x88,
    0x24,0x50,0x24,0x20,0x25,0x10,0x26,0x18,
    0x24,0x08,0x50,0x00,0x8F,0xFE,0x00,0x00,
};

char chu16S[]={
    /* 以下是 '出' 的 16点阵宋体 字模，32 byte */
    0x01,0x00,0x01,0x00,0x21,0x04,0x21,0x04,
    0x21,0x04,0x21,0x04,0x3F,0xFC,0x21,0x04,
    0x01,0x00,0x21,0x04,0x21,0x04,0x21,0x04,
    0x21,0x04,0x3F,0xFC,0x20,0x04,0x00,0x00,
};

void drawmat(char *mat,int matsize,int x,int y,int color)
/*依次：字模指针、点阵大小、起始坐标(x,y)、颜色*/
{
    int i, j, k, n;
    n = (matsize - 1) / 8 + 1;
    for(j = 0; j < matsize; j++)
        for(i = 0; i < n; i++)
            for(k = 0;k < 8; k++)
                if(mat[j * n + i] & (0x80 >> k))  /*测试为1的位则显示*/
                    putpixel(x + i * 8 + k, y + j, color);
}
/*************************************************************/


/************************************************************/
/*功能：初始化图形模式                                      */
/*调用TC中initgraph(&gdrive,&gmode,"E:\\Turboc\\")函数      */
/************************************************************/
void init(int gdrive,int gmode)
{
    initgraph(&gdrive,&gmode,"");
}
/*************************************************************/
/*功能：在(a,b)处用color色显示大小为font的字符串txt          */
/*************************************************************/
void txt(int a,int b,char *txt,int font,int color)
{
    setcolor(color);
    settextstyle(0,0,font);/*设置图形模式下字符显示底字体、方向和大小*/
    outtextxy(a,b,txt);    /*图形模式下在x,y坐标位置显示一行字符串*/
}
/*************************************************************/
/*功能：在图形模式下的清屏，将屏幕用黑色填充                 */
/*************************************************************/
void cls()
{
    setfillstyle(SOLID_FILL,0);    /*用黑色进行实填充*/
    setcolor(0);                   /*设置前景颜色    */
    bar(0,0,640,480);              /*0,0为屏幕左上角，屏幕宽为640,长为480*/
}
/*************************************************************/
/*功能：在图形模式下的高级清屏                               */
/*它能填充指定颜色color的指定大小的矩形框从(a,b)到(c,d)      */
/*************************************************************/
void clscr(int a,int b,int c,int d,int color)
{
    setfillstyle(SOLID_FILL,color);
    setcolor(color);
    bar(a,b,c,d);
}
/*************************************************************/
/*功能：最小棋子的绘制                                       */
/*所在方格的所在格的坐标为(fc,fr)  (1,1)为可落子的最左上格   */
/*棋子的半径为radius                                         */
/*棋子的颜色为color                                          */
/*棋子中心坐标为(a,b)                                        */
/*************************************************************/
void minbox(int fc,int fr,int color)
{
    int a=0,b=0;
    int radius;
    int color1=color;
    a=(fc*MINBOXSIZE+SCCOL)+MINBOXSIZE/2;
    b=(fr*MINBOXSIZE+SCROW)+MINBOXSIZE/2;
    radius=MINBOXSIZE/2-5;
    
    setcolor(color1);
    setfillstyle(1,color1);
    pieslice(a, b,0,360,radius);
}
/*************************************************************/
/*功能：绘制棋盘的可见的最外框                               */
/*非可见外框的最左上坐标为(ac,ar)                            */
/* 每一格棋的宽度为aw                                        */
/*************************************************************/
void outerdraw(int ac,int br,int aw,int color)
{
    int a,b;
    a=ac+aw;
    b=br+aw;
    
    setcolor(color);
    rectangle(a,b,a+8*MINBOXSIZE, b+8*MINBOXSIZE);
    rectangle(a-3,b-3,a+8*MINBOXSIZE+3, b+8*MINBOXSIZE+3);
    rectangle(a-4,b-4,a+8*MINBOXSIZE+4, b+8*MINBOXSIZE+4);
    rectangle(a-5,b-5,a+8*MINBOXSIZE+5, b+8*MINBOXSIZE+5);
}
/*************************************************************/
/*功能：绘制棋盘中间的纵横线                                 */
/*非可见外框的最左上坐标为(cc,cr)                            */
/*每一格棋的宽度为cw                                         */
/*线的颜色为color                                            */
/*先画七根纵线,后画七根横线                                  */
/*************************************************************/
void linedraw(int cc,int cr,int cw,int color)
{
    int m=0;
    int x1,y1,x2,y2;
    
    for(m=1;m<8;m++)
    {
        x1=(cc+cw)+m*cw;
        y1=(cr+cw);
        x2=x1;
        y2=y1+8*cw;
        
        setcolor(color);
        line(x1,y1,x2,y2);
    }
    for(m=1;m<8;m++)
    {
        x1=(cc+cw);
        y1=(cr+cw)+m*cw;
        x2=x1+8*cw;
        y2=y1;
        
        setcolor(color);
        line(x1,y1,x2,y2);
    }
}
/*************************************************************/
/*功能：整个屏幕外框的绘制                                   */
/*从(a,b)到(c,d)                                             */
/*背景色彩为bgcolor                                          */
/**text指向文字内容，字体为font,色彩为textcolor              */
/*************************************************************/
void win(int a,int b,int c,int d,int bgcolor,char *text,int font,int textcolor)
{
    clscr(a,b,c,d,7);
    clscr(a+3,b+24,c-3,d-3,bgcolor);
    clscr(a+3,b+3,c-3,b+20,9);
    
    setcolor(15);
    line(a,b,c,b);
    line(a,b,a,d);
    line(a+2,b+21,c-2,b+21);
    line(c-2,b+2,c-2,b+21);
    line(a+2,d-2,c-2,d-2);
    line(c-2,b+23,c-2,d-2);
    txt(a+5,b+8,text,font,textcolor);/*调用文字显示函数*/
    
    setcolor(0);
    line(a,d,c,d);
    line(c,b,c,d);
    line(a+3,b+2,c-3,b+2);
    line(a+2,b+2,a+2,b+20);
    line(a+2,b+24,a+2,d-2);
    line(a+2,b+23,c-3,b+23);
}
/*************************************************************/
/*功能：绘出所在格的提示光标                                 */
/*(gc,gr)表示当前光标的所在格,(1,1)为的最左上格              */
/*************************************************************/
void gbdw(int gc,int gr,int color)
{
    int x1,y1;
    x1=(SCCOL+MINBOXSIZE)+(gc-1)*MINBOXSIZE;
    y1=(SCROW+MINBOXSIZE)+(gr-1)*MINBOXSIZE;
    
    setcolor(color);
    line(x1+9,y1+9,x1+MINBOXSIZE-9,y1+MINBOXSIZE-9);
    line(x1+9,y1+MINBOXSIZE-9,x1+MINBOXSIZE-9,y1+9);
}
/*************************************************************/
/*功能：判断当前是否格可以落子                               */
/*(xc,yr)表示当前欲落子的所在格的坐标,(1,1)为可落子的最左上格*/
/*测试当前方块是否可以放置棋子,可放的话得到present_p_v的值   */
/*val表示欲落子的值,白为1,黑为2                         	 */
/*返回值为int 0或1,0代表不可继续下落                         */
/*判断依据:向八个方向依次推进,判断每一个方向的情况           */
/*************************************************************/
int putdown(int xc,int yr,int val,int map_get[10][10])
{
    int a=1,b=1,c=1,d=1,e=1,f=1,g=1,h=1;
    int a_val=0,b_val=0,c_val=0,d_val=0,e_val=0,f_val=0,g_val=0,h_val=0;
    int temp=0;
    if(map_get[xc][yr]==0)
    {
        if(map_get[xc-1][yr-1]==2/val)/*沿左上角向左上上沿*/
        {
            while(map_get[xc-a][yr-a]==2/val)
            {
                a_val=a_val+gril_value[xc-a][yr-a];
                a++;
            }
            if(map_get[xc-a][yr-a]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+a_val+present_p_v[xc][yr];
            else a_val=0;
        }
        if(map_get[xc][yr-1]==2/val)/*沿正上方向正上方上沿*/
        {
            while(map_get[xc][yr-b]==2/val)
            {
                b_val=b_val+gril_value[xc][yr-b];
                b++;
            }
            if(map_get[xc][yr-b]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+b_val+present_p_v[xc][yr];
            else b_val=0;
        }
        if(map_get[xc+1][yr-1]==2/val)/*沿右上角向右上上沿*/
        {
            while(map_get[xc+c][yr-c]==2/val)
            {
                c_val=c_val+gril_value[xc+c][yr-c];
                c++;
            }
            if(map_get[xc+c][yr-c]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+c_val+present_p_v[xc][yr];
            else c_val=0;
        }
        if(map_get[xc-1][yr]==2/val)/*沿正左方向向正左方左沿*/
        {
            while(map_get[xc-d][yr]==2/val)
            {
                d_val=d_val+gril_value[xc-d][yr];
                d++;
            }
            if(map_get[xc-d][yr]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+d_val+present_p_v[xc][yr];
            else d_val=0;
        }
        if(map_get[xc+1][yr]==2/val)/*沿正右方向向正右方右沿*/
        {
            while(map_get[xc+e][yr]==2/val)
            {
                e_val=e_val+gril_value[xc+e][yr];
                e++;
            }
            if(map_get[xc+e][yr]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+e_val+present_p_v[xc][yr];
            else e_val=0;
        }
        if(map_get[xc-1][yr+1]==2/val)/*沿左下角向左下角下沿*/
        {
            while(map_get[xc-f][yr+f]==2/val)
            {
                f_val=f_val+gril_value[xc-f][yr+f];
                f++;
            }
            if(map_get[xc-f][yr+f]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+f_val+present_p_v[xc][yr];
            else f_val=0;
            
        }
        if(map_get[xc][yr+1]==2/val)/*沿正下方向正下方下沿*/
        {
            while(map_get[xc][yr+g]==2/val)
            {
                g_val=g_val+gril_value[xc][yr+g];
                g++;
            }
            if(map_get[xc][yr+g]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+g_val+present_p_v[xc][yr];
            else g_val=0;
        }
        if(map_get[xc+1][yr+1]==2/val)/*沿右下角向右下下沿*/
        {
            while(map_get[xc+h][yr+h]==2/val)
            {
                h_val=h_val+gril_value[xc+h][yr+h];
                h++;
            }
            if(map_get[xc+h][yr+h]==val)
                present_p_v[xc][yr]=gril_value[xc][yr]+h_val+present_p_v[xc][yr];
            else h_val=0;
        }
    }
    else return 0;
    temp=a_val+b_val+c_val+d_val+e_val+f_val+g_val+h_val;
    if(temp==0)
        return 0;
    return 1;
}
/*************************************************************/
/*功能：吃掉所落棋子与同色棋子之间别的棋子                   */
/*所落棋子为（xr,yr）                                        */
/*val表示欲落子的值,白为1,黑为2                              */
/*向八个方向进行查找，吃子。                                 */
/*************************************************************/
int change_val(int xc,int yr,int val,int map_receive[10][10])
{
    int a=1,b=1,c=1,d=1,e=1,f=1,g=1,h=1;
    int temp=0;
    
    if(map_receive[xc-1][yr-1]==2/val)/*沿左上角向左上上沿*/
    {
        while(map_receive[xc-a][yr-a]==2/val)
        {
            a++;
        }
        if(map_receive[xc-a][yr-a]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<a;temp++)
                map_receive[xc-temp][yr-temp]=val;
        }
    }
    if(map_receive[xc][yr-1]==2/val)/*沿正上方向正上方上沿*/
    {
        while(map_receive[xc][yr-b]==2/val)
        {
            b++;
        }
        if(map_receive[xc][yr-b]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<b;temp++)
                map_receive[xc][yr-temp]=val;
        }
    }
    if(map_receive[xc+1][yr-1]==2/val)/*沿右上角向右上上沿*/
    {
        while(map_receive[xc+c][yr-c]==2/val)
        {
            c++;
        }
        if(map_receive[xc+c][yr-c]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<c;temp++)
                map_receive[xc+temp][yr-temp]=val;
        }
    }
    if(map_receive[xc-1][yr]==2/val)/*沿正左方向向正左方左沿*/
    {
        while(map_receive[xc-d][yr]==2/val)
        {
            d++;
        }
        if(map_receive[xc-d][yr]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<d;temp++)
                map_receive[xc-temp][yr]=val;
        }
    }
    if(map_receive[xc+1][yr]==2/val)/*沿正右方向向正右方右沿*/
    {
        while(map_receive[xc+e][yr]==2/val)
        {
            e++;
        }
        if(map_receive[xc+e][yr]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<e;temp++)
                map_receive[xc+temp][yr]=val;
        }
    }
    if(map_receive[xc-1][yr+1]==2/val)/*沿左下角向左下角下沿*/
    {
        while(map_receive[xc-f][yr+f]==2/val)
        {
            f++;
        }
        
        if(map_receive[xc-f][yr+f]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<f;temp++)
                map_receive[xc-temp][yr+temp]=val;
        }
        
    }
    if(map_receive[xc][yr+1]==2/val)/*沿正下方向正下方下沿*/
    {
        while(map_receive[xc][yr+g]==2/val)
        {
            g++;
        }
        if(map_receive[xc][yr+g]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<g;temp++)
                map_receive[xc][yr+temp]=val;
        }
    }
    if(map_receive[xc+1][yr+1]==2/val)/*沿右下角向右下下沿*/
    {
        while(map_receive[xc+h][yr+h]==2/val)
        {
            h++;
        }
        if(map_receive[xc+h][yr+h]==val)
        {
            map_receive[xc][yr]=val;
            for(temp=1;temp<h;temp++)
                map_receive[xc+temp][yr+temp]=val;
        }
    }
}
/*************************************************************/
/*功能：游戏中提示信息的显示                                 */
/*************************************************************/
void help_info(char *txt,int color)
{
    clscr(SCCOL,410,500,430,15);
    setcolor(color);
    settextstyle(0,0,1);/*设置图形模式下字符显示底字体、方向和大小*/
    outtextxy(SCCOL+15,417,txt);   /*图形模式下在x,y坐标位置显示一行字符串*/
}
/*************************************************************/
/*功能：延时函数                                             */
/*t为所延时的长度                                            */
/*************************************************************/
void wait(int t)
{
    clock_t t2,t1;
    t2=clock();
    t1=clock();
    while((t1-t2)<25*t)
    {
        t1=clock();
    }
}
/*************************************************************/
/*功能：判断游戏是否结束，并进行得分的统计，判定胜负         */
/*************************************************************/
void jud_win()
{
    int x,y;
    int jud=0;/*表示游戏已经结束*/
    num_user=0;
    num_computer=0;
    for(x=1;x<=8;x++)
    {
        for(y=1;y<=8;y++)
        {
            if(gril[x][y]==1) ++num_user;
            if(gril[x][y]==2) ++num_computer;
            if(gril[x][y]==0) jud=1;
        }
    }
    if(jud==0)/*游戏已经结束时*/
    {
        if(num_user>num_computer) help_info("You Win! ^_^       Game will Exit! ",4);
        if(num_user<num_computer) help_info("You Lose! ^_^       Game will Exit! ",0);
        if(num_user==num_computer)help_info("Dogfall! ^_^        Game will Exit! ",14);
        wait(4);
        exit(1);
    }
}
/*************************************************************/
/*功能：完成每一次有落子后的屏幕的棋局的刷新              ****/
/*************************************************************/
void repaint()
{
    int i=1,j=1;
    for(i=1;i<=8;i++)
    {
        for(j=1;j<=8;j++)
        {
            if(gril[i][j]==1)
                minbox(i,j,15);
            if(gril[i][j]==2)
                minbox(i,j,0);
            if(gril[i][j]==0)
                minbox(i,j,7);
        }
    }
    gbdw(gbwzc,gbwzr,12);
}
/*************************************************************/
/*游戏打开后第一个屏幕                                       */
/*************************************************************/
void front_screen()
{
    int il,jl,kl;
    
    clscr(0,0,640,480,15);
    win(1,1,639,479,BGCOLOR,"                 Welcome to use the programme of deepblue!",1,15);/***windows 绘制***/
    txt(192,155,"OTHELO",5,15);
    txt(190,153,"OTHELO",5,7);
    txt(205,350,"programmed by:zhang fangjun",1,7);
    txt(205,360,"E-mail:fjzhang@yeah.net",1,7);
    txt(205,370,"Http://deepblue.5280.cn",1,7);
    wait(1);
    getch();
    
}
/*************************************************************/
/*按键提示信息的帮助显示                                     */
/*************************************************************/
void help_fangxiang()
{
    drawmat(zuo16S, 16, 410,110, 15);
    drawmat(yi16S,16,   425,110, 15);
    drawmat(you16S, 16, 410,130, 15);
    drawmat(yi16S,16,   425,130, 15);
    drawmat(shang16S,16,410,150, 15);
    drawmat(yi16S, 16,  425,150, 15);
    drawmat(xia16S, 16, 410,170, 15);
    drawmat(yi16S, 16,  425,170, 15);
    drawmat(luo16S, 16, 410,190, 15);
    drawmat(zi16S, 16,  425,190, 15);
    drawmat(tui16S, 16, 410,220, 15);
    drawmat(chu16S, 16, 425,220, 15);
    
    txt(440,115," :  Left  KEY",1,15);
    txt(440,135," :  Right KEY",1,15);
    txt(440,155," :  Up    KEY",1,15);
    txt(440,175," :  Down  KEY",1,15);
    txt(440,195," :  Space KEY",1,15);
    txt(440,225," :  Esc   KEY",1,15);
    
    
}
/*************************************************************/
/*功能：进行深度搜索　　　　　　　　　　　　　　　　　　　　 */
/*************************************************************/
int compute(int map[10][10],int treelevel,int alpha,int beta)
{
    int l=0,k=0;
    int m=0,n=0;
    int map_temp_a[10][10];
    int map_temp_b[10][10];
    int p_c;				/*1表示人走白子,2表示电脑走黑子*/
    int bestvalue,value;
    int alpha_1=-1000;
    int beta_1=1000;
    
    if(treelevel%2==1)
        p_c=2;
    else p_c=1;
    
    if(treelevel==TREE_LEVEL)
    {
        return(add_value(map));
    }
    
    if(p_c==2)
        bestvalue=-INF;/*初始最佳值设为负无穷*/
    if(p_c==1)
        bestvalue=INF;/* 初始最佳值设为正无穷*/
    
    for(l=1;l<=8;l++)
    {
        for(k=1;k<=8;k++)
        {
            map_temp_a[l][k]=map[l][k];
            map_temp_b[l][k]=map[l][k];
        }
    }
    
    for(m=1;m<=8;m++)                   /*计算出当前的map时的电脑或人有几个可以落子的点-->num;*/
    {
        for(n=1;n<=8;n++)
        {
            if(putdown(m,n,p_c,map_temp_a)==1)              /*当前位置可落子时*/
            {
                change_val(m,n,p_c,map_temp_b);             /*完成对map_temp_b的吃子后图形的生成*/
                value=compute(map_temp_b,treelevel+1,alpha_1,beta_1);		/*用map_temp代入compute()中进行递归地计算*/
                
                if((p_c==2)&&(value>bestvalue))
                    bestvalue=value;
                if((p_c==1)&&(value<bestvalue))
                    bestvalue=value;
                
                if((p_c==1)&&(value<=alpha))
                    goto AB;
                if((p_c==2)&&(value>=beta))
                    goto AB;
                
                alpha_1=value;
                beta_1=value;
            }
        }
    }
AB:
    return bestvalue;
}

int add_value(int map_r[10][10])
{
    int add_p=0,add_c=0;
    int xx,yy;
    
    /*末局时，以计算棋子的数量进行各落子位的计算*/
    if(empty_num<=14)
    {
        TREE_LEVEL=14;
        for(xx=1;xx<=8;xx++)
        {
            for(yy=1;yy<=8;yy++)
            {
                if(map_r[xx][yy]==1)
                    add_p++;
                if(map_r[xx][yy]==2)
                    add_c++;
            }
        }
    }
    
    /*初、中局时，以计算棋子的数量乘以权值进行各落子位的计算*/
    if(empty_num>14)
    {
        /*if(map_r[1][1]==1)
         {
         gril_value[3][1]=-1;
         gril_value[3][2]=-1;
         gril_value[3][3]=-1;
         gril_value[2][3]=-1;
         gril_value[1][3]=-1;
         }
         if(map_r[1][8]==1)
         {
         gril_value[1][6]=-1;
         gril_value[2][6]=-1;
         gril_value[3][6]=-1;
         gril_value[3][7]=-1;
         gril_value[3][8]=-1;
         }
         if(map_r[8][1]==1)
         {
         gril_value[6][1]=-1;
         gril_value[6][2]=-1;
         gril_value[6][3]=-1;
         gril_value[6][7]=-1;
         gril_value[6][8]=-1;
         }
         if(map_r[8][8]==1)
         {
         gril_value[6][6]=-1;
         gril_value[6][7]=-1;
         gril_value[6][8]=-1;
         gril_value[7][6]=-1;
         gril_value[8][6]=-1;
         }*/
        if(map_r[1][1]==2)
        {
            gril_value[2][1]=35;
            gril_value[2][2]=35;
            gril_value[1][2]=35;
        }
        if(map_r[1][8]==2)
        {
            gril_value[1][7]=35;
            gril_value[2][7]=35;
            gril_value[8][8]=35;
            
        }
        if(map_r[8][1]==2)
        {
            gril_value[7][1]=35;
            gril_value[7][2]=35;
            gril_value[8][2]=35;
        }
        if(map_r[8][8]==2)
        {
            gril_value[8][7]=35;
            gril_value[7][7]=35;
            gril_value[7][8]=35;
        }
        
        for(xx=1;xx<=8;xx++)
        {
            for(yy=1;yy<=8;yy++)
            {
                if(map_r[xx][yy]==1)/*权值估值*/
                    add_p=add_p+gril_value[xx][yy];
                if(map_r[xx][yy]==2)
                    add_c=add_c+gril_value[xx][yy];
                
                if(putdown(xx,yy,1,map_r)==1)
                    add_p=add_p+10;
                if(putdown(xx,yy,2,map_r)==1)
                    add_c=add_c+10;
                
            }
        }
    }
    return(add_c-add_p);
}
int add_up()
{
    int num=0;
    int ax,by,m,n;
    int map_gril_a[10][10];
    int big_val[15][3]={0};
    int biggest_val[3]={0,0,0};
    
    for(ax=0;ax<=14;ax++)
        big_val[ax][0]=-1000;
    for(ax=1;ax<=8;ax++)
    {
        for(by=1;by<=8;by++)
        {
            if(putdown(ax,by,2,gril)==1)
            {
                if(((ax==1)&&(by==1))||((ax==1)&&(by==8))||((ax==8)&&(by==1))||((ax==8)&&(by==8)))
                {
                    biggest_val[1]=ax;biggest_val[2]=by;
                    goto JUMP;
                }
                for(m=1;m<=8;m++)
                {
                    for(n=1;n<=8;n++)
                    {
                        map_gril_a[m][n]=gril[m][n];
                    }
                }
                change_val(ax,by,2,map_gril_a);
                big_val[num][1]=ax;
                big_val[num][2]=by;
                big_val[num++][0]=compute(map_gril_a,2,-1000,1000);
            }
        }
    }
    biggest_val[0]=big_val[0][0];
    biggest_val[1]=big_val[0][1];
    biggest_val[2]=big_val[0][2];
    for(ax=1;ax<=14;ax++)
    {
        if(big_val[ax][0]>biggest_val[0])
        {
            biggest_val[0]=big_val[ax][0];
            biggest_val[1]=big_val[ax][1];
            biggest_val[2]=big_val[ax][2];
        }
    }
JUMP:
    gril[biggest_val[1]][biggest_val[2]]=2;
    change_val(biggest_val[1],biggest_val[2],2,gril);
}
/****************************************************************************/
/************************************************/
/*此函数完成程序的程序的主要功能                */
/*在每一个时间片里通过对键盘的监视完成相应的操作*/
/************************************************/
void main_game()
{
    int key,cant1=0,cant2=0; /*cant用来判断我是否此时无子可走,cant=0为无子可走*/
    int m,n;
    for(;;)
    {
        /*用户走子*/
        while(1)           /*if(bioskey(1))*/
        {
            /*******************/
            cant1=0;
            for(m=1;m<=8;m++)
            {
                for(n=1;n<=8;n++)
                {
                    if(putdown(m,n,1,gril)==1)
                        cant1=1;
                }
            }
            if(cant1==0)
            {
                help_info("You have to no chessman to play this time !",BGCOLOR);
                wait(2);
                break;
            }/*无子可走，给电脑走子*/
            /*********************/
            help_info("It is your turn now !",BGCOLOR);
            key=bioskey(0);
            if(key==VK_LEFT)
            {
                if(gbwzc>1)
                {
                    --gbwzc;
                    repaint();
                }
            }
            if(key==VK_RIGHT)
            {
                if(gbwzc<8)
                {
                    ++gbwzc;
                    repaint();
                }
            }
            if(key==VK_UP)
            {
                if(gbwzr>1)
                {
                    --gbwzr;
                    repaint();
                }
            }
            if(key==VK_DOWN)
            {
                if(gbwzr<8)
                {
                    ++gbwzr;
                    repaint();
                }
            }
            if(key==VK_SPACE)
            {
                if(putdown(gbwzc,gbwzr,1,gril)==1)
                {
                    gril[gbwzc][gbwzr]=1;/*完成落子*/
                    repaint();
                    change_val(gbwzc,gbwzr,1,gril);/*完成落子后的吃子*/
                    gbwzc=5;/*完成光标的复位*/
                    gbwzr=5;
                    repaint();
                    jud_win();/*统计积分,判断胜负及游戏的结束*/
                    break;/*交换走棋权*/
                }
            }
            if(key==VK_ESC)
            {
                help_info("Do you really want to Exit?(Y/N)",BGCOLOR);
                again=getch();
                if((again=='Y')||(again=='y'))
                    exit(1);
            }
        }
        /*电脑走子*/
        while(1)
        {
            cant2=0;
            for(m=1;m<=8;m++)
            {
                for(n=1;n<=8;n++)
                {
                    if(putdown(m,n,2,gril)==1)
                        cant2=1;
                }
            }
            if((cant2==0)&&(cant1==0))
            {
                help_info("You and computer have no chessman to play!!!",BGCOLOR);
                wait(2);
                
                if(num_user>num_computer) help_info("You Win! ^_^       Game will Exit! ",4);
                if(num_user<num_computer) help_info("You Lose! ^_^       Game will Exit! ",0);
                if(num_user==num_computer)help_info("Dogfall! ^_^        Game will Exit! ",14);
                wait(4);
                exit(1);
            }
            if(cant2==0)
            {
                help_info("The computer have no chessman to play this time !",BGCOLOR);
                wait(2);
                break;/*无子可走，给用户走子*/
            }
            
            /*********************/
            help_info("It is the turn of computer now !",BGCOLOR);
            empty_num=0;/*中局和末局的判断*/
            for(m=1;m<=8;m++)
            {
                for(n=1;n<=8;n++)
                {
                    if(gril[m][n]==0)
                        empty_num++;
                }
            }
            add_up();
            repaint();
            
            jud_win();/*统计积分,判断胜负及游戏的结束*/
            break;/*交换走棋权*/
        }
    }
}


/*主程序开始*/
void main()
{
    int i,j;
    
    init(VGA,VGAHI);
    front_screen();
    
    clscr(0,0,640,480,15);
    win(1,1,639,479,BGCOLOR,"                 Welcome to use the programmme of deepblue!",1,15);/***windows 绘制***/
    clscr(SCCOL,SCROW,SCCOL+MAXSIZE,SCROW+MAXSIZE,7);
    outerdraw(SCCOL,SCROW,MINBOXSIZE,8);
    linedraw(SCCOL,SCROW,MINBOXSIZE,8);
    repaint();
    help_info("Thank you for using this program !",BGCOLOR);
    help_fangxiang();
    wait(1);
    
    main_game();
    getch();
    closegraph();
}
