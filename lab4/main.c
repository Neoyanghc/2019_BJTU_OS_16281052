#include "windows.h"
#include <conio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include<iostream>
#include<time.h>
using namespace std;
#define MAX 64


void FIFO();          //先进先出算法
void Best();          //最佳置换算法
void PBA();          //页面缓冲算法PBA
void LRU();           //最近最久未使用算法
void CLOCK();        //改进型Clock置换算法
void init();    //初始化相关数据结构
void generate();      //随机生成访问序列
void display();   //显示当前状态及缺页情况
int  find_page(int page);      //查找页面是否在内存
bool change();

struct PageInfo       //页面信息结构
{
	int  data[MAX];  // 模拟的最大访问页面数
	int  flag;         // 标志位，0表示无页面访问数据
	int  unfind;    // 统计信息，缺页次数，缺页中断次数
	int  size;     // 分配的页框数
	int  length;     // 访问页面序列长度
} page_info;

struct memoryInfo
{
	int time; //记录页框中数据的访问历史
	int A;//访问位
	int M;//修改位
	int data;//页号,16位信息，前6位代表页号，后10位为偏移地址 0~2^16
}memory_info;

memoryInfo page_list[MAX];      // 分配的页框
int  loss = 0;                 // 缺页次数
int  current_page;             //页面访问指针
int  change_page;             //页面替换指针
int  flag_of_page;            //缺页标志，0为不缺页，1为缺页



//初始化相关数据结构
void init()
{
	int i, pf;
	flag_of_page = 0;              //缺页标志，0为不缺页，1为缺页
	page_info.unfind = 0;   // 缺页次数
	page_info.flag = 0;        // 标志位，0表示无页面访问数据
	printf("\n请输入要分配的页框数：");   // 自定义分配的页框数 
	scanf("%d", &pf);
	page_info.size = pf;
	for (i = 0; i < MAX; i++)   // 清空页面序列
	{
		page_info.data[i] = -1;
	}
}


// 随机生成访问序列
void generate(void)
{
	int pn;
	init();     //初始化相关数据结构
	printf("\n请输入要随机生成访问序列的长度：");   //自定义随机生成访问序列的长度
	scanf("%d", &pn);
	page_info.length = pn;
	srand((unsigned)time(NULL)); //用时间做种，每次产生随机数不一样
	int  p = rand() % MAX;
	int i, j=0;
	double t;
	int m = 8;
	int e = 8;
	int s = 10;
	t = rand() % 10 / 10.0; // 0-1之间的数
	//如果想继续加大页面访问序列串的长度，请返回第2步，否则结束。
	for (i = 0; i < s; i++)
	{
		if (j > page_info.length)
			break;
		//生成m个取值范围在p和p + e间的随机数，并记录到页面访问序列串中；
		for (j = i * m; j < (i + 1) * m; j++)
		{
			page_info.data[j]= (p + (rand() % e)) % MAX;
		}
		// 生成一个随机数r，0 ≤ r ≤ 1；
		double r = (rand() % 10) / 10.0;
		// 如果r < t，则为p生成一个新值
		if (r < t)
		{
			p = rand() % MAX;
		}
		//否则p = (p + 1) mod N；
		else
		{
			p = (p + 1) % MAX;
		}
	}
}


//  显示当前状态及缺页情况
void display(void)
{
	int i, n;
	if (current_page == 0)
	{
		printf("\n=============页面访问序列=============\n");
		for (i = 0; i < page_info.length; i++)
		{
			printf("%4d", page_info.data[i]);
			if ((i + 1) % 10 == 0) printf("\n");   //每行显示10个	  	 		   
		}
		printf("\n======================================\n");
	}
	printf("访问%3d : 内存<", page_info.data[current_page]);
	for (n = 0; n < page_info.size; n++)     // 页框信息
	{
		if (page_list[n].data >= 0)
			printf("%3d", page_list[n].data);
		else
			printf("   ");
	}
	printf(" >");
	if (flag_of_page == 1)     //缺页标志，0为不缺页，1为缺页
	{
		printf(" ==>缺页 ");
		if (current_page == 0)
		{
			printf("缺页率%3.1f", (float)(page_info.unfind) * 100.00 );
		}
		else{ printf("缺页率%3.1f", (float)(page_info.unfind) * 100.00 / current_page); }
		
	}
	printf("\n");
}


// 查找页面是否在内存，1为在内存，0为不在即缺页
int find_page(int page)
{
	int n;
	for (n = 0; n < page_info.size; n++)
	{
		page_list[n].time++;   // 访问历史加1
	}
	for (n = 0; n < page_info.size; n++)
	{
		if (page_list[n].data == page)
		{
			flag_of_page = 0;    //flag_of_page缺页标志，0为不缺页，1为缺页	   
			page_list[n].time = 0;   //置访问历史为0
			if (change()) {
				page_list[n].A = 1;
				page_list[n].M = 1;
			}
			else {
				page_list[n].A = 1;
			}
			return 1;
		}
	}
	flag_of_page = 1;  	//页面不存在，缺页
	return 0;
}


