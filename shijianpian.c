#include "stdio.h"
#include "stdlib.h"

typedef struct PCB
{
     int name;//进程id
     int runtime;//剩余需要执行的时间，可随机分配
     int runedtime;//已经执行的时间
     int state;//执行状态
     int killtime; //统计进程结束的时间，这个参数选做！
     struct PCB *next;
}PCB;

typedef struct link{
    PCB *head ;
    PCB *tail ;
}linkqueue;//就绪队列的数据结构

typedef struct run{
    PCB *node;
    PCB *next;
}runlink;


int killtimes[10];
#define NUM 10 //10个pcb

int main()
{
    int timeslice=3;
    linkqueue *queue;//就绪队列
    queue = (struct link*)malloc(sizeof(struct link));
    queue->head = queue->tail = NULL;
    runlink *runqueue = NULL;
    PCB *top = NULL,*tail = NULL,*temp = NULL;//队列指针
    int i;
//进程创建过程
    printf("进程基本信息：\n");
   for(i=0;i<NUM;i++)//for循环创建进程流程
    {
         temp = (struct PCB*)malloc(sizeof(struct PCB));
         //构建就绪队列
        int t_name = i;
        int t_runtime = rand() % 20;
        int t_runedtime = 0;
        temp->name = t_name;
        temp->runtime = t_runtime;
        temp->runedtime = t_runedtime;
        temp->killtime = 0;
        printf("process name=%d, runtime=%d, runedtime=%d\n"
        ,temp->name,temp->runtime,temp->runedtime);
        if (queue->head == NULL && queue->tail == NULL)
        {
            queue->head = queue->tail = temp;
        }
        else
        {
            queue->tail->next = temp;
            queue->tail = temp;
        }
    }
    
    //时间片轮转调度
    top = queue->head;
    tail = queue->tail;
    printf("*********************************************\n");
    printf("进程调度过程：\n");
    int times = 0;//统计时间片执行了多少次
    runqueue = (struct run*)malloc(sizeof(struct run));
    while(top!=NULL)
    {
        if (top->runtime <= 0)
        {
            break;
        }
        runqueue->next = top;//从就绪队列中选择一个pcb执行
        
        //减少一个时间片，模拟实行时间片过程
        int start_runtime = runqueue->next->runtime;
        //记录每一次时间片运行之前PCB剩余的运行时间
        runqueue->next->runtime= runqueue->next->runtime-timeslice;
        int a = runqueue->next->runtime;
        if(a<=0)
        {
            //如果执行时间用完，处理逻辑
            if (runqueue->next->runtime < 0)
            {
                runqueue->next->runtime = 0;
            }
              runqueue->next->runedtime += start_runtime;
              top = top->next;
            printf("process name=%d, runtime=%d, runedtime=%d\n",runqueue->next->name,runqueue->next->runtime,runqueue->next->runedtime);
            runqueue->next->killtime = times*3 + start_runtime;
            killtimes[runqueue->next->name] = runqueue->next->killtime;
        }
        else{
        //如果没结束，执行逻辑
            runqueue->next->runedtime += timeslice;
            tail->next = runqueue->next;
            tail = tail->next;
            top = top->next;
            printf("process name=%d, runtime=%d, runedtime=%d\n"
            ,runqueue->next->name,runqueue->next->runtime,runqueue->next->runedtime);
            runqueue->next = NULL;
            }
        times++;
        }
    printf("****************************************\n");
    printf("每个进程结束的时间：\n");
    for (int i=0; i<10; i++)
    {
        printf("process name=%d, killtime=%d\n",i,killtimes[i]);
    }
    
    }
