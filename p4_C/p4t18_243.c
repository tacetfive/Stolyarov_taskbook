/* program works non correct. Rewrite */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct word {
    char letter;
    struct word *next;
};

struct phrase {
    struct word *word_ptr;
    struct phrase *next;
};

struct phrase *list();
void print_out(struct phrase *);

int main(int argc, char **argv)
{
    struct phrase *ph_ptr = list();
    print_out(ph_ptr);
    return 0;
}

void print_out(struct phrase *ph_ptr)
{
    int is_end = 1; // = 0 when longest word ends up
    struct phrase *tmp;
    struct word *word_ptr_del;
    char c;
    while(is_end) {
        for(tmp = ph_ptr; tmp; tmp = tmp->next) {
            if(tmp->word_ptr) {
                c = tmp->word_ptr->letter;
                putchar(c);
            }
            else
                putchar(' ');
        }
        putchar('\n');
        is_end = 0;
        for(tmp = ph_ptr; tmp; tmp = tmp->next) {
            if(tmp->word_ptr) {
                word_ptr_del = tmp->word_ptr;
                tmp->word_ptr = tmp->word_ptr->next;
                free(word_ptr_del);
                if(tmp->word_ptr)
                    is_end |= 1;
                else
                    is_end |= 0;
            }
        }
    }        
}


struct phrase *list()
{
    struct phrase *head = NULL, *tail = NULL, *tmp;
    struct word *head_word = NULL, *tail_word = NULL, *tmp_word;
    char c;
    while((c = getchar()) != EOF) {
        if (!isspace(c)) { // if not space
            tmp = malloc(sizeof(struct phrase));
            while(!isspace(c)) {
                tmp_word = malloc(sizeof(struct word));
                tmp_word->letter = c;
                tmp_word->next = NULL; // it's anyway NULL, aren't?
                if(tail_word)
                    tail_word->next = tmp_word;
                else
                    head_word = tmp_word;
                tail_word = tmp_word;
                c = getchar();
            }
            tail_word = NULL;
            tmp->word_ptr = head_word;
            tmp->next = NULL;
            if(tail)
                tail->next = tmp;
            else
                head = tmp;
            tail = tmp;
        }
    }
    return head;
}
