#include "unity.h"
#include "../shogi/usi.h"
#include "../shogi/position.h"
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
	//printf("%s\n",str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	str = pop_queue(u);
	//printf("%s\n", str);
	free(str);
	push_queue(u, "ninth");
	//printf("%s\n", pop_queue(u));
	free_queue(u);
}

void test_handle(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char buf[2048];
	new_board(&hd);

	//pattern 1
	sprintf_s(buf, 2048, "position startpos");
	handle(&hd, buf);
	for (int pt = 0; pt < 25; pt++) {
		TEST_ASSERT_EQUAL_INT(0, hd.bd->hb[pt]);
	}
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(wking, hd.bd->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq1a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9b]);
	TEST_ASSERT_EQUAL_INT(wrook, hd.bd->mb[sq8b]);
	for (int sq = sq7b; sq >= sq3b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wbishop, hd.bd->mb[sq2b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1b]);
	for (int sq = sq9c; sq >= sq1c; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq]);
	}
	for (int sq = sq9d; sq >= sq1d; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9g; sq >= sq1g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(bbishop, hd.bd->mb[sq8h]);
	for (int sq = sq7h; sq >= sq3h; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(brook, hd.bd->mb[sq2h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1h]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, hd.bd->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq1i]);
	
	//pattern 2(“š‚¦‚Ípattern 1‚Æ“¯‚¶)
	sprintf_s(buf, 2048, "position sfen lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1");
	for (int pt = 0; pt < 25; pt++) {
		TEST_ASSERT_EQUAL_INT(0, hd.bd->hb[pt]);
	}
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(wking, hd.bd->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq1a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9b]);
	TEST_ASSERT_EQUAL_INT(wrook, hd.bd->mb[sq8b]);
	for (int sq = sq7b; sq >= sq3b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wbishop, hd.bd->mb[sq2b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1b]);
	for (int sq = sq9c; sq >= sq1c; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq]);
	}
	for (int sq = sq9d; sq >= sq1d; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	for (int sq = sq9g; sq >= sq1g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(bbishop, hd.bd->mb[sq8h]);
	for (int sq = sq7h; sq >= sq3h; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(brook, hd.bd->mb[sq2h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1h]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, hd.bd->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq1i]);

	//pattern 3 
	sprintf_s(buf, 2048, "position startpos moves 2g2f 4a3b 2f2e 6a5b 7g7f 5a4b 6i7h 7a6b 7i6h 9c9d 8h7g 8a9c 2e2d 2c2d 2h2d 9c8e P*2c 8e7g+ 8i7g 9d9e 3i3h 8b9b 2c2b+ 3a2b 2d2f 2b2c B*5f P*2e 2f2e P*2d 2e8e B*2h 8e8c+ 2h1i+ 8c8a L*2e 8a2a 2e2i+ 5f2c+ 3b2c S*4a N*3a 2a3b 4b5a 3b5b");
	handle(&hd, buf);
	TEST_ASSERT_EQUAL_INT(2, hd.bd->hb[bpawn]);
	TEST_ASSERT_EQUAL_INT(2, hd.bd->hb[bknight]);
	TEST_ASSERT_EQUAL_INT(1, hd.bd->hb[bgold]);
	TEST_ASSERT_EQUAL_INT(1, hd.bd->hb[wbishop]);
	
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(wking, hd.bd->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq1a]);
	TEST_ASSERT_EQUAL_INT(wrook, hd.bd->mb[sq9b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7b]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq6b]);
	TEST_ASSERT_EQUAL_INT(bdragon, hd.bd->mb[sq5b]);
	for (int sq = sq4b; sq >= sq1b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9c]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8c]);
	for (int sq = sq7c; sq >= sq3c; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq2c]);
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq1c]);
	for (int sq = sq9d; sq >= sq3d; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq2d]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1d]);
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq9e]);
	for (int sq = sq8e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9f]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8f]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq7f]);
	for (int sq = sq6f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq9g]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq8g]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq7g]);
	for (int sq = sq6g; sq >= sq3g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2g]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq1g]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8h]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq7h]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq6h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq5h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq4h]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq3h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1h]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, hd.bd->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(wprolance, hd.bd->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(whorse, hd.bd->mb[sq1i]);

	//pattern 4 (“š‚¦‚Ípattern 3‚Æ“¯‚¶)
	sprintf_s(buf, 2048, "position sfen lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1 moves 2g2f 4a3b 2f2e 6a5b 7g7f 5a4b 6i7h 7a6b 7i6h 9c9d 8h7g 8a9c 2e2d 2c2d 2h2d 9c8e P*2c 8e7g+ 8i7g 9d9e 3i3h 8b9b 2c2b+ 3a2b 2d2f 2b2c B*5f P*2e 2f2e P*2d 2e8e B*2h 8e8c+ 2h1i+ 8c8a L*2e 8a2a 2e2i+ 5f2c+ 3b2c S*4a N*3a 2a3b 4b5a 3b5b");
	TEST_ASSERT_EQUAL_INT(2, hd.bd->hb[bpawn]);
	TEST_ASSERT_EQUAL_INT(2, hd.bd->hb[bknight]);
	TEST_ASSERT_EQUAL_INT(1, hd.bd->hb[bgold]);
	TEST_ASSERT_EQUAL_INT(1, hd.bd->hb[wbishop]);

	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(wking, hd.bd->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wknight, hd.bd->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, hd.bd->mb[sq1a]);
	TEST_ASSERT_EQUAL_INT(wrook, hd.bd->mb[sq9b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8b]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7b]);
	TEST_ASSERT_EQUAL_INT(wsilver, hd.bd->mb[sq6b]);
	TEST_ASSERT_EQUAL_INT(bdragon, hd.bd->mb[sq5b]);
	for (int sq = sq4b; sq >= sq1b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9c]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8c]);
	for (int sq = sq7c; sq >= sq3c; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wgold, hd.bd->mb[sq2c]);
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq1c]);
	for (int sq = sq9d; sq >= sq3d; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq2d]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1d]);
	TEST_ASSERT_EQUAL_INT(wpawn, hd.bd->mb[sq9e]);
	for (int sq = sq8e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9f]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8f]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq7f]);
	for (int sq = sq6f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq9g]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq8g]);
	TEST_ASSERT_EQUAL_INT(bknight, hd.bd->mb[sq7g]);
	for (int sq = sq6g; sq >= sq3g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2g]);
	TEST_ASSERT_EQUAL_INT(bpawn, hd.bd->mb[sq1g]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8h]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq7h]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq6h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq5h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq4h]);
	TEST_ASSERT_EQUAL_INT(bsilver, hd.bd->mb[sq3h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq2h]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq1h]);
	TEST_ASSERT_EQUAL_INT(blance, hd.bd->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, hd.bd->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, hd.bd->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(empty, hd.bd->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(wprolance, hd.bd->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(whorse, hd.bd->mb[sq1i]);
}


