#include <stdio.h>
#include <ctype.h>

void remove_spaces(char *str_ptr)
{
    char *ptr;
    char *last_nonspace = ptr;
    for(ptr = str_ptr; *ptr; ptr++) {
        if(isspace(*ptr)) {
            for(ptr++; isspace(*ptr) ; ptr++) {}; /* pass repetitive spaces */
            if(!isspace(*last_nonspace)) /* false only if 1st char is space */
                last_nonspace++;
            *last_nonspace = *ptr;
        }
        else {
            if(last_nonspace != ptr) {
                last_nonspace++;
                if(last_nonspace != ptr)
                    *last_nonspace = *ptr;
            }
        }
    }
    if(last_nonspace != str_ptr) {
        for(last_nonspace++; (last_nonspace < ptr); last_nonspace++)
            *last_nonspace = 0;
    }
}

int main(int argc, char **argv)
{
    char *str_ptr = argv[1];
    printf("%s\n", str_ptr);
    remove_spaces(str_ptr);
    printf("%s\n", str_ptr);
    return 0;
}
