#include<iostream>
#include<string>
#include"FileSystem.h"
#include<Windows.h>
using namespace std;

void help() {
    cout << "***************************************************************************" << endl;
    cout << "* 1.create  + <string>                    新建一个文件,文件名为<string>   *" << endl;
    cout << "* 2.destory + <string>                    删除文件名为<string>的文件      *" << endl;
    cout << "* 3.rename  + <string> + <string>         重命名文件,第二个string是新名字 *" << endl;
    cout << "* 4.open    + <string>                    打开文件,文件名为<string>       *" << endl;
    cout << "* 5.close   + <string>                    关闭文件,文件名为<string>       *" << endl;
    cout << "* 6.write   + <index>                     写文件,文件索引为<index>        *" << endl;
    cout << "* 7.read    + <index>                     读文件,文件索引为<index>        *" << endl;
    cout << "* 8.lseek   + <index>  + <pos>            文件指针定位,文件索引为<index>  *" << endl;
    cout << "* 9.ls                                    查看所有文件                    *" << endl;
	cout << "* 10.exit                                 退出                            *" << endl;
    cout << "***************************************************************************" << endl
        << endl;
}

int Operate() {
    while (1) {
        help();
        while (1) {
            cout << endl;
            string choice;
			char name1[20],name2[20];
			int index, memarea, count;
            show_path();
            cin >> choice;
			if (choice == "create")
			{
				cin >> name1;
				create(name1);
			}
			else if (choice == "destory")
			{
				cin >> name1;
				destroy(name1);
			}
			else if (choice == "open")
			{
				cin >> name1;
				open(name1);
			}
			else if (choice == "close")
			{
				cin >> index;
				close(index);
			}
			else if (choice == "rename")
			{
				cin >> name1 >> name2;
				rename(name1,name2);
			}
			else if (choice == "write")
			{
				cin >> index;
				cin >> memarea;
				cin >> choice;
				count = choice.size();
				write(index, choice, count);

			}
			else if (choice == "read")
			{
				cin >> index;
				cin >> memarea;
				cin >> count;
				read(index, choice, count);
			}
			else if (choice == "lseek")
			{
				cin >> index;
				cin >> memarea;
				lseek(index,memarea);
			}
			else if (choice == "ls")
			{
				dir();
			}
            else if (choice == "clear") {
                system("CLS");
                help();
            } else if (choice == "exit") {
                system("CLS");
                return 0;
            } 
        }
    }

}

int main() {
	int choice = 0;
	system("CLS");
	Sleep(2);
	cout << "\t\t*****************************************\t" << endl;
	cout << "\t\t             文  件  系  统          \t" << endl;
	Operate();
	return 0;
}
