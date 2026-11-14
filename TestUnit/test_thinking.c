#include "unity.h"

#include "../shogi/position.h"
#include "../shogi/thinking.h"
#include "../shogi/stb_ds.h"
#include "../shogi/usi.h"
#include "test_thinking.h"

void test_attacks_from(void) {
	/*
	   9  8  7  6  5  4  3  2  1
	a  l  n  s  g  k  g  s  n  l
	b  .  r  .  .  .  .  .  b  .
	c  p  p  p  p  p  p  p  p  p
	d  .  .  .  .  .  .  .  .  .
	e  .  .  .  .  .  B  .  .  P
	f  .  .  .  .  .  .  R  .  .
	g  P  P  P  P  P  P  P  P  .
	h  L  .  .  .  .  .  .  .  .
	i  .  N  S  G  K  G  S  N  L
	*/
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	new_board(&hd);
	strcpy_s(sfen, 128, "lnsgkgsnl/1r5b1/ppppppppp/9/5B2P/6R2/PPPPPPPP1/L8/1NSGKGSNL b - 1");
	set_board(&hd, sfen);

	int* ml = NULL;
	int pc = bpawn;
	for (int sq = sq9g; sq >= sq1g; sq -= 9) {
		ml = attacks_from(&hd, pc, sq, black);
		TEST_ASSERT_EQUAL_INT(1, arrlen(ml));
		TEST_ASSERT_EQUAL_INT(sq - 1, ml[0]);
		arrfree(ml);
	}
	pc = blance;
	ml = attacks_from(&hd, pc, sq1i, black);
	TEST_ASSERT_EQUAL_INT(3, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq1h, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq1g, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq1f, ml[2]);
	arrfree(ml);
	pc = bknight;
	ml = attacks_from(&hd, pc, sq2i, black);
	TEST_ASSERT_EQUAL_INT(2, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq3g, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq1g, ml[1]);
	arrfree(ml);

	pc = bsilver;
	ml = attacks_from(&hd, pc, sq3i, black);
	TEST_ASSERT_EQUAL_INT(3, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq4h, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq3h, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq2h, ml[2]);
	arrfree(ml);
	pc = bbishop;
	ml = attacks_from(&hd, pc, sq4e, black);
	TEST_ASSERT_EQUAL_INT(5, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq5d, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq6c, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq3d, ml[2]);
	TEST_ASSERT_EQUAL_INT(sq2c, ml[3]);
	TEST_ASSERT_EQUAL_INT(sq5f, ml[4]);
	arrfree(ml);
	pc = brook;
	ml = attacks_from(&hd, pc, sq3f, black);
	TEST_ASSERT_EQUAL_INT(11, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq3e, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq3d, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq3c, ml[2]);
	TEST_ASSERT_EQUAL_INT(sq4f, ml[3]);
	TEST_ASSERT_EQUAL_INT(sq5f, ml[4]);
	TEST_ASSERT_EQUAL_INT(sq6f, ml[5]);
	TEST_ASSERT_EQUAL_INT(sq7f, ml[6]);
	TEST_ASSERT_EQUAL_INT(sq8f, ml[7]);
	TEST_ASSERT_EQUAL_INT(sq9f, ml[8]);
	TEST_ASSERT_EQUAL_INT(sq2f, ml[9]);
	TEST_ASSERT_EQUAL_INT(sq1f, ml[10]);
	arrfree(ml);
	pc = bgold;
	ml = attacks_from(&hd, pc, sq4i, black);
	TEST_ASSERT_EQUAL_INT(3, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq5h, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq4h, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq3h, ml[2]);
	arrfree(ml);
	pc = bking;
	ml = attacks_from(&hd, pc, sq5i, black);
	TEST_ASSERT_EQUAL_INT(5, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq6h, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq5h, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq4h, ml[2]);
	TEST_ASSERT_EQUAL_INT(sq6i, ml[3]);
	TEST_ASSERT_EQUAL_INT(sq4i, ml[4]);
	arrfree(ml);
	pc = bhorse;
	ml = attacks_from(&hd, pc, sq4e, black);
	TEST_ASSERT_EQUAL_INT(9, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq5d, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq6c, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq3d, ml[2]);
	TEST_ASSERT_EQUAL_INT(sq2c, ml[3]);
	TEST_ASSERT_EQUAL_INT(sq5f, ml[4]);
	TEST_ASSERT_EQUAL_INT(sq4d, ml[5]);
	TEST_ASSERT_EQUAL_INT(sq5e, ml[6]);
	TEST_ASSERT_EQUAL_INT(sq3e, ml[7]);
	TEST_ASSERT_EQUAL_INT(sq4f, ml[8]);
	arrfree(ml);
	pc = bdragon;
	ml = attacks_from(&hd, pc, sq3f, black);
	TEST_ASSERT_EQUAL_INT(15, arrlen(ml));
	TEST_ASSERT_EQUAL_INT(sq3e, ml[0]);
	TEST_ASSERT_EQUAL_INT(sq3d, ml[1]);
	TEST_ASSERT_EQUAL_INT(sq3c, ml[2]);
	TEST_ASSERT_EQUAL_INT(sq4f, ml[3]);
	TEST_ASSERT_EQUAL_INT(sq5f, ml[4]);
	TEST_ASSERT_EQUAL_INT(sq6f, ml[5]);
	TEST_ASSERT_EQUAL_INT(sq7f, ml[6]);
	TEST_ASSERT_EQUAL_INT(sq8f, ml[7]);
	TEST_ASSERT_EQUAL_INT(sq9f, ml[8]);
	TEST_ASSERT_EQUAL_INT(sq2f, ml[9]);
	TEST_ASSERT_EQUAL_INT(sq1f, ml[10]);
	TEST_ASSERT_EQUAL_INT(sq4e, ml[11]);
	TEST_ASSERT_EQUAL_INT(sq2e, ml[12]);
	TEST_ASSERT_EQUAL_INT(sq4g, ml[13]);
	TEST_ASSERT_EQUAL_INT(sq2g, ml[14]);
}

