#include <stdio.h>
#include <string.h>
#include <stdlib.h>
double avg_TurnaroundTime;//平均周转时间
double AQTT;//平均带权周转时间
typedef struct progress{
    char proName[80];
    int arriveTime;//到达时间
    int serviceTime;//服务时间
    //int flag;        //访问标志，未访问1，访问0
    int finishTime;//完成时间
    int cyclingTime;//周转时间
    double float_Wi;//带权周转时间
}PCB;

typedef struct linktable{
    PCB *node;
    struct linktable *next;
}link;

int n;//pcb块的数目
void print(link* waitlink);
link* create_wait_link(void);
void finish_time(link *runlink);
void cycling_time(link *runlink);
void Wi_time(link *runlink);
void avgTurnaroundTime(link* runlink);
void avgTurnaroundTime_wi(link* runlink);

int main()
{
    link *waitline;
    link *runqueue;
    waitline = (link*)malloc(sizeof(link));
    runqueue = (link*)malloc(sizeof(link));
    waitline = create_wait_link();
    finish_time(waitline);
    cycling_time(waitline);
    Wi_time(waitline);
    avgTurnaroundTime(waitline);
    avgTurnaroundTime_wi(waitline);
    
    
    print(waitline);
}

link* create_wait_link(void)
{
    printf("请输入进程数：");
    scanf("%d",&n);
    PCB original[50];//最开始将进程输入到这个数组中
    link *runLink = NULL;//创建等待列表
    runLink = (link*)malloc(sizeof(link));
    for (int i=0; i<n; i++)
    {
        printf("请输入第%d个进程的相应信息。\n", i+1);
        printf("请输入进程名:");
        scanf("%s", original[i].proName);
        printf("请输入到达时间:");
        scanf("%d", &original[i].arriveTime);
        printf("请输入服务时间:");
        scanf("%d", &original[i].serviceTime);
    }
    PCB temp;
    for(int i=0;i<n; i++)
    {
        for(int j=i; j<n; j++)
        {
            if(original[j].arriveTime > original[j+1].arriveTime)
            {
                temp = original[j];
                original[j] = original[j+1];
                original[j+1] = temp;
            }
        }
    }
    //将数组中的元素组成列表
    runLink = (link*)malloc(sizeof(link));
    link *node_link = runLink;
    for (int i=0; i<n; i++)
    {
        link *temp = NULL;
        temp = (link*)malloc(sizeof(link));
        temp->node = original+i;
        temp->next = NULL;
        node_link->next = temp;
        node_link = node_link->next;
    }

    return runLink;
}

void finish_time(link *runlink)
{
    link *run = NULL;//第一个到达的进程
    run = runlink->next;
    run->node->finishTime = run->node->arriveTime + run->node->serviceTime;
    int temp = run->node->finishTime;//记录第一个进程的完成时间
    run = run->next;
    while (run != NULL)
    {
        if (temp < run->node->arriveTime)
        {
            run->node->finishTime = run->node->arriveTime + run->node->serviceTime;
            temp = run->node->finishTime;
            run = run->next;
        }
        else
        {
            run->node->finishTime = temp + run->node->serviceTime;
            temp = run->node->finishTime;
            run = run->next;
        }
    }
}

void cycling_time(link *runlink)//周转时间
{
    link *run = NULL;
    run = runlink->next;
    while (run != NULL)
    {
        run->node->cyclingTime = run->node->finishTime - run->node->arriveTime;
        run = run->next;
    }
    
    return;
}

void Wi_time(link *runlink)//带权周转时间
{
    link *run = NULL;
    run = runlink->next;
    while (run != NULL)
    {
        run->node->float_Wi = (double)run->node->cyclingTime / run->node->serviceTime;
        run = run->next;
    }
    return;
}


void avgTurnaroundTime(link* runlink)
{
    link *run = NULL;
    int n = 0;//记录进程数
    run = runlink->next;
    int sum_cycling = 0;
    while (run != NULL)
    {
        sum_cycling += run->node->cyclingTime;
        run = run->next;
        n++;
    }
    avg_TurnaroundTime = sum_cycling / n;
    return;
}

void avgTurnaroundTime_wi(link* runlink)
{
    link *run = NULL;
    int n = 0;//记录进程数
    run = runlink->next;
    double sum_cycling = 0;
    while (run != NULL)
    {
        sum_cycling += run->node->float_Wi;
        run = run->next;
        n++;
    }
    AQTT = sum_cycling / n;
    return;
}

void print(link* waitlink)
{
    waitlink = waitlink->next;
    printf("进程的相应信息。\n");
    printf("进程\t到达时间\t服务时间\t完成时间\t周转时间\t带权周转\n");
    do{
        printf("%s\t    %d\t\t    %d\t\t    %d\t\t   %d\t\t   %.002lf\t\t\n", waitlink->node->proName,waitlink->node->arriveTime,    waitlink->node->serviceTime,waitlink->node->finishTime,waitlink->node->cyclingTime,waitlink->node->float_Wi);
        
        waitlink = waitlink->next;
    }while (waitlink != NULL);
    printf("平均周转时间\t平均带权周转时间\n");
    printf("   %.002lf\t\t   %.002lf\n",avg_TurnaroundTime,AQTT);

}
