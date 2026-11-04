#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv[]) {
    int opt;
    
    while ((opt = getopt(argc, argv, "e::ugip")) != -1) {
        switch (opt) {
            case 'e':
                if (optarg != NULL) {
                    char *val = getenv(optarg);
                    if (val != NULL) {
                        printf("%s = %s\n", optarg, val);
                    }
                } 
                else if (optind < argc && argv[optind][0] != '-') {
                    char *val = getenv(argv[optind]);
                    if (val != NULL) {
                        printf("%s = %s\n", argv[optind], val);
                    }
                    optind++;
                } 
                else {
                    for (char **env = environ; *env != NULL; env++) {
                        printf("%s\n", *env);
                    }
                }
                break;

            case 'u': {
                uid_t ruid = getuid();
                uid_t euid = geteuid();
                struct passwd *pw_r = getpwuid(ruid);
                struct passwd *pw_e = getpwuid(euid);

                printf("Real User ID : %d(%s)\n", ruid, pw_r ? pw_r->pw_name : "unknown");
                printf("Valid User ID : %d(%s)\n", euid, pw_e ? pw_e->pw_name : "unknown");
                break;
            }

            case 'g': {
                gid_t rgid = getgid();
                gid_t egid = getegid();
                struct group *gr_r = getgrgid(rgid);
                struct group *gr_e = getgrgid(egid);

                printf("My Realistic Group ID : %d(%s)\n", rgid, gr_r ? gr_r->gr_name : "unknown");
                printf("My Valid Group ID : %d(%s)\n", egid, gr_e ? gr_e->gr_name : "unknown");
                break;
            }

            case 'i':
                printf("My Process ID : [%d]\n", getpid());
                break;

            case 'p':
                printf("My Parent Process ID : [%d]\n", getppid());
                break;

            default:
                fprintf(stderr, "Usage: %s [-e [env_name]] [-u] [-g] [-i] [-p]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}
