#include <sys/types.h> 
int main() 
{
   int pid, child, status;
   printf("[%d] parents\n", getpid( ));
   pid = fork();
   if (pid == 0) {
      printf("[%d] child \n", getpid( ));
      exit(1);
   }
   child = wait(&status);
   printf("[%d] child end\n", getpid(), child);
   printf("\t end code %d\n", status>>8);
}

