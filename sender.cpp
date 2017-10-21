/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sender.cpp
 * Author: osboxes
 *
 * Created on 08 September 2017, 09:49
 */

#include <cstdlib>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#define N 1000
#define SIZE 6*1024*1024
#define THRESHOLD 999

using namespace std;

/*
 * 

 *  */

typedef struct ReturnV{
int* array;
int size;}rv;

void atomic_action(char *mem)
{
    int32_t val=-1;
    asm volatile("lock; xadd %0,%1"
                 :"=r"(val), "=m"(*mem)
                 :"0"(val),"m"(*mem)
                 :"memory","cc");
    
}
char * allocate_aligned(size_t size, size_t alignment)
{
  const size_t mask = alignment - 1;
  const uintptr_t mem = (uintptr_t) malloc(size + alignment);
  return (char *) ((mem + mask) & ~mask);
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

rv get_surpass(char *ptr)
{
    int *add=new int[SIZE/20];
    int j=0;
    for(int i=0; i<SIZE; i++)
    {uint64_t time=rdtsc();
    atomic_action(ptr++);
    uint64_t last=rdtsc()-time;
    if(last> THRESHOLD)
    { add[j++]=i;
     //printf("%p\n",ptr+i);
    }    
    }
    printf("%d\n",j);
    rv val;
    val.array=add;
    val.size=j-1;
    return val;
}

int main(int argc, char** argv) {
    char *ptr;
    int list[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int i;
    //ptr=allocate_mem_1();
    //ptr=allocate_mem_2();
    size_t size=SIZE;
    size_t alignment=4*1024;
     ptr=allocate_aligned(size,alignment);
     rv val=get_surpass(ptr);
     int* add=val.array;
     int j=val.size;
     
     /*for(i=0;i<j;i++)
         printf("%d\n",add[i]);*/
     for(i=0;i<(sizeof(list)/sizeof(list[0]));i++)
     {if(list[i]==1)
      {
        for(i=0;i<j;i++)
            printf("Doing atomic operation... omitting bit----------->(1)\n");
            atomic_action(ptr+add[i]);}
     else
     {sleep(0.5);
     printf("System sleeps...   omitting bit----------->(0)\n");
     }}
         

    return 0;
}

