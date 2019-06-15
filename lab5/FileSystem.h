#include<iostream>
#include<string.h>
#include<string>
#include<iomanip>
#include <stdlib.h>
#include<stdio.h>
using namespace std;


#define MAX_NAME  20    //文件名或者目录名字最大字节数
#define C_MAX  100
#define B_MAX  100
#define H_MAX  10



int read_block(int i, char* p);
int write_block(int i, char* p);
int create(char *filename);// 根据指定的文件名创建新文件。
int destroy(char *filename);// 删除指定文件。
int open(char *filename);// 打开文件。该函数返回的索引号可用于后续的read, write, lseek, 或close操作。
int close(int index);// 关闭制定文件。
int rename(char *old, char *new1);
int read(int index, string mem_area,int count);// 从指定文件顺序读入count个字节memarea指定的内存位置。读操作从文件的读写指针指示的位置开始。
int write(int index, string mem_area,int count);// 把memarea指定的内存位置开始的count个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始。
int lseek(int index, int pos);// 把文件的读写指针移动到pos指定的位置。pos是一个整数，表示从文件开始位置的偏移量。文件打开时，读写指针自动设置为0。每次读写操作之后，它指向最后被访问的字节的下一个位置。lseek能够在不进行读写操作的情况下改变读写指针能位置。
int dir();
int show_path();

