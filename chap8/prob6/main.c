#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

int main()
{ 
   printf("really ID : %d(%s) \n", getuid(), getpwuid(getuid())->pw_name);
   printf("user ID : %d(%s) \n", geteuid(), getpwuid(geteuid())->pw_name);
   printf("my really ID : %d(%s) \n", getgid(), getgrgid(getgid())->gr_name);
   printf("my user ID : %d(%s) \n", getegid(), getgrgid(getegid())->gr_name);
}

