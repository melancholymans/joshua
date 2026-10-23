#include "unity.h"
#include "../shogi/usi.h"
#include "test_usi.h"

void test_shorten_space(void) {
	char buf[64] = "Hello    world     space";
	shorten_space(buf);
	TEST_ASSERT_EQUAL_STRING("Hello world space", buf);
}

void test_command_queue(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	char moves[8];
	new_board(&hd);
	strcpy_s(sfen, 63 + 1, "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1");
	set_board(&hd, sfen);
	init_queue(&hd, 8);
	comman_queue_t* u = hd.ucq;
	push_queue(u, "first");
	push_queue(u, "second");
	push_queue(u, "third");
	push_queue(u, "fourth");
	push_queue(u, "fifth");
	push_queue(u, "sixth");
	push_queue(u, "seventh");
	push_queue(u, "eighth");
	char* str = pop_queue(u);
	printf("%s\n",str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	printf("%s\n", str);
	free(str);

	push_queue(u, "ninth");
	printf("%s\n", pop_queue(u));
	free_queue(u);
}