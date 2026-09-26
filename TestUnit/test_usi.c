#include "unity.h"
#include "../shogi/usi.h"
#include "test_usi.h"

void test_shorten_space() {
	char buf[64] = "Hello    world     space";
	shorten_space(buf);
	printf("%s\n", buf);
	TEST_ASSERT_EQUAL_STRING("Hello world space", buf);
}