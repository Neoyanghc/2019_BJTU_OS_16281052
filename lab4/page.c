#include "stdio.h"
#include "stdlib.h"

typedef struct item
{
    int num; //页号
    int time;
    //等待时间，LRU算法会用到这个属性
} Pro;

int pageNum;
//系统分配给作业的主存中的页面数
int memoryNum;
//可用内存页面数
void print(Pro *page1);
//打印当前主存中的页面
int Search(int num1, Pro *memory1);
//在页面集memory1中查找num1，如果找到，返回其在memory1中的下标，否则返回-1
int Max(Pro *memory1);
int optimal(int num, int tag, Pro *memory1, Pro *page1);

int main(void)
{
    int i;
    int curmemory;
    //调入主存中的页面个数
    int missNum;
    //缺页次数
    float missRate;
    //缺页率
    char c;
    //得到用户的输入字符，来选择相应的置换算法

    Pro *page;
    //作业页面集
    Pro *memory;
    //内存页面集

    printf("输入系统分配给作业的主存中的页面数:");
    scanf("%d", &pageNum);

    printf("输入内存页面数:");
    scanf("%d", &memoryNum);

    page = (Pro *)malloc(sizeof(Pro) * pageNum);
    memory = (Pro *)malloc(sizeof(Pro) * memoryNum);

    for (i = 0; i < pageNum; i++)

    {
        printf("第 %d 个页面号为:", i);
        scanf("%d", &page[i].num);
        page[i].time = 0;
        //等待时间开始默认为0
    }

    do
    {
        for (i = 0; i < memoryNum; i++) //初始化内存中页面

        {
            memory[i].num = -1;
            //页面为空用-1表示
            memory[i].time = -1;
            //
        }
        printf("*****f:FIFO页面置换*****\n");
        printf("*****o:OPT页面置换*****\n");
        printf("*****l:LRU页面置换*****\n");
        printf("*****请选择操作类型(f,o,l),按其它键结束******\n");
        fflush(stdin);
        scanf("%c", &c);
        c=getchar();
        printf("%c",c);
        i = 0;
        curmemory = 0;

        if (c == 'f') //FIFO页面置换

        {
            missNum = 0;

            printf("FIFO页面置换情况:   \n");
            for (i = 0; i < pageNum; i++)
            {
                if (Search(page[i].num, memory) < 0) //若在主存中没有找到该页面

                {
                    missNum++;
                    memory[curmemory].num = page[i].num;
                    print(memory);
                    curmemory = (curmemory + 1) % memoryNum;
                }

            } //end for
            missRate = (float)missNum / pageNum;
            printf("缺页次数：%d   缺页率:  %f\n", missNum, missRate);

        } //end if

        if (c == 'o') //OPT页面置换

        {
            missNum = 0;

            printf("OPT页面置换情况:   \n");
            for (i = 0; i < pageNum; i++)
            {
                if (Search(page[i].num, memory) < 0) //若在主存中没有找到该页面
                {
                    if (i < memoryNum)
                        curmemory = i;
                    else
                        curmemory = optimal(page[i].num, i, memory, page);
                    missNum++;
                    memory[curmemory].num = page[i].num;
                    print(memory);
                    curmemory = (curmemory + 1) % memoryNum;
                }

            } //end for
            missRate = (float)missNum / pageNum;
            printf("缺页次数：%d   缺页率:  %.2f%%\n", missNum, missRate * 100);

        } //end if

        if (c == 'l') //LRU页面置换
        {
            missNum = 0;
            printf("LRU页面置换情况:   \n");
            for (i = 0; i < pageNum; i++)

            {
                for (int j = 0; j < memoryNum; j++)

                {
                    if (memory[j].num >= 0)
                        memory[j].time++;
                }
                if (Search(page[i].num, memory) < 0) //若在主存中没有找到该页面

                {
                    missNum++;
                    //printf("%d \n",curmemory);
                    if (i < 3)
                        curmemory = i;
                    else
                        curmemory = Max(memory);

                    memory[curmemory].num = page[i].num;
                    memory[curmemory].time = 0;
                    print(memory);
                    curmemory = (curmemory + 1) % memoryNum;
                }
                else
                {
                    curmemory = Search(page[i].num, memory);
                    memory[curmemory].time = 0;
                    curmemory = (curmemory + 1) % memoryNum;
                }

            } //end for
            missRate = (float)missNum / pageNum;
            printf("缺页次数：%d   缺页率:  %.2f%%\n", missNum, missRate * 100);

        } //end if

    } while (c == 'f' || c == 'l' || c == 'o');

    return 0;
}

void print(Pro *memory1) //打印当前的页面
{
    int j;

    for (j = 0; j < memoryNum; j++)
        printf("%d ", memory1[j].num);
    printf("\n");
}

//在页面集memory1中查找num1，如果找到，返回其在memory1中的下标，否则返回-1
int Search(int num1, Pro *memory1)
{
    int j;

    for (j = 0; j < memoryNum; j++)

    {
        if (num1 == memory1[j].num)
            return j;
    }
    return -1;
}
int Max(Pro *memory1)
{
    int max = 0;

    for (int k = 1; k < memoryNum; k++)

    {
        if (memory1[k].time > memory1[max].time)
            max = k;
    }
    return max;
}

int optimal(int num, int tag, Pro *memory1, Pro *page1)
{
    int k, j, min[100], min_k;
    for (k = 0; k < memoryNum; k++)
        min[k] = 500;
    for (k = 0; k < memoryNum; k++)

    {
        j = tag;
        do
        {

            j++;
            if (j > 20)
                break;

        } while (page1[j].num != memory1[k].num);
        if (j < min[k])

        {

            min[k] = j;
        }
    }
    int max = 0;
    for (int t = 1; t < memoryNum; t++)

    {
        if (min[t] > min[max])
            max = t;
    }
    return max;
}