//  FIFO页面置换算法
void FIFO(void)
{
	int n, full, status;
	change_page = 0;          // 页面替换指针初始化为0
	loss = 0;  // 缺页数初始化为0
	
	full = 0;           // 是否装满是所有的页框
	for (n = 0; n < page_info.size; n++) // 清除页框信息
	{
		page_list[n].data = -1;
	}
	flag_of_page = 0;   //缺页标志，0为不缺页，1为缺页
	for (current_page = 0; current_page < page_info.length; current_page++)  // 执行算法
	{
		status = find_page(page_info.data[current_page]);  //查找页面是否在内存
		if (full < page_info.size)    // 开始时不计算缺页
		{
			if (status == 0)    // 页不存在则装入页面
			{
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				full++;
			}
		}
		else      // 正常缺页置换
		{
			if (status == 0)    // 页不存在则置换页面
			{
				
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				page_info.unfind++;     // 缺页次数加1				  			
			}
		}
		Sleep(10);
		display();       // 显示当前状态
	}	 // 置换算法循环结束
	_getch();
	return;
}

void Best()
{
	int n, full, status;
	change_page = 0;          // 页面替换指针初始化为0
	loss = 0;  // 缺页数初始化为0

	full = 0;           // 是否装满是所有的页框
	for (n = 0; n < page_info.size; n++) // 清除页框信息
	{
		page_list[n].data = -1;
	}
	flag_of_page = 0;   //缺页标志，0为不缺页，1为缺页
	for (current_page = 0; current_page < page_info.length; current_page++)  // 执行算法
	{
		status = find_page(page_info.data[current_page]);  //查找页面是否在内存
		if (full < page_info.size)    // 开始时不计算缺页
		{
			if (status == 0)    // 页不存在则装入页面
			{
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				full++;
			}
		}
		else      // 正常缺页置换
		{
			if (status == 0)    // 页不存在则置换页面
			{
				int min,max = 0 ; //很大的数
				for (int m = 0; m < page_info.size ;m++)
				{
					min = 1000;
					for (int n = current_page; n < page_info.length; n++)
					{
						if (page_info.data[n] == page_list[m].data)
						{
							min = n;
							break;
						}
					}
					if (max < min)
					{
						max = min;
						change_page = m;
					}
					
				}
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				page_info.unfind++;     // 缺页次数加1				  			
			}
		}
		Sleep(10);
		display();       // 显示当前状态
	}	 // 置换算法循环结束
	_getch();
	return;
}

void PBA()
{
	int n, full, status;
	change_page = 0;          // 页面替换指针初始化为0
	loss = 0;  // 缺页数初始化为0
	memoryInfo page_buffer[MAX];      // 分配的页框
	      // 随机生成访问序列
	full = 0;           // 是否装满是所有的页框
	for (n = 0; n < page_info.size; n++) // 清除页框信息
	{
		page_list[n].data = -1;
		page_buffer[n].data = -1;
	}
	flag_of_page = 0;   //缺页标志，0为不缺页，1为缺页
	for (current_page = 0; current_page < page_info.length; current_page++)  // 执行算法
	{
		status = find_page(page_info.data[current_page]);  //查找页面是否在内存
		if (full < page_info.size)    // 开始时不计算缺页
		{
			if (status == 0)    // 页不存在则装入页面
			{
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				full++;
			}
		}
		else      // 正常缺页置换
		{
			if (status == 0)    // 页不存在则置换页面
			{
				int num = 0;
				page_buffer[num++] = page_list[change_page]; //添加到buffer中
				page_list[change_page].data = page_info.data[current_page];
				change_page = (change_page + 1) % page_info.size;
				page_info.unfind++;     // 缺页次数加1				  			
			}
		}
		Sleep(10);
		display();       // 显示当前状态
	}	 // 置换算法循环结束
	_getch();
	return;

}


//查找替换页面算法，递归实现
int whichpage(int num) {

	int j;
	for (j = 0; j < page_info.size; j++)
	{
		if (page_list[(j + num) % page_info.size].A == 0 && page_list[(j+num)%page_info.size].M == 0)
		{
			return (j+num)%page_info.size;
		}
	}
	for (j = 0; j < page_info.size; j++)
	{
		if (page_list[(j + num) % page_info.size].A == 0 && page_list[(j + num) % page_info.size].M == 1)
		{
			return (j + num) % page_info.size;
		}
		page_list[(j + num) % page_info.size].A = 0;
	}
	for (j = 0; j < page_info.size; (j+num)%page_info.size++)
	{
		page_list[(j + num) % page_info.size].A = 0;
	}
	return whichpage(num);
}


//随机化函数
bool change()
{
	if ((rand() % 2 + 1) == 1)
	{
		return true;
	}
	else
		return false;
}


