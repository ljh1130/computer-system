//
//  进程管理.c
//  蓝桥杯 acwing
//
//  Created by 李继航 on 2021/11/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define totalCmdNum   320         // 指令总数
#define BlockNum    4           // 内存块总数
#define OutBlockNum 32           // 外存块总数

// 内存块结构体
typedef struct {
    int id;            // 物理地址
    int page;        // 页号(默认为-1)
    int keepTime;    // 在内存中驻留的时间
    int last_use;    // 上次被使用的时间
} Block;

typedef struct {
    int block_id;
    int unused_time;
}lru;//在lru中记录最长未使用时间和块号


int cmdNum = 0;                    // 指令执行次数
int curCmd;                        // 当前正在执行的指令
int isExecute[totalCmdNum];        // 判断每条指令是否执行的标志数组
double lostPage = 0;            // 缺页次数
Block blocks[BlockNum];        // 内存块指针数组
int change;                    //选择替换算法，0为fifo，1为lru
int clock_sum = 0;                  //总时钟，统计程序运行的总时间

// 初始化
void init(void) {
    // 初始化标志数组
    memset(isExecute,0,320);//将标志数组全部置为0，表示没有进程正在工作
    // 设置随机数种子
    srand(time(NULL));
    // 初始化内存块数组
    for (int i = 0; i < BlockNum; i++)
    {
        blocks[i] = *(Block*)malloc(sizeof(Block));
        blocks[i].page = -1;
        blocks[i].id = i;
        blocks[i].keepTime = 0;
        blocks[i].last_use = 0;
    }
}

// 产生指令
int genCmd(void) {
    //参考题目要求，也可根据自己想法设计
   //要求遍历完全部指令即可
    //随机选取0到320之间的30个数字代表进程
    cmdNum += 1;
    int cmd;//指令号
    while (1)
    {
        cmd = rand() % 320;
        if (isExecute[cmd] == 0)
        {
            isExecute[cmd] = 1;//标记这一条指令已经被使用
            break;
        }
    }
    
    return cmd;

}

// 判断所有指令是否全部执行了一遍
int isFinish(void) {
    int flag = 0;
    // 遍历所有指令执行情况
//    for (int i = 0; i < totalCmdNum; i++) {
//
//    }
    if (cmdNum == 30)
    {
        flag = 1;
    }
    return flag;
}

// 作业调度
void dispatchJob(void) {
    curCmd = genCmd();
    int page = curCmd / 10;        // 根据指令号计算页号，也可以自己设计转换方法

    int i = 0;
    for (i=0; i < BlockNum; i++)
    {
        // 如果访问的指令在内存中，则显示其物理地址。本实验直接模拟遍历内存，可以自己实现页表、快表，有加分

        if (blocks[i].page == page)
        {
            printf("指令%3d在内存块%d中\n", curCmd, blocks[i].id);
            for (int i=0; i<BlockNum; i++)//将已在内存的页的保存时间加一
            {
                if (blocks[i].page != -1)
                {
                    blocks[i].keepTime++;
                }
            }
            for (int i=0; i<BlockNum; i++)//将未使用时间加一
            {
                if (blocks[i].page != page && blocks[i].page != -1)
                {
                    blocks[i].page++;
                }
            }
            blocks[i].last_use = clock_sum;
            clock_sum++;
            return;
        }
    }

    // 如果访问的指令不在内存中
    
//    if (1) {;
    i = 0;
    for (i=0;i<BlockNum;i++)
    {            // 寻找未分配的内存块
        if (blocks[i].page == -1)
        {
            blocks[i].page = page;
            //找到后分配
            printf("指令%3d最初被分配在内存块%d中\n", curCmd, blocks[i].id);
            for (int i=0; i<BlockNum; i++)//将已在内存的页的保存时间加一
            {
                if (blocks[i].page != -1)
                {
                    blocks[i].keepTime++;
                }
            }
            blocks[i].last_use = clock_sum;
            clock_sum++;
            lostPage++;
            return;
        }
        
    }
//    }


    // 如果所有内存块都已被分配（FIFO,LRU,NRU至少任选一种算法实现，实现多个增加得分）
    lostPage += 1;
    if (change == 0)
    {
        //置换后数据内存地址
        int oldestBlock = -1,oldestTime = 0;//最长停留时间的页编号以及最长停留时间
        for (int i=0; i<BlockNum; i++)
        {
            if (oldestTime < blocks[i].keepTime)
            {
                oldestTime = blocks[i].keepTime;
                oldestBlock = i;
            }
        }
        blocks[oldestBlock].keepTime = 0;
        blocks[oldestBlock].last_use = 0;
        blocks[oldestBlock].page = page;
        clock_sum++;
        printf("指令%3d在内存块%d中,发生置换\n", curCmd, blocks[oldestBlock].id);
    }
    else if(change == 1)
    {
        //lru替换算法
        int longest_unused = 1000000,longest_block = -1;//记录最长未被使用的时间和对应的块号
        for (int i=0; i<BlockNum; i++)
        {
            if (blocks[i].last_use < longest_unused)
            {
                longest_unused = blocks[i].last_use;
                longest_block = i;
            }
        }
        blocks[longest_block].keepTime = 0;
        blocks[longest_block].last_use = clock_sum;
        blocks[longest_block].page = page;
        printf("指令%3d在内存块%d中,发生置换\n", curCmd, blocks[longest_block].id);
    }
    

    for (i=0; i < BlockNum; i++)
    {
        if (blocks[i].page != -1)
        {
            blocks[i].keepTime++;        // 累计驻留时间，这里可实现你自己想做的其他逻辑
        }
    }
    clock_sum++;
    return;
}

int main() {
    printf("请输入置换算法（0为fifo，1为lru）\n");
    scanf("%d",&change);
    init();                    // 初始化
    while (1) {
        dispatchJob();        // 作业调度
        if (isFinish()) {    // 判断作业是否运行完成
            break;
        }
    }
    printf("\n共执行指令%d条\n", cmdNum);
    printf("缺页率约为:%.1lf%%\n", (lostPage / cmdNum)*100);
//    for (int i = 0; i < BlockNum; i++)
//    {
//        free(blocks+i);
//        *(blocks[i]) = NULL;
//    }
    return 0;
    
}

