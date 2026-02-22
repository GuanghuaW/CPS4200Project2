/*
 * Course: CPS-4200
 *
 * File: ksh.c
 * Description: a new re-implemented shell program
 * 
*/

#include "types.h"
#include "fcntl.h"
#include "user.h"

#define SH_PROMPT "ksh> "
#define NULL (void *)0
#define MAXLINE 256

char *strtok(char *s, const char *delim);
int process_one_cmd(char *);


int main(int argc, char *argv[])
{
    char buf[MAXLINE];  // buffer to hold input command
    int i;
    int n;

    printf(1, SH_PROMPT);  // print prompt ksh>

    while((n = read(0, buf, MAXLINE)) != 0)  // read input from stdin (keyboard)
    {
        if(n == 1)  // no input at all, we should skip
        {
            printf(1, SH_PROMPT);
            continue;
        }

        buf[i = (strlen(buf) - 1)] = 0;  // replace newline with null terminator

        process_one_cmd(buf);  // process the entered command
        
        printf(1, SH_PROMPT);
      
        memset(buf, 0, sizeof(buf));  // clear buffer for next command
    }
    
    exit();
}

int exit_check(char **tok, int num_tok)
{
    if(num_tok == 1)
    {
        if(strlen(tok[0]) == 4
            && (tok[0][0] == 'e')
            && (tok[0][1] == 'x')
            && (tok[0][2] == 'i')
            && (tok[0][3] == 't'))
        {
            return 1;
        }
    }
    return 0;
}

int process_normal(char **tok, int bg)
{
    // TODO: task 1, 2 ,3
    return 0;
}

int process_one_cmd(char* buf)
{
    int i, num_tok;
    char **tok;
    int bg;
    i = (strlen(buf) - 1);
    num_tok = 1;

    while(i)  // count the number of tokens(words) in the command
    {
        if(buf[i--] == ' ')  // based on the number of 'space'
        {
            num_tok++;
        }
    }

    // allocate memory for storing tokenized words
    if((tok = malloc((num_tok + 1) * sizeof(char *))) == 0) 
    {
        printf(1, "malloc failed\n");
        exit();
    }

    i = bg = 0;
    //TODO: task 5, 6

    tok[i++] = strtok(buf, " ");  // tokenize first word

    // e.g., input: echo hello world
    // tok[0]: "echo"
    // tok[1]: "hello"
    // tok[2]: "world"
    // tok[3]: "NULL" (to indicate the end)

    // check special symbols
    while((tok[i] = strtok(NULL, " "))) 
    {
        switch(*tok[i])  // like if-else
        {
            case '&':  // if symbol '&'(background) is found in tok
                bg = i;
                tok[i] = NULL;
                break;
            // TODO: task 5, 6

            default:  // all other no special symbol cases
                // do nothing
                break;
        }

        i++;
    }

    // check if the input cmd is 'exit'
    if(exit_check(tok, num_tok))
    {
        // if yes
	    // TODO: task 4
        exit();
    }
    
    //TODO: task 5, 6

    // to process a normal command (foreground or background)
    process_normal(tok, bg);

    free(tok);
    return 0;
}

/*
 * do NOT touch this function
 *
 * tokenize a string by breaking it into smaller substrings
 * based on delimiter
*/
char *strtok(s, delim)
    register char *s;
    register const char *delim;
{
    register char *spanp;
    register int c, sc;
    char *tok;
    static char *last;


    if(s == NULL && (s = last) == NULL)
    {
        return (NULL);
    }

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for(spanp = (char *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if(c == 0) {        // no non-delimiter characters
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for(;;)
    {
        c = *s++;
        spanp = (char *)delim;
        do {
            if((sc = *spanp++) == c)
            {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}
