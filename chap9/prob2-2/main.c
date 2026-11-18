#include <sys/types.h>	
   int main() 
   {
      int pid1, pid2, child, status;
	
      printf("[%d] parents start \n", getpid( ));
      pid1 = fork();
      if (pid1 == 0) {
	 printf("[%d] child1start \n", getpid( ));
	 sleep(1);
	 printf("[%d] child1end \n", getpid( ));
	 exit(1);
      }
pid2 = fork();
	 if (pid2 == 0) {
	     printf("[%d] child2start \n", getpid( ));
	     sleep(2);
	     printf("[%d] child2end", getpid( ));
	     exit(2);
	 }
	 child = waitpid(pid1, &status, 0); 
	 printf("[%d] child1 %d end \n", getpid( ), child);
	 printf("\t end code %d\n", status>>8);
	}

