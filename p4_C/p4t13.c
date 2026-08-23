// Print Nth to Mth digits of infinite sequense 
// formed by natural numbers squares

#include <stdio.h>
#define uint unsigned int
#define ull unsigned long long

int convert_number(char *);

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Input error. Two natural numbers expected\n");
		return 1;
	}
// check: not charactes, not negative, init <= last
    char *init_str = argv[1];
	char *last_str = argv[2];
	int init = convert_number(init_str);
	int last = convert_number(last_str);
//	uint substr_len = last - init;
	ull number = 1;
	uint ch_cnt = 0;
	for(number; (ch_cnt < last); number++) {
		char str[20]; // maximum ull length in decimal
		ull square = number * number;
		int strln = sprintf(str, "%llu", square);
		int str_end;
		int ch_cnt_prev = ch_cnt;
		ch_cnt += strln;
		if (ch_cnt >= init) {
			int str_start_v = (init - ch_cnt_prev - 1); 
			int str_start = (str_start_v < 0) ? 0 : str_start_v;  
			if (ch_cnt >= last) {
				str_end = last - ch_cnt_prev - 1;
			}
			else {
				str_end = strln;
			}
			for(str_start; (str_end >= str_start); str_start++) {
				putchar(str[str_start]);
			}
			init = ch_cnt_prev;
		}
	}	
	putchar('\n');
return 0;
}

int convert_number (char *n_str) {
	char *endptr;
	uint n = strtoul(n_str, &endptr, 10);
	return n;
}