void test_canmove_without_promotion(void) {
	//bpawn
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5a, bpawn, black));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5b, bpawn, black));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5c, bpawn, black));
	//blance
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5a, blance, black));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5b, blance, black));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5c, blance, black));
	//bknight
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5a, bknight, black));
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5b, bknight, black));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5c, bknight, black));
	//bsilver
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bsilver, black));
	//bbishop
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bbishop, black));
	//brook
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, brook, black));
	//bgold
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bgold, black));
	//bking
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bking, black));
	//bhorse
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bhorse, black));
	//bdragon
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5a, bdragon, black));
	//wpawn
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5i, wpawn, white));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5h, wpawn, white));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5g, wpawn, white));
	//wlance
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5i, wlance, white));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5h, wlance, white));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5g, wlance, white));
	//wknight
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5i, wknight, white));
	TEST_ASSERT_EQUAL_INT(0, canmove_without_promotion(sq5h, wknight, white));
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5g, wknight, white));
	//wsilver
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wsilver, white));
	//wbishop
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wbishop, white));
	//wrook
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wrook, white));
	//wgold
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wgold, white));
	//wking
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wking, white));
	//whorse
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, whorse, white));
	//bdragon
	TEST_ASSERT_EQUAL_INT(1, canmove_without_promotion(sq5i, wdragon, white));
}

void test_can_promote(void) {
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, bpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, bpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, bpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, bpawn));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, blance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, blance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, blance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, blance));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, bknight));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, bknight));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, bknight));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, bknight));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, bsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, bsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, bsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, bsilver));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, bbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, bbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, bbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, bbishop));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, brook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5c, brook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5b, brook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5a, brook));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5d, bking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5c, bking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5b, bking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5a, bking));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5g, wpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5h, wpawn));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5i, wpawn));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wlance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5g, wlance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5h, wlance));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5i, wlance));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5g, wsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5h, wsilver));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5i, wsilver));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5g, wbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5h, wbishop));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5i, wbishop));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wrook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5g, wrook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5h, wrook));
	TEST_ASSERT_EQUAL_INT(1, can_promote(sq5i, wrook));

	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5f, wking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5g, wking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5h, wking));
	TEST_ASSERT_EQUAL_INT(0, can_promote(sq5i, wking));
}

