#include <stdio.h>
#include <windows.h>

#include "unity.h"
#include "../shogi/quickRefC.h"
#include "simd_learn.h"

void setUp(void) {}
void tearDown(void) {}

void test_quickRefC() {
	printf("-----------test_quickRefC----------------------------------\n");
	float a1[] = { 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0 };
	float a2[] = { 11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0 };
	struct Node_Type m = {101,"text"};
	int a3=1, a4=2;

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
	TEST_ASSERT_EQUAL_INT(1, swapf(&a3,&a4));
	TEST_ASSERT_EQUAL_INT(1, selection_sortf(a1, 9));
	TEST_ASSERT_FLOAT_WITHIN(15.0, 0.1, *(binarySearch(15.0, a2, 9)));
	TEST_ASSERT_FLOAT_WITHIN(23.0, 0.1, add(2, 11.0, 12.0));
	TEST_ASSERT_EQUAL_INT(1, array_init());
	TEST_ASSERT_EQUAL_INT(1, string_init());
	TEST_ASSERT_EQUAL_INT(1, mul_array_init());
	TEST_ASSERT_EQUAL_INT(1, void_pointer());
	TEST_ASSERT_EQUAL_INT(1, const_pointer());
	TEST_ASSERT_EQUAL_INT(1, array_pointer());
	TEST_ASSERT_EQUAL_INT(1, func_pointer());
	TEST_ASSERT_EQUAL_INT(1, struct_unit_bitfiled());
	TEST_ASSERT_EQUAL_INT(1, DynamicMemoryManagement());
	//BSTはパス
	TEST_ASSERT_TRUE(isReadWriteable("test_main.c"));
	TEST_ASSERT_TRUE(file_error("test_main.c"));
	TEST_ASSERT_EQUAL_INT(1, print_errno("test_main.c"));
	TEST_ASSERT_EQUAL_INT(1, File_stream());
	//TEST_ASSERT_EQUAL_INT(1, scanf_test());	ユーザーによる入力が必要なのでパス
	TEST_ASSERT_FLOAT_WITHIN(135.0,0.1,sig_sum(a2,9));	//135.0
	float sum_arg = 0;
	TEST_ASSERT_TRUE(sum(a2,9,&sum_arg));	//sum_arg = 135.0
	TEST_ASSERT_EQUAL_INT(1,const_basic());
	return 1;
}

void test_simd() {
	printf("-----------test_simd----------------------------------\n");
	TEST_ASSERT_EQUAL_INT(1, simd_128());
	TEST_ASSERT_EQUAL_INT(1, simd_256());
	//int a[8] = { 1,2,3,4,5,6,7,8 };
	//TEST_ASSERT_EQUAL_INT(1, scalar_multiplication(a, 1, 8, 2));
	//TEST_ASSERT_EQUAL_INT(1, simd_info());
}

int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_quickRefC);
	RUN_TEST(test_simd);
	return UNITY_END();;
}

