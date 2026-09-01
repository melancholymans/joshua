#include <stdio.h>
#include <windows.h>

#include "unity.h"
#include "simd_learn.h"

void setUp(void) {}
void tearDown(void) {}


void test_simd() {
	printf("-----------test_simd----------------------------------\n");
	TEST_ASSERT_EQUAL_INT(1, simd_128());
	TEST_ASSERT_EQUAL_INT(1, simd_256());
	int a[8] = { 1,2,3,4,5,6,7,8 };
	TEST_ASSERT_EQUAL_INT(72, scalar_multiplication(a, 1, 8, 2));
	TEST_ASSERT_EQUAL_INT(1, simd_info());
}

int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_simd);
	return UNITY_END();;
}

