#include "init.h"
#include "bitboard.h"
#include "position.h"

//各種テーブルの初期化
void init_tables() {
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
	new_knight_attacks();	// new_pawn_attackが成立していることが前提
	new_square_relation_direct();
	new_between_bb();		//new_square_relation_directが成立していることが前提
	new_star_bb();			//new_silver_attacksが成立していることが前提
	new_attack_to_edge();	//rook_attack,bishop_attack,lance_attackが成立していることが前提
	new_gold_check_table();
	new_silver_check_table();
	new_knight_check_table();
	new_lance_check_table();
	new_pawn_check_table();
	new_bishop_check_table();
}
