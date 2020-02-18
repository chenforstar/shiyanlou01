#include<stdio.h>
#include<stdlib.h>
//used by linux system 
#include<cures.h>//zhi fu jie mian  zhi zuo
#include<unistd.h>//Contains function prototypes for many system services
#include<signal.h>//Signal header file that defines signal symbol constants, signal structure, and signal operation function prototypes.
//end
#include<time.h>


/*
	global variable
*/

//main interface
int a[4][4]={0};//二维数组
//numbers of spaces
int empty;
int old_y,old_x;

/*
	functions
*/

void draw();//draw the game interface
void play();//the logical body of game
void init();
void draw_one(int y,int x);
void cnt_value(int *new_y,int *new_x);
int game_over();
int cnt_one(int y,int x);



int main()
{
	init();
	play();
	endwin();

	return 0;
}

void init()
{
	int x, y;

	initscr();//设置所有curses所需的初值,清除屏幕上所有字符
	cbreak();//调用cbreak函数后，除了"Del"和"Ctrl"键外，接受其他所有字符输入
	noecho();//noecho()/echo()：关闭/打开输入回显功能
	curs_set(0);//个函数用来设制光标是否可见。它的参数可以是：0（不可见），1（可见），2（完全可见）

	empty = 15;
	srand(time(0));//用当前时间来设定rand函数所用的随机数产生演算法的种子值。rand（）产生随机数时，如果用srand（seed）播下种子之后，一旦种子相同（下面的getpid方法），产生的随机数将是相同的。当然很多时候刻意让rand（）产生的随机数随机化，用时间作种子 srand（time（NULL）），这样每次运行程序的时间肯定是不相同的，产生的随机数肯定就不一样了。

	x = rand() % 4;
	y = rand() % 4;
	a[y][x] = 2;
	draw();
}

void draw()
{
	int n, m, x, y;
	char c[4] = {'0', '0', '0', '0'};
	clear();

	for(n = 0; n < 9; n += 2)//画出横线
	{
		for(m = 0; m < 21; m++)
		{
			move(n, m);//光标挪到新的位置
			addch('-');//将单一的字符打印到屏幕上
			refresh();//显示到终端，并刷新，只刷新改变的部分
		}
	}
	for(m = 0; m < 22; m +=5)//竖着画
	{
		for(n = 1; n < 8; n++)
		{
			move(n, m);
			addch('|');
			refresh();
		}
	}
	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 4; x++)
		{
			draw_one(y, x);
		}
	}
}

void draw_one(int y, int x)
{
    int i, m, k, j;
    char c[5] = {0x00}; //数字的字符串形式，第五个元素为0
    i = a[y][x];//画出这个位置的数字
    m = 0;
    while(i > 0)//十进制求出每一位数字，变成字符形式保存到数组c中
    {
        j = i % 10;
        c[m++] = j + '0';
        i = i / 10;
    }
    m = 0;
    k = (x + 1) * 5 - 1;//输出字符的位置，乘以5是为了调整在屏幕中的位置
    while(c[m] != 0x00)//字符还没输出完
    {
        move(2*y+1, k);//y改变也是为了调整到屏幕中的位置
        addch(c[m++]);//将字符打印出来，但是没有刷新，故不会显示出来
        k--;//从右往左挪光标
    }
}

