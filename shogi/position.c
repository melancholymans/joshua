#include "position.h"

//fileとrankを指定して座標値を返す
int set_square(int f, int r) {
	return f * 9 + r;
}

// sq座標からfile座標を計算して返している。
int set_file(int sq) {
	return sq / 9;
}

// sq座標からrank座標を計算して返している。
int set_rank(int sq) {
	return sq % 9;
}