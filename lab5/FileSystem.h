#include<iostream>
#include<string.h>
#include<string>
#include<iomanip>
#include <stdlib.h>
#include<stdio.h>
using namespace std;


#define MAX_NAME  20    //�ļ�������Ŀ¼��������ֽ���
#define C_MAX  100
#define B_MAX  100
#define H_MAX  10



int read_block(int i, char* p);
int write_block(int i, char* p);
int create(char *filename);// ����ָ�����ļ����������ļ���
int destroy(char *filename);// ɾ��ָ���ļ���
int open(char *filename);// ���ļ����ú������ص������ſ����ں�����read, write, lseek, ��close������
int close(int index);// �ر��ƶ��ļ���
int rename(char *old, char *new1);
int read(int index, string mem_area,int count);// ��ָ���ļ�˳�����count���ֽ�memareaָ�����ڴ�λ�á����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
int write(int index, string mem_area,int count);// ��memareaָ�����ڴ�λ�ÿ�ʼ��count���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
int lseek(int index, int pos);// ���ļ��Ķ�дָ���ƶ���posָ����λ�á�pos��һ����������ʾ���ļ���ʼλ�õ�ƫ�������ļ���ʱ����дָ���Զ�����Ϊ0��ÿ�ζ�д����֮����ָ����󱻷��ʵ��ֽڵ���һ��λ�á�lseek�ܹ��ڲ����ж�д����������¸ı��дָ����λ�á�
int dir();
int show_path();

