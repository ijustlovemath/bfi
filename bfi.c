/* brainfuck interpreter */
#include <stdio.h>

#define WORKSPACE_SIZE (65535)

void die(const char *msg)
{
	puts(msg);
	int x = *(int *)0;
}

void do_fsm(const char *prgm, unsigned char **dp, unsigned char **pc)
{
#define print_byte(b_) do {\
  fprintf(stderr, #b_ ": '%c', %02x\n", b_, b_);\
  } while(0)

	int loop_level = 0;
	int working_byte = 0;
	char printer[2] = {0};
	print_byte(**pc);

	switch(**pc) {
	case '>':
		(*dp)++;
		break;
	case '<':
		(*dp)--;
		break;
	case '+':
		(**dp)++;
		break;
	case '-':
		(**dp)--;
		break;
	case '.':
		*printer = **dp;
        puts(printer);
		break;
	case ',':
		**dp = getchar();
		break;
	case '[':
		if(!**dp) {
			loop_level++;
			while(loop_level) {
				(*pc)++;
				working_byte = **pc;
				if(working_byte == '[')
					loop_level++;
				if(working_byte == ']')
					loop_level--;
			}
		}
		break;
	case ']':
		if(**dp) {
			loop_level++;
			while(loop_level) {
				(*pc)--;
				working_byte = **pc;
                print_byte(working_byte);
                fprintf(stderr, "loop_level: %d\n", loop_level);
				if(working_byte == ']') {
					loop_level++;
                }
				if(working_byte == '[') {
					loop_level--;
                }
			}
		}
		break;

	default:
		die("unrecognized instruction");
	}
}

void cpy(unsigned char *dst, unsigned char *src, unsigned int len)
{
	for(;len--; *dst++ = *src++);
}

#define OFST(p2, p1) ((unsigned long int) (p2) - (unsigned long int) (p1))

#define dbg(pc, dp, prgm) printf("pc %08x; dp %08x; *dp %02x '%c'\n", OFST(pc,prgm), OFST(dp,prgm), *dp, (char) 'x')

int main(void)
{
	unsigned char prgm [WORKSPACE_SIZE] = {0};
	unsigned char data[WORKSPACE_SIZE] = {0};
	unsigned char *dp = data;
	unsigned char *pc = prgm;

	cpy(prgm, ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.", 164);

	dbg(pc, dp, prgm);
	while(pc >= prgm && pc < prgm + WORKSPACE_SIZE && *pc) {
		do_fsm(prgm, &dp, &pc);
		dbg(pc, dp, prgm);

		pc++;
        dbg(pc, dp, prgm);
	}
}
