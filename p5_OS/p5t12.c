#include <unistd.h> /* execvp() */
#include <stdio.h> /* stderr */
#include <stdlib.h> /* exit() */
#include <sys/wait.h> /* waitpid */

int main(int argc, char **argv)
{
    char **argv_new = argv + 1;
    int w_status;
    pid_t p, w;
    p = fork();
    if (p == 0) {
        execvp(argv[1], argv_new);
        perror(argv[1]);
        fflush(stderr);
        _exit(1);
    } else {
        while(1) {
            w = waitpid(-1, &w_status, WUNTRACED | WCONTINUED);
            if ( w == -1 ) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if (w == p) {
                if (WIFEXITED(w_status)) {
                    printf("process %d exited with code %d\n", p, 
                                WEXITSTATUS(w_status));
                } else {
                    printf("process %d killed by signal %d\n", p, 
                                WTERMSIG(w_status));
                }
                break;
            }
        }
        exit(EXIT_SUCCESS);
    }
}




