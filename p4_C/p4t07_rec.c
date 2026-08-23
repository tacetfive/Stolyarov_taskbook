#include <stdio.h>

int space_counter(const char *str_ptr);
int counter = 0;

int main(int argc, const char **argv) {
	space_counter(argv[1]);
	printf("%d\n", counter);
	return 0;
}

int space_counter(const char *str_ptr) {
	if (!*str_ptr)
		return 0;
	if (*str_ptr == ' ')
		counter++;
	space_counter(str_ptr+1);
	return 0;
}

