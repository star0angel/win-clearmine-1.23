#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)//取消scanf返回值的警告
#include <easyx.h>
#include <stdio.h>
#include <time.h>
#include <synchapi.h>
#include <graphics.h>
//游戏棋盘的大小
#define ROW 20  
#define COL 20
#define SIZE 30
//定义雷的数量
#define MINENUM 20
#define CLSNUM  ROW*COL-MINENUM
//棋盘的大小
#define ROWS ROW+2  
#define COLS COL+2

int mine[ROWS][COLS];//棋盘数组
IMAGE img[12];//图片文件
int clearnum;
//#include "mine.cpp"
void loadminepg()
{
	
loadimage(&img[0],_T( "./res/0.png"),SIZE,SIZE);
loadimage(&img[1], _T("./res/1.png"),SIZE,SIZE);
loadimage(&img[2], _T("./res/2.png"),SIZE,SIZE);
loadimage(&img[3], _T("./res/3.png"),SIZE,SIZE);
loadimage(&img[4], _T("./res/4.png"),SIZE,SIZE);
loadimage(&img[5], L"./res/5.png",SIZE,SIZE);
loadimage(&img[6], L"./res/6.png",SIZE,SIZE);
loadimage(&img[7], L"./res/7.png",SIZE,SIZE);
loadimage(&img[8], L"./res/8.png",SIZE,SIZE);
loadimage(&img[9], L"./res/9.png",SIZE,SIZE);
loadimage(&img[10], L"./res/flag.png",SIZE,SIZE);
loadimage(&img[11], L"./res/s.png",SIZE,SIZE);

}

void InitBoard()
{

	for (int i=0;i<ROWS;i++)
	{
		for (int j = 0; j < COLS; j++)
			mine[i][j] = 0;
	}
	//设置游戏区里的雷
	int n = MINENUM;
	while (n)
	{
		int x = rand() % ROW + 1;
		int y = rand() % ROW + 1;
		if (mine[x][y]!=-1)//是雷则跳过
		{
			mine[x][y] = -1;
			n--;
		}
	}

	for (int i=1;i<=ROW;i++)
	{
		for (int j=1;j<=COL;j++)
		{
			if (mine[i][j]!=-1)
			{
				for (int p=i-1;p<=i+1;p++)//扫描九宫格  有雷数字加一
				{
					for (int q = j - 1; q <= j + 1; q++)
					{
						if (mine[p][q]==-1)
						{
							mine[i][j]++;
						}
					}
				}
			}
		}
	}
	//将格子藏起来
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			mine[i][j] += 20; //-1 19 0 20 8 28  5 25
		}
	}
}
void printUI()
{
	for (int i = 1; i < ROW + 1; i++)
	{
		for (int j = 1; j < COL + 1; j++)
		{
			//打印数字 看到符号代替  图片
			if (mine[i][j] >= 0 && mine[i][j] <= 8)  //0-8的数字
			{

				//printf(" * ");
				//图形界面输出 0-8的图片
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[mine[i][j]]);
			}
			else if (mine[i][j] == -1)   //雷
			{
				//printf(" # ");
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[9]);
			}
			else if (mine[i][j] >= 19 && mine[i][j] <= 28) //19-28  加密的格子
			{
				//printf(" $ ");
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[11]);
			}
			else if (mine[i][j] >= 29 && mine[i][j] <= 38)  //29-38  小旗子
			{
				//printf(" & ");
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[10]);
			}
		}
		//printf("\n");
	}
}
void OpenZero(int r, int c)
{
	mine[r][c] -= 20; //打开它
	clearnum++;
	for (int i=r-1;i<=r+1;i++)
	{
		for (int j=c-1;j<=c+1;j++)
		{
			if (mine[i][j] >= 19 && mine[i][j] <= 28) //加密的格子
			{
				if (mine[i][j] == 20)
					OpenZero(i, j);
				else
				{
					clearnum++;
					mine[i][j] -= 20;//打开9宫格
				}
			}
		}
	}
}

int Play()
{
	MOUSEMSG msg = { 0 };
	msg = GetMouseMsg(); //获取鼠标信息
	int r = msg.y / SIZE + 1;//左边轴的y坐标

	int c = msg.x / SIZE + 1;//左边轴的x坐标
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		//
		
		//printf("\n %d %d \n", msg.y, msg.x);
	
		//点击！
		if (mine[r][c] == 20)  //0
		{
			OpenZero(r, c);
		}
		else if (mine[r][c] >= 19 && mine[r][c] <= 28)  //点击加密的格子
		{
			mine[r][c] -= 20;
			if (mine[r][c]!=-1)
			{
				clearnum++;
			}
		}
		return mine[r][c]; //-1 0 1 2 3 4 5 6 7 8
	}
	else if (msg.uMsg == WM_RBUTTONDOWN)
	{
	
		if (mine[r][c] >= 19 && mine[r][c] <= 28)  //点击加密的格子
		{
			mine[r][c] += 10; //做标记
		}
		else if (mine[r][c] >= 29 && mine[r][c] <= 38)  //已经被标记的
		{
			mine[r][c] -= 10; //取消标记
		}
		return mine[r][c];
	}
	return 0;
}
int main()
{
	srand((unsigned int)time(NULL));//随机数种子
	HWND winhwnd = initgraph(COL * SIZE, ROW * SIZE, 2);
	loadminepg();//加载扫雷图片
	again:
	clearnum = 0;
	

	InitBoard();//初始化棋盘数组
	//cleardevice();
	while (true)
	{
		printUI();
		if (clearnum>=CLSNUM)
		{
			MessageBox(winhwnd, _T("你赢了！"), _T("提示："), MB_OK);
			break;
		}
		if (Play()==-1)
		{
			printUI();
			MessageBox(winhwnd,_T("你输了！"),_T("提示："),MB_OK);
			break;
		}

		//Sleep(1000);
	}
	goto again;
	//system("pause");
	return 0;
	
	//putimage();



}