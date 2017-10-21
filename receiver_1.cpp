/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: osboxes
 *
 * Created on 04 October 2017, 09:57
 */

#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#define MAX 100000
#define SAMPLE 50

using namespace std;

/*
 * 
 */
void atomic_action(char *mem,unsigned char incr)
{
    int64_t val=-1;
    asm volatile("lock; xadd %0,%1"
                 :"=r"(val), "=m"(*mem)
                 :"0"(incr),"m"(*mem)
                 :"memory");
    
}

uint64_t rdtsc()
    {
        uint64_t a,d;
        asm volatile("mfence");
        asm volatile("rdtsc":"=a"(a),"=d"(d));
        a=(d<<32)|a;
        asm volatile("mfence");
        return a;
    }



char* allocate_mem()
{
    char *ptr=(char*)aligned_alloc(4*1024,4*1024*1024);
    //char *ptr=(char*)malloc((4*1024+1)*8);
    char *unaligned_addr=ptr+2;
    return unaligned_addr;
}

int get_atomic_num(char *mem)
{
    int time=MAX;
    int num=0;
    unsigned char incr=0x01;
        while(time>0)
        {
            uint64_t start=rdtsc();
            atomic_action(mem++,incr);
            //mem++;
            int last=(int)(rdtsc()-start);
            //printf("%" PRIu64 "\n",last);
            time=time-last;
            num++;
        }
        return num;
}

float sample(char *mem)
{   int i,num;
    int sum=0;
    float mean=0;
    int min=0; int max=0;
    for(i=0;i<SAMPLE;i++)
    {
        num=get_atomic_num(mem);
        printf("Num:%d\n",num);
        sleep(0.5);
        if(num>max)
            max=num;
        if(num<300)
        {min++;
        sum+=num;}
        //printf("Sum:%d\n",sum);
    }
    
    mean=float(sum-max)/(min-1);
    return mean;
}

int main(int argc, char** argv) {
    char *ptr=allocate_mem();
    float threshold=sample(ptr);
    printf("Mean:%f\n",threshold);
    int num=0;
    int i=0;
    unsigned char incr=0x01;
    sleep(3);
    while(1)
    {num=get_atomic_num(ptr);
     printf("Num:%d",num);
     if(num<(threshold-0))
     {printf("Bit detected ----------------->(1)\n");
      sleep(1);}
     else
     {printf("Bit detected ----------------->(0)\n");
     sleep(1);}
    }
    
    /*for(num;rest>0;num++)
    {
        uint64_t start=rdtsc();
        atomic_action(ptr++);
        rest=rest-(rdtsc()-start);
    }*/
    //printf("%d\n",num);
    return num;
}

