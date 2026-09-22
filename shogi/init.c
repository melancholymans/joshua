#include "init.h"
#include "bitboard.h"
#include "position.h"

//各種テーブルの初期化
void init_tables() {
	new_sq_mask();
	new_file_mask();
	new_rank_mask();
	new_all_one_mask();
	new_all_zero_mask();
	new_in_front_mask();
	new_enemy_mask();
	new_lance_attack_mask();
	new_rook_attack_rank_to_mask();
	new_bishop_attack_to_mask();
	new_king_attack_mask();
	new_gold_attack_mask();
	new_silver_attack_mask();
	new_pawn_attack_mask();
	new_star_mask();			// new_silver_attacksが成立していることが前提
	new_knight_attack_mask();	// star_mask,new_pawn_attackが成立していることが前提
	new_square_relation_direct();
	new_between_mask();		// new_square_relation_directが成立していることが前提
	new_attack_to_edge();	// rook_attack,bishop_attack,lance_attackが成立していることが前提
	new_gold_check_table();
	new_silver_check_table();
	new_knight_check_table();
	new_lance_check_table();
	new_pawn_check_table();
	new_bishop_check_table();
	new_horse_check_table();
	new_neighbor5x5();
}
