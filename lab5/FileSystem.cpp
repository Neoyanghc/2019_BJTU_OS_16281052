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
	char data[512];   //��Ӧ�������ֽ�
	char flag;        //�Ƿ�洢
	MyBlock* next;
}myblock;


typedef struct File         //�ļ��ṹ��
{
	char* name;            //�ļ�����
	char* time;            //����ʱ��
	char* changeTime;      //�޸�ʱ��
	myblock* content;
}file;

typedef struct OpenFile
{
	int index;
	int pos;
	myblock* data;
}openFile;

myblock ldisk[C_MAX][H_MAX][B_MAX];  // ����ģ�� CHB �ֱ��ʾ����ţ���ͷ�ź�������
string error[] = { "/","\\", ":","<",">","|","*","&" };  //�����еķǷ��ַ�
openFile item[C_MAX];
char buffer[100]; //���ݻ�����

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
int create(char* filename)// ����ָ�����ļ����������ļ���
{
	file newfile;
	newfile.name = filename;

	//�����Ƿ��зǷ��ַ�
	for (int i = 0; i < 8; i++)
	{
		if (strchr(filename, error[i][0]))
		{
			cout << "error �������зǷ��ַ�������ʧ��" << endl;
			break;
		}
	}
	int temp = 1;
	int pos = 0;
	// �Ƿ������ظ�
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
	//����ռ�
	if (temp == 0)
	{
		cout << "error �����ظ�������ʧ��" << endl;
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
		cout << "ok  �����ɹ�" << endl;
	}
	return 1;
}
int destroy(char* filename)// ɾ��ָ���ļ���
{
	int temp = 1;
	int pos = 0;
	// �ҵ��ļ�
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
		cout << "error δ�ҵ��ļ����޷�ɾ��" << endl;
	}
	else {
		ldisk[0][0][pos].flag = '0';
		ldisk[0][0][pos].data[0] = '\0';
		ldisk[0][0][pos].next = NULL;
		cout << "ok  ɾ���ɹ�" << endl;
	}
	return 0;
}
int open(char* filename)// ���ļ����ú������ص������ſ����ں�����read, write, lseek, ��close������
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
		cout << "error δ�ҵ��ļ����޷���" << endl;
	}
	else {
		cout << "ok  �򿪳ɹ�" << endl;

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
int close(int index)// �ر��ƶ��ļ���
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
	cout << "�رճɹ�" << endl;
	return 0;
}
int rename(char *old, char *new1)
{
	int temp = 1;
	int pos = 0;
	// �ҵ��ļ�
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
		cout << "δ�ҵ��ļ�" << endl;
	}
	else {
		strcat(ldisk[0][0][pos].data, new1);
		cout << "�ļ������ɹ�" << endl;
	}
	return 0;
}
int read(int index, string mem_area, int count)// ��ָ���ļ�˳�����count���ֽ�memareaָ�����ڴ�λ�á����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
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
			cout << "�����ɹ�" << endl;
			return 0;
		}
	}
	cout << "�Ҳ����ļ�,������" << endl;
	return 0;
}
int write(int index, string mem_area, int count)// ��memareaָ�����ڴ�λ�ÿ�ʼ��count���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
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
			cout << "д��ɹ�" << endl;
			return 0;
		}
	}
	cout << "�Ҳ����ļ�,������" << endl;
	return 0;
}
int lseek(int index, int pos)// ���ļ��Ķ�дָ���ƶ���posָ����λ�á�pos��һ����������ʾ���ļ���ʼλ�õ�ƫ�������ļ���ʱ����дָ���Զ�����Ϊ0��ÿ�ζ�д����֮����ָ����󱻷��ʵ��ֽڵ���һ��λ�á�lseek�ܹ��ڲ����ж�д����������¸ı��дָ����λ�á�
{
	for (int j = 0; j < C_MAX; j++)
	{
		if (item[j].index == index)
		{
			item[j].pos = pos;
			cout << "ָ�����óɹ�" << endl;
			return 0;
		}
	}
	cout << "�Ҳ����ļ�,������" << endl;
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
int show_path()//ʵ����ʾ��ǰ·���ĺ���
{
    cout << "FileSystem/";
    return 1;
}