//ƒwƒ‹ƒp[ŠÖ” move•Ï”‚Ìfrom,to,ispmoto,morh,piece,cap‚ðŽæ‚èo‚µˆø”‚Æ“¯‚¶‚È‚çtrue‚ð•Ô‚·
_Bool cmp_move(int m, int from, int to, int pmoto, int morh, int piece, int cap) {
	_Bool t1 = (to == (m & 0x7f));
	_Bool t2 = (from == ((m >> 7) & 0x7f));
	_Bool t3 = (pmoto == ((m >> 14) & 0x01));
	_Bool t4 = (morh == ((m >> 15) & 0x01));
	_Bool t5 = (piece == ((m >> 16) & 0x1f));
	_Bool t6 = (cap == ((m >> 24) & 0x1f));
	return t1 * t2 * t3 * t4 * t5 * t6;
}

void test_possible_moves(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	new_board(&hd);
	strcpy_s(sfen, 128, "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1");
	set_board(&hd, sfen);
	hd.bd->hb[bpawn] = 1;
	hd.bd->hb[blance] = 1;
	hd.bd->hb[bknight] = 1;
	hd.bd->hb[bsilver] = 1;
	hd.bd->hb[bbishop] = 1;
	hd.bd->hb[brook] = 1;
	hd.bd->hb[bgold] = 1;

	int* ml = NULL;
	ml = possible_moves(&hd, ml);
	TEST_ASSERT_EQUAL_INT(269, arrlen(ml));
	//cmp_move -> from to pmoto morh piece cap
	TEST_ASSERT_TRUE(cmp_move(ml[0], sq1g, sq1f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[1], sq1i, sq1h, 0, 0, blance, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[2], sq2g, sq2f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[3], sq2h, sq3h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[4], sq2h, sq4h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[5], sq2h, sq5h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[6], sq2h, sq6h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[7], sq2h, sq7h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[8], sq2h, sq1h, 0, 0, brook, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[9], sq3g, sq3f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[10], sq3i, sq4h, 0, 0, bsilver, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[11], sq3i, sq3h, 0, 0, bsilver, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[12], sq4g, sq4f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[13], sq4i, sq5h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[14], sq4i, sq4h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[15], sq4i, sq3h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[16], sq5g, sq5f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[17], sq5i, sq6h, 0, 0, bking, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[18], sq5i, sq5h, 0, 0, bking, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[19], sq5i, sq4h, 0, 0, bking, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[20], sq6g, sq6f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[21], sq6i, sq7h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[22], sq6i, sq6h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[23], sq6i, sq5h, 0, 0, bgold, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[24], sq7g, sq7f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[25], sq7i, sq7h, 0, 0, bsilver, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[26], sq7i, sq6h, 0, 0, bsilver, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[27], sq8g, sq8f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[28], sq9g, sq9f, 0, 0, bpawn, empty));
	TEST_ASSERT_TRUE(cmp_move(ml[29], sq9i, sq9h, 0, 0, blance, empty));

}

void test_is_double_pawn(void) {
	/*
	   9  8  7  6  5  4  3  2  1
	a  l  n  s  g  k  g  s  n  l
	b  .  r  .  .  .  .  .  b  .
	c  p  p  p  p  p  p  p  .  p
	d  .  .  .  .  .  .  .  .  .
	e  .  .  .  .  .  B  .  .  P
	f  .  .  .  .  .  .  R  .  .
	g  .  P  P  P  P  P  P  P  .
	h  L  .  .  .  .  .  .  .  .
	i  .  N  S  G  K  G  S  N  L
	*/
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	new_board(&hd);
	strcpy_s(sfen, 128, "lnsgkgsnl/1r5b1/ppppppp1p/9/5B2P/6R2/1PPPPPPP1/L8/1NSGKGSNL b - 1");
	set_board(&hd, sfen);

	print_board(&hd);
	TEST_ASSERT_FALSE(is_double_pawn(&hd, sq9e, bpawn));
	TEST_ASSERT_TRUE(is_double_pawn(&hd, sq8e, bpawn));
	TEST_ASSERT_TRUE(is_double_pawn(&hd, sq3e, wpawn));
	TEST_ASSERT_FALSE(is_double_pawn(&hd, sq2e, wpawn));
}

void test_do_thinking(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	new_board(&hd);
	strcpy_s(sfen, 128, "ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1");
	set_board(&hd, sfen);
	//go(&hd);
	TEST_ASSERT_EQUAL_INT(1, go(&hd));
}