/* brainfuck interpreter */
#include <stdio.h>

#define WORKSPACE_SIZE (65535)

void die(const char *msg)
{
	puts(msg);
	int x = *(int *)0;
}

/**
 * dp is a pointer to the data pointer; that way we can manipulate the data
 * pointer itself, and not just the data it points to. This allows us to
 * easily implement both '>' and '+' instructions.
 * pc is similar, it's a pointer to the program counter, which we use to index
 * into the program text itself.
 */
void do_fsm(unsigned char **dp, const char **pc)
{
#define print_byte(b_) do {\
  fprintf(stderr, #b_ ": '%c', %02x\n", b_, b_);\
  } while(0)

/**
 * starting at program counter, look in direction for matching
 * branch character, accounting for nested loops.
 * needs to have a check for overrunning past program text, in case of
 * unmatched braces
 * This is not quite as efficient as doing a single pass to note down
 * locations of all open braces and their matching closing brace, but
 * it's quick to write and this interpreter is meant to be simple, not fast.
 */
#define BRANCH_CASE(branch_char, matched_char, direction, dp_negation) \
    case branch_char:\
        if(dp_negation **dp) {\
            loop_level++; \
            while(loop_level) {\
                (*pc) += direction;\
                working_byte = **pc;\
                if(working_byte == branch_char)\
                    loop_level++;\
                if(working_byte == matched_char)\
                    loop_level--;\
            }\
        }\
        break;

	int loop_level = 0;
	int working_byte = 0;

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
        putchar(**dp);
		break;
	case ',':
		**dp = getchar();
		break;
    BRANCH_CASE('[', ']',  1,  !)
    BRANCH_CASE(']', '[', -1, !!)
	default:;
	}
}

void cpy(unsigned char *dst, unsigned char *src, unsigned int len)
{
	for(;len--; *dst++ = *src++);
}

#define OFST(p2, p1) ((unsigned long int) (p2) - (unsigned long int) (p1))

#define dbg(pc, dp, prgm) printf("pc %08x; dp %08x; *dp %02x '%c'\n", OFST(pc,prgm), OFST(dp,prgm), *dp, (char) 'x')

#undef dbg
#define dbg(...)

#define BOUNDED_BY(start, ptr, end) ((ptr >= start) && (ptr < end))

#define WARN_UNBOUNDED(start, ptr, end) do {\
if(!BOUNDED_BY(start, ptr, end)) puts("[WARNING] " #ptr " has stepped outside its allowed bounds!");\
   } while(0)

void run(const char *prgm)
{
	unsigned char data[WORKSPACE_SIZE] = {0};
	unsigned char *dp = data;
	const char *pc = prgm;

	dbg(pc, dp, prgm);
	while(BOUNDED_BY(prgm, pc, prgm + WORKSPACE_SIZE) && BOUNDED_BY(data, dp, data + WORKSPACE_SIZE) && *pc) {
		do_fsm(&dp, &pc);
		dbg(pc, dp, prgm);
		pc++;
	}
    WARN_UNBOUNDED(prgm, pc, prgm+WORKSPACE_SIZE);
    WARN_UNBOUNDED(data, dp, data+WORKSPACE_SIZE);
}

void xcpy(char *dest, const char *src)
{
    unsigned int len = 0;
    for(; len < WORKSPACE_SIZE && *src; *dest = *src, src++, dest++, len++);
    // zero out the rest to prevent multiple programs from cluttering the program buffer
    for(; len < WORKSPACE_SIZE; *dest = 0, dest++, len++);
}

int main(void)
{
	char prgm [WORKSPACE_SIZE] = {0};

	// Hello, World!
    xcpy(prgm, ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.");

	// Game of Life
	xcpy(prgm, ">>>->+>+++++>(++++++++++)[[>>>+<<<-]>+++++>+>>+[<<+>>>>>+<<<-]<-]>>>>[\n"
"  [>>>+>+<<<<-]+++>>+[<+>>>+>+<<<-]>>[>[[>>>+<<<-]<]<<++>+>>>>>>-]<-\n"
"]+++>+>[[-]<+<[>+++++++++++++++++<-]<+]>>[\n"
"  [+++++++++.-------->>>]+[-<<<]>>>[>>,----------[>]<]<<[\n"
"    <<<[\n"
"      >--[<->>+>-<<-]<[[>>>]+>-[+>>+>-]+[<<<]<-]>++>[<+>-]\n"
"      >[[>>>]+[<<<]>>>-]+[->>>]<-[++>]>[------<]>+++[<<<]>\n"
"    ]<\n"
"  ]>[\n"
"    -[+>>+>-]+>>+>>>+>[<<<]>->+>[\n"
"      >[->+>+++>>++[>>>]+++<<<++<<<++[>>>]>>>]<<<[>[>>>]+>>>]\n"
"      <<<<<<<[<<++<+[-<<<+]->++>>>++>>>++<<<<]<<<+[-<<<+]+>->>->>\n"
"    ]<<+<<+<<<+<<-[+<+<<-]+<+[\n"
"      ->+>[-<-<<[<<<]>[>>[>>>]<<+<[<<<]>-]]\n"
"      <[<[<[<<<]>+>>[>>>]<<-]<[<<<]]>>>->>>[>>>]+>\n"
"    ]>+[-<<[-]<]-[\n"
"      [>>>]<[<<[<<<]>>>>>+>[>>>]<-]>>>[>[>>>]<<<<+>[<<<]>>-]>\n"
"    ]<<<<<<[---<-----[-[-[<->>+++<+++++++[-]]]]<+<+]>\n"
"  ]>>\n"
"]");	
    // Sierpinski
    //xcpy(prgm, "++++++++[>+>++++<<-]>++>>+<[-[>>+<<-]+>>]>+[-<<<[->[+[-]+>++>>>-<<]<[<]>>++++++[<<+++++>>-]+<<++.[-]<<]>.>+[>>]>+]");

	//xcpy(prgm, ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.");
    //run(prgm);

    // For now, just run a simple "Hello, World!" program every time.
	run(">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.");
    run("[[["); // should segfault
    return 0;
}
