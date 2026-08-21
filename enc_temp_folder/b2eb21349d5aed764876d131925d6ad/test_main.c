#include <stdio.h>
#include <windows.h>

#include "unity.h"
#include "../shogi/quickRefC.h"

void setUp(void) {
}	//setUp,tearDownを書かないとエラーが出る -> 未解決の外部シンボル setUp が関数 UnityDefaultTestRun で参照されました
void tearDown(void) {}	//上に同じく -> 未解決の外部シンボル tearDown が関数 UnityDefaultTestRun で参照されました

int test_quickRefC() {
	float a1[] = { 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0 };
	float a2[] = { 11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0 };
	struct Node_type {
		char name[25];
		int age;
	};
	struct Node_type m;

	TEST_ASSERT_EQUAL_INT(1,type_conversion());
	TEST_ASSERT_EQUAL_INT(1, sizeof_basic_types());
	TEST_ASSERT_EQUAL_INT(1, float_learn());
	TEST_ASSERT_EQUAL_INT(1, enum_learn());
	TEST_ASSERT_EQUAL_INT(1, void_learn());
	TEST_ASSERT_EQUAL_INT(1, error_exit(1));
	TEST_ASSERT_EQUAL_INT(1, string_literal_learn());
	TEST_ASSERT_EQUAL_INT(1, pointer_learn());
	TEST_ASSERT_EQUAL_INT(1, incremental_learn());
	TEST_ASSERT_EQUAL_INT(1, struct_op());
	TEST_ASSERT_EQUAL_INT(1, Alignof_learn());
	TEST_ASSERT_EQUAL_INT(1, function_learn());
	TEST_ASSERT_EQUAL_INT(1, factorial(5));
	TEST_ASSERT_EQUAL_INT(1, addArray(a1, a2,9));
	TEST_ASSERT_EQUAL_INT(1, initNode(&m));
}


int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_quickRefC);
	return UNITY_END();;
}