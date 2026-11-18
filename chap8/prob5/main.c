#include <stdio.h>
 #include <unistd.h>

 int main()
 {    
     printf("my number : [%d] \n", getpid());
     printf("parents number : [%d] \n", getppid());
 }

