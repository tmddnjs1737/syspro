#include <stdio.h>
#include <unistd.h>
int main()
{ 
   int pid;
   printf("[%d] start \n", getpid());
   pid = fork();
   printf("[%d] return%d\n", getpid(), pid);
}