void CLOCK()
{
	int n, full, status;
	int num = -1;
	change_page = 0;          // 页面替换指针初始化为0
	loss = 0;  // 缺页数初始化为0
	
	full = 0;           // 是否装满是所有的页框
	for (n = 0; n < page_info.size; n++) // 清除页框信息
	{
		page_list[n].data = -1;
		page_list[n].M = 0;
		page_list[n].A = 0;
		page_list[n].time = 0;
	}
	flag_of_page = 0;   //缺页标志，0为不缺页，1为缺页
	for (current_page = 0; current_page < page_info.length; current_page++)  // 执行算法
	{
		status = find_page(page_info.data[current_page]);  //查找页面是否在内存
		if (full < page_info.size)    // 开始时不计算缺页
		{

			if (status == 0)    // 页不存在则装入页面
			{
				page_list[change_page].data = page_info.data[current_page];
				page_list[n].A = 1;
				full++;
			}
		}
		else      // 正常缺页置换
		{
			if (status == 0)    // 页不存在则置换页面
			{
				change_page = whichpage(++num);
				page_list[change_page].data = page_info.data[current_page];
				page_list[change_page].A = 1;
				page_info.unfind++;     // 缺页次数加1				  			
			}
		}
		Sleep(10);
		display();       // 显示当前状态
	}	 // 置换算法循环结束
	_getch();
	return;
}


//  LRU页面置换算法
void LRU(void)
{
	int n, full, status, max;
	change_page = 0;          // 页面替换指针
	loss = 0;  // 缺页次数初始化为0
	
	full = 0;           // 是否装满所有的页框
	for (n = 0; n < page_info.size; n++)
	{
		page_list[n].data = -1;    // 清除页框信息
		page_list[n].time = 0;   // 清除页框历史
	}
	flag_of_page = 0;    //缺页标志，0为不缺页，1为缺页
	for (current_page = 0; current_page < page_info.length; current_page++)  // 执行算法
	{
		status = find_page(page_info.data[current_page]);  //查找页面是否在内存
		if (full < page_info.size)   // 开始时不计算缺页
		{
			if (status == 0)   // 页不存在则装入页面
			{
				page_list[change_page].data = page_info.data[current_page]; //把要调入的页面放入一个空的页框里
				change_page = (change_page + 1) % page_info.size;
				full++;
			}
		}
		else //正常缺页置换
		{
			if (status == 0)//页不存在则置换页面
			{
				max = 0;
				for (n = 1; n < page_info.size; n++)
				{
					if (page_list[n].time > page_list[max].time)
					{
						max = n;
					}
				}
				change_page = max;
				page_list[change_page].data = page_info.data[current_page];
				page_list[change_page].time = 0;
				page_info.unfind++;  // 缺页次数加1				
			}
		}
		Sleep(10);
		display();    // 显示当前状态
	} 	// 置换算法循环结束
	_getch();
	return;
}



//  主函数

int main()
{
	char ch;
	system("cls");
	while (true)
	{
		printf("*******************************************\n");
		printf("     若要执行Best 页面置算法请按“1” \n");
		printf("     若要执行FIFO 页面置算法请按“2”\n");
		printf("     若要执行LRU  页面置算法请按“3” \n");
		printf("     若要执行Clock页面置算法请按“4” \n");
		printf("     若要执行PBA  页面置算法请按“5” \n");
		printf("     若是“6”程序退出\n");
		printf("*******************************************\n");
		printf("Enter your choice : ");
		do
		{   //如果输入信息不正确，继续输入
			ch = (char)_getch();
		} while (ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5' && ch != '6');
		printf("\n\n你按的是：%c ,现在为你执行对应操作。", ch);
		if (ch == '6') //选择5，退出
		{
			return 0;
		}
		else
		{
			if (ch == '1')
			{

				
			}
			else if (ch == '2')
			{
				printf("\n\n----------*****执行FIFO算法*****----------\n");
				FIFO();

			}
			else if (ch == '3')
			{
				printf("\n\n----------*****执行LRU算法*****----------\n");
				LRU();
			}
			else if (ch == '4')
			{
				printf("\n\n----------*****执行CLOCK算法*****----------\n");
				CLOCK();
			}
			else {
				printf("\n\n----------*****执行PBA算法*****----------\n");
				PBA();
			}
		}
		//system("cls");
	}
	printf("\n\nPress Any Key To Continue:");

	return 0;
}

//测试代码
/*
int main()
{
	generate();        // 随机生成访问序列
	printf("\n\n----------*****执行Best算法*****-----------\n");
	clock_t start, finish;
	double totaltime;
	start = clock();
	Best();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	printf("\n\n----------*****执行Best算法*****-----------\n");

	start = clock();
	FIFO();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	printf("\n\n----------*****执行Best算法*****-----------\n");

	start = clock();
	LRU();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	printf("\n\n----------*****执行Best算法*****-----------\n");

	start = clock();
	CLOCK();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	printf("\n\n----------*****执行Best算法*****-----------\n");

	start = clock();
	PBA();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
}
*/
