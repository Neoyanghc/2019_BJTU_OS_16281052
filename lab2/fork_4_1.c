// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <stdio.h>
#include <time.h>

int main()
{
	long int a = 1;
	int x, y;
	double b = 1.0;
	clock_t start, end;
	printf("请输入整数运算与浮点数运算概率（9  1）\n");
	scanf_s("%d %d", &x, &y);            /*控制运行频率*/
	start = clock();
	for (int i = 0; i < int (x * 100000000); i++)                  /*整数操作*/
	{
		a *= 123456;
	}
	end = clock();
	double  time1 = (double)(end - start) / CLK_TCK;
	printf("整数运行时间为%f\n",time1 );
	start = clock();
	for (int i = 0; i < int( y * 100000000); i++)                /*浮点操作*/
	{
		b *= 1.23456;
	}
	end = clock();
	double time2 = (double)(end - start) / CLK_TCK;
	printf("浮点数运行时间%f\n", time2);
	printf("加权运行时间为 %f\n", double((y*time2+x*time1)/(x+y)));
	return 0;
}
