#include <stdlib.h>
#include <fcntl.h> /* contains open() syscall */
#include <stdio.h> /* contains fputc etc */
#include <unistd.h> /* contains write() syscall */
/* #include <errno.h> */
/* #include <string.h> */


void write_out_files(FILE *input_file, FILE *output_file_spaces, 
                     int output_file_int_fd)
{
    int flag = 0;
    int char_counter = 0;
    char prev = 0;
    char curr;
    while( (curr = fgetc(input_file)) != EOF ) {
        char_counter++;
        if ( curr == ' ' && ( prev == '\n' || !prev ) ) flag = 1;
        if (flag) fputc(curr, output_file_spaces);
        if ( curr == '\n' ) {
            if (write(output_file_int_fd, &char_counter, sizeof(int)) == -1) {
                perror("write error");
           /* This error message generator also works, but requires <string.h>
              and <errno.h>:
                fprintf(stderr, "write error: %s, (Error code %d)\n",
                        strerror(errno), errno);  */
                return;
            }
            flag = 0;
            char_counter = 0;
        }
        prev = curr;
    }
}

int main(int argc, char **argv)
{
    FILE *input_file, *output_file_spaces; 
    int output_file_int_fd;
    if (argc != 4) { 
        fprintf(stderr, "Incorrect parameters.\n");
        return 1;
    }
    input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror(argv[1]); /* print message to error stream */
        exit(1);
    }
    output_file_spaces = fopen(argv[2], "w");
    /* creat() is eq to open() with O_CREAT|O_WRONLY|O_TRUNC */
    output_file_int_fd = creat(argv[3], S_IRWXU); 
    write_out_files(input_file, output_file_spaces, output_file_int_fd);
    fclose(input_file);
    fclose(output_file_spaces);
    close(output_file_int_fd);
    return 0;
}
