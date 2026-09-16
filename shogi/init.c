#include "init.h"
#include "bitboard.h"
#include "position.h"

//各種テーブルの初期化
void init_tables() {
	// bitboard init
	new_mask_bb();
	new_file_mask();
	new_rank_mask();
	new_all_one_bb();
	new_in_front_mask();
	new_enemy_field();
	new_lance_attack();
	new_rook_attacks();
	new_bishop_attacks();
	new_king_attacks();
	new_gold_attacks();
	new_silver_attacks();
	new_pawn_attacks();
	new_knight_attacks();	// new_pawn_attackが実行していることが前提
	// position init
	new_square_relation_direct();
}
