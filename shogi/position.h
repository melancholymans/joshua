#pragma once

// 盤面の座標を表す定数を定義しています。これにより、将棋の盤面上の各マスを簡単に参照できます。
enum {
	sq1a = 0,	
	sq1b = 1,
	sq1c = 2,
	sq1d = 3,
	sq1e = 4,
	sq1f = 5,
	sq1g = 6,			
	sq1h = 7,
	sq1i = 8,
	sq2a = 9,
	sq2b = 10,
	sq2c = 11,
	sq2d = 12,
	sq2e = 13,
	sq2f = 14,
	sq2g = 15,
	sq2h = 16,
	sq2i = 17,
	sq3a = 18,
	sq3b = 19,
	sq3c = 20,
	sq3d = 21,
	sq3e = 22,
	sq3f = 23,
	sq3g = 24,
	sq3h = 25,
	sq3i = 26,
	sq4a = 27,
	sq4b = 28,
	sq4c = 29,
	sq4d = 30,
	sq4e = 31,
	sq4f = 32,
	sq4g = 33,
	sq4h = 34,
	sq4i = 35,
	sq5a = 36,
	sq5b = 37,
	sq5c = 38,
	sq5d = 39,
	sq5e = 40,
	sq5f = 41,
	sq5g = 42,
	sq5h = 43,
	sq5i = 44,
	sq6a = 45,
	sq6b = 46,
	sq6c = 47,
	sq6d = 48,
	sq6e = 49,
	sq6f = 50,
	sq6g = 51,
	sq6h = 52,
	sq6i = 53,
	sq7a = 54,
	sq7b = 55,
	sq7c = 56,
	sq7d = 57,
	sq7e = 58,
	sq7f = 59,
	sq7g = 60,
	sq7h = 61,
	sq7i = 62,
	sq8a = 63,
	sq8b = 64,
	sq8c = 65,
	sq8d = 66,
	sq8e = 67,
	sq8f = 68,
	sq8g = 69,
	sq8h = 70,
	sq8i = 71,
	sq9a = 72,
	sq9b = 73,
	sq9c = 74,
	sq9d = 75,
	sq9e = 76,
	sq9f = 77,
	sq9g = 78,
	sq9h = 79,
	sq9i = 80
};

enum {
	rank1 = 0,
	rank2 = 1,
	rank3 = 2,
	rank4 = 3,
	rank5 = 4,
	rank6 = 5,
	rank7 = 6,
	rank8 = 7,
	rank9 = 8,
	file1 = 0,
	file2 = 1,
	file3 = 2,
	file4 = 3,
	file5 = 4,
	file6 = 5,
	file7 = 6,
	file8 = 7,
	file9 = 8
};

enum {
	delta_nw = 8,
	delta_sw = 10,
	delta_ne = -10,
	delta_se = -8
};

//ボナンザに使われていた方向定数
enum {
	direct_misc = 0,	//ない
	direct_file = 2,	//縦方向
	direct_rank = 3,	//横方向
	direct_diag_nesw = 4,	//右上から左下
	direct_diag_nwse = 5,	//左上から右下 
	direct_cross = 2,	//十字方向（縦横）
	direct_diag = 4
};

enum {
	black = 0,
	white = 1
};

extern int square_relation_direct[81][81];

void new_square_relation_direct();
int set_square(int f, int r);
int set_file(int sq);
int set_rank(int sq);
int square_relation(const int sq1, const int sq2);
int opposite_color(const int c);