void play()
{
	int x, y, i, new_x, new_y, temp;
	int old_empty, move;
	char ch;
    
	while(1) 
	{
		move = 0;
		old_empty = empty;
		ch = getch(); //等待用户输入
		switch(ch) {
			case 97:
			case 104:
			case 68://左移
				
				for(y = 0; y < 4; y++)//按行求每个点的新值
					for(x = 0; x < 4; ) //第一种情况是：首元素非零
					{
						if(a[y][x] == 0) //默认没有数就是0，首元素为零
						{
							x++;//看左面的那个数
							continue;//继续下一层循环，就是当前位置的右边
						} 
						else //该行首元素不为为零
						{
							for(i = x + 1; i < 4; i++) //如果当前位置不是0，就要加上右边的那个数
							{
								if(a[y][i] == 0) //右边相邻没有，就继续往右找，直到找到第一个非零的
								{
									continue;
								}
								else 
								{
									if(a[y][x] == a[y][i]) //两个数相等
									{
										a[y][x] += a[y][i];//左边翻倍
										a[y][i] = 0;//右边归零
										empty++;//空的值变多
										break;//强行结束本层循环
									}
									else 
									{
										break;//如果不相等也结束，不再找该位置右边的东西了
									}
								}
							}
							x = i;//从归零位置（已经加完了）往右边判断,进行求和等一系列判断
						}
					}
				for(y = 0; y < 4; y++)
					for(x = 0; x < 4; x++) //第二种情况：第一个非零元素不是首元素
					{
						if(a[y][x] == 0) 
						{
							continue;
						} 
						else //找到非零了，但是左边是零
						{
							for(i = x; (i > 0) && (a[y][i-1] == 0); i--) 
							{
								a[y][i-1] = a[y][i];//右边的数赋值给左边
								a[y][i] = 0;//右边的数归零
							    move = 1;//标记进行了移动
							}
						}
					}
				break;//结束switch，等输入新的值
			case 100:
			case 108:
			case 67:
				for(y = 0; y < 4; y++)
					for(x = 3; x >= 0; ) 
					{
						if(a[y][x] == 0) 
						{
							x--;
							continue;
						} 
						else 
						{
							for(i = x - 1; i >= 0; i--)
							{
								if(a[y][i] == 0)
								{
									continue;
								} 
								else if(a[y][x] == a[y][i]) 
								{
									a[y][x] += a[y][i];
									a[y][i] = 0;
									empty++;
									break;
								} 
								else 
								{
									break;
								}
							}
							x = i;
						}
					}
				for(y = 0; y < 4; y++)
					for(x = 3; x >= 0; x--) 
					{
						if(a[y][x] == 0) 
						{
							continue;
						} else 
						{
							for(i = x; (i < 3) && (a[y][i+1] == 0); i++) 
							{
								a[y][i+1] = a[y][i];
								a[y][i] = 0;
							    move = 1;
							}
						}
					}
				break;
			case 119:
			case 107:
			case 65:
				for(x = 0; x < 4; x++)
					for(y = 0; y < 4; ) 
					{
						if(a[y][x] == 0)
						{
							y++;
							continue;
						} 
						else 
						{
							for(i = y + 1; i < 4; i++) 
							{
								if(a[i][x] == 0) 
								{
									continue;
								} 
								else if(a[y][x] == a[i][x]) 
								{
									a[y][x] += a[i][x];
									a[i][x] = 0;
									empty++;
									break;
								} else 
								{
									break;
								}
							}
							y = i;
						}
					}
				for(x = 0; x < 4; x++)
					for(y = 0; y < 4; y++) 
					{
						if(a[y][x] == 0) 
						{
							continue;
						} 
						else
						{
							for(i = y; (i > 0) && (a[i-1][x] == 0); i--) 
							{
								a[i-1][x] = a[i][x];
								a[i][x] = 0;
							    move = 1;
							}
						}
					}
				break;
			case 115:
			case 106:
			case 66:
				for(x = 0; x < 4; x++)
					for(y = 3; y >= 0; ) 
					{
						if(a[y][x] == 0) 
						{
							y--;
							continue;
						} 
						else 
						{
							for(i = y - 1; i >= 0; i--) 
							{
								if(a[i][x] == 0) 
								{
									continue;
								} 
								else if(a[y][x] == a[i][x])
								{
									a[y][x] += a[i][x];
									a[i][x] = 0;
									empty++;
									break;
								} 
								else
								{
									break;
								}
							}
							y = i;
						}
					}
				for(x = 0; x < 4; x++)
					for(y = 3; y >= 0; y--) 
					{
						if(a[y][x] == 0) 
						{
							continue;
						} 
						else 
						{
							for(i = y; (i < 3) && (a[i+1][x] == 0); i++) 
							{
								a[i+1][x] = a[i][x];
								a[i][x] = 0;
							    move = 1;
							}
						}
					}
				break;
			case 'Q':
			case 'q':
				game_over();
				break;
			default:
				continue;
				break;
		}
		if(empty <= 0)//满了就结素游戏
			game_over();
		if((empty != old_empty) || (move == 1)) //进行挪动了---标记：1.空格数变了，2.做了移动----》那么就重新生成新的非零数
		{ 
			do{
				new_x = rand() % 4;
				new_y = rand() % 4;//生成的新坐标
			}while(a[new_y][new_x] != 0);

			cnt_value(&new_y, &new_x);//找到最佳新坐标

			do {
				temp = rand() % 4;//产生随机数，直到产生的随机数与4的余数不是0或者2，即产生的数temp是0到3的
			}while(temp == 0 || temp == 2);//temp是余数，跳出循环时，temp为1或3
			a[new_y][new_x] = temp + 1;//产生的新数是2或4
			empty--;
		}
		draw();//重新画表
	}
}

int cnt_one(int y, int x)//判断他的周围有几个是空的
{
	int value = 1;

	if(y - 1 > 0)  
		a[y-1][x] ? 0 : value++;
	if(y + 1 < 4)
		a[y+1][x] ? 0 : value++;
	if(x - 1 >= 0)
		a[y][x-1] ? 0 : value++;
	if(x + 1 < 4)
		a[y][x+1] ? 0 : value++;
	if(y - 1 >= 0 && x - 1 >= 0)
		a[y-1][x-1] ? 0 : value++;
	if(y - 1 >= 0 && x + 1 < 4)
		a[y-1][x+1] ? 0 : value++;
	if(y + 1 < 4 && x - 1 >= 0)
		a[y+1][x-1] ? 0 : value++;
	if(y + 1 < 4 && x + 1 < 4)
		a[y+1][x+1] ? 0 : value++;

	return value;
}

void cnt_value(int *new_y, int *new_x)
{
	int max_x, max_y, x, y, value;
	int max = 0;

	max = cnt_one(*new_y, *new_x);//max表新位置周围最大的空数
	for(y = 0; y < 4; y++)
		for(x = 0; x < 4; x++) //为了找到一个空位置，他的相邻空位置数最多
		{
			if(!a[y][x]) //遍历所有空的位置
			{
				value = cnt_one(y, x);
				if(value > max && old_y != y && old_x != x) //如果右更多的空位置，并且不是已经有的位置（产生新的数据不能覆盖旧的欸）
				{
					*new_y = y;
					*new_x = x;
					old_x = x;
					old_y = y;
					break;//更换成新的坐标
				}
			}
		}
}

int game_over()
{
	sleep(1);
/*暂停
当 timeout = 0， 即 Sleep(0)，如果线程调度器的可运行队列中有大于或等于当前线程优先级的就绪线程存在，操作系统会将当前线程从处理器上移除，调度其他优先级高的就绪线程运行；如果可运行队列中的没有就绪线程或所有就绪线程的优先级均低于当前线程优先级，那么当前线程会继续执行，就像没有调用 Sleep(0)一样。

当 timeout > 0 时，如：Sleep(1)，会引发线程上下文切换：调用线程会从线程调度器的可运行队列中被移除一段时间，这个时间段约等于 timeout 所指定的时间长度。为什么说约等于呢？是因为睡眠时间单位为毫秒，这与系统的时间精度有关。*/
	endwin();//将终端机恢复到使用curses之前的设置
	exit(0);
}
