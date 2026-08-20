#include <stdio.h>

#include "unity.h"
#include "../shogi/add.h"

void setUp(void) {}	//setUp,tearDownを書かないとエラーが出る -> 未解決の外部シンボル setUp が関数 UnityDefaultTestRun で参照されました
void tearDown(void) {}	//上に同じく -> 未解決の外部シンボル tearDown が関数 UnityDefaultTestRun で参照されました

int test_add(void) {
	TEST_ASSERT_EQUAL_INT(5, add(2, 3));
}

int test_sub() {
	TEST_ASSERT_EQUAL_INT(1, sub(3, 2));
}

int test_div() {
	TEST_ASSERT_EQUAL_INT(1, div(3, 2));
}

int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_add);
	RUN_TEST(test_sub);
	RUN_TEST(test_div);
	return UNITY_END();;
}