#include<iostream>
#include<string.h>
#include<string>
#include<iomanip>
#include <stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<iomanip>
#include"FileSystem.h"
using namespace std;



typedef struct MyBlock
{
	char data[512];   //对应扇区的字节
	char flag;        //是否存储
	MyBlock* next;
}myblock;


typedef struct File         //文件结构体
{
	char* name;            //文件命名
	char* time;            //创建时间
	char* changeTime;      //修改时间
	myblock* content;
}file;

typedef struct OpenFile
{
	int index;
	int pos;
	myblock* data;
}openFile;

myblock ldisk[C_MAX][H_MAX][B_MAX];  // 磁盘模型 CHB 分别表示柱面号，磁头号和扇区号
string error[] = { "/","\\", ":","<",">","|","*","&" };  //命名中的非法字符
openFile item[C_MAX];
char buffer[100]; //数据缓存区

int read_block(int i, char* p)
{
	for (int j = 0; j < i; j++)
	{
		p[j] = buffer[j];
	}
	return 1;
}
int write_block(int i, char* p)
{
	for (int j = 0; j < i; j++)
	{
		buffer[j] = p[j];
	}
	return 1;
}
int create(char* filename)// 根据指定的文件名创建新文件。
{
	file newfile;
	newfile.name = filename;

	//命名是否含有非法字符
	for (int i = 0; i < 8; i++)
	{
		if (strchr(filename, error[i][0]))
		{
			cout << "error 命名含有非法字符，创建失败" << endl;
			break;
		}
	}
	int temp = 1;
	int pos = 0;
	// 是否命名重复
	for (int i = 0; i < B_MAX; i++)
	{
		if (ldisk[0][0][i].flag == '1')
		{
			if (strcmp(ldisk[0][0][i].data,filename)==0)
			{
				temp = 0;
				break;
			}
		}
		else {
			pos = i;
		}
	}
	//申请空间
	if (temp == 0)
	{
		cout << "error 命名重复，创建失败" << endl;
	}
	else {
		ldisk[0][0][pos].flag = '1';
		strcat(ldisk[0][0][pos].data, filename);
		for (int m = 1; m < C_MAX; m++)
		{
			for (int n = 1; n < H_MAX; n++)
			{
				for (int p =0 ; p < B_MAX; p++)
				{
					if (ldisk[m][n][p].flag != '1')
					{
						ldisk[m][n][p].flag = '1';
						ldisk[0][0][pos].next = &ldisk[m][n][p];
						break;
					}
				}
			}
		}
		cout << "ok  创建成功" << endl;
	}
	return 1;
}
int destroy(char* filename)// 删除指定文件。
{
	int temp = 1;
	int pos = 0;
	// 找到文件
	for (int i = 0; i < B_MAX; i++)
	{
		if (ldisk[0][0][i].flag == '1')
		{
			if (strcmp(ldisk[0][0][i].data, filename)== 0)
			{
				pos = i;
				break;
			}
		}
	}
	if (pos == 0)
	{
		cout << "error 未找到文件，无法删除" << endl;
	}
	else {
		ldisk[0][0][pos].flag = '0';
		ldisk[0][0][pos].data[0] = '\0';
		ldisk[0][0][pos].next = NULL;
		cout << "ok  删除成功" << endl;
	}
	return 0;
}
int open(char* filename)// 打开文件。该函数返回的索引号可用于后续的read, write, lseek, 或close操作。
{
	int pos = 0;
	for (int i = 0; i < B_MAX; i++)
	{
		if (ldisk[0][0][i].flag == '1')
		{
			if (strcmp(ldisk[0][0][i].data, filename)==0)
			{
				pos = i;
				break;
			}
		}
	}
	if (pos == 0)
	{
		cout << "error 未找到文件，无法打开" << endl;
	}
	else {
		cout << "ok  打开成功" << endl;

		for (int j = 0; j < C_MAX; j++)
		{
			if (item[j].index == 0)
			{
				item[j].index = 19706500 + j;
				item[j].pos = 0;
				item[j].data = ldisk[0][0][pos].next;
				cout << "index: " << item[j].index << endl;
				return item[j].index;
			}

		}
	}
	return 0;
}
int close(int index)// 关闭制定文件。
{
	for (int j = 0; j < C_MAX; j++)
	{
		if (item[j].index == index)
		{
			item[j].index == 0;
			item[j].pos = 0;
			item[j].data = NULL;
		}

	}
	cout << "关闭成功" << endl;
	return 0;
}
int rename(char *old, char *new1)
{
	int temp = 1;
	int pos = 0;
	// 找到文件
	for (int i = 0; i < B_MAX; i++)
	{
		if (ldisk[0][0][i].flag == '1')
		{
			if (strcmp(ldisk[0][0][i].data, old)==0)
			{
				pos = i;
				break;
			}
		}
	}
	if (pos == 0)
	{
		cout << "未找到文件" << endl;
	}
	else {
		strcat(ldisk[0][0][pos].data, new1);
		cout << "文件改名成功" << endl;
	}
	return 0;
}
int read(int index, string mem_area, int count)// 从指定文件顺序读入count个字节memarea指定的内存位置。读操作从文件的读写指针指示的位置开始。
{
	for (int j = 0; j < C_MAX; j++)
	{
		if (item[j].index == index)
		{
			for (int i = 0; i < count; i++)
			{
			    mem_area[i] = item[j].data->data[i + item[j].pos];
				cout << mem_area[i];
			}
			cout << "读出成功" << endl;
			return 0;
		}
	}
	cout << "找不到文件,请重试" << endl;
	return 0;
}
int write(int index, string mem_area, int count)// 把memarea指定的内存位置开始的count个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始。
{
	for (int j = 0; j < C_MAX; j++)
	{
		if (item[j].index == index)
		{
			myblock* p = item[j].data;
			for (int i = 0; i < count; i++)
			{
				if (i+1 % 512 == 0)
				{
					p->next = NULL;
					p = p->next;
				}
				p->data[item[j].pos] = mem_area[i];
				item[j].pos++;
			}
			cout << "写入成功" << endl;
			return 0;
		}
	}
	cout << "找不到文件,请重试" << endl;
	return 0;
}
int lseek(int index, int pos)// 把文件的读写指针移动到pos指定的位置。pos是一个整数，表示从文件开始位置的偏移量。文件打开时，读写指针自动设置为0。每次读写操作之后，它指向最后被访问的字节的下一个位置。lseek能够在不进行读写操作的情况下改变读写指针能位置。
{
	for (int j = 0; j < C_MAX; j++)
	{
		if (item[j].index == index)
		{
			item[j].pos = pos;
			cout << "指针重置成功" << endl;
			return 0;
		}
	}
	cout << "找不到文件,请重试" << endl;
	return 0;
}
int dir() {
	for (int i = 0; i < B_MAX; i++)
	{
		if (ldisk[0][0][i].flag == '1')
		{
			cout << ldisk[0][0][i].data << endl;
		}
	}
	return 0;
}
int show_path()//实现显示当前路径的函数
{
    cout << "FileSystem/";
    return 1;
}

