#include <stdio.h>
#include <string.h>

#include "usi.h"

usi_handler_t* new_usi_handler(FILE* out) {
	return &(usi_handler_t) {
		.bd = NULL,
		.stream = out,
		.name = "shogi",
		.author = "joshua",
		.mv = NULL
	};
}

errno_t handle(usi_handler_t* usi_hd, char* buf){
	char* words[512];	//ポインタの配列、300手ぐらいを想定して512まで用意
	int count = 0;
	shorten_space(buf);	
	char* ptr = NULL;
	char* token = strtok_s(buf, " ",&ptr);
	for (; token != NULL && count < 512;) {
		words[count++] = token;
		token = strtok_s(NULL, " ",&ptr);
	}
	for (int i = 0; i < count; i += 1) {
		printf("words[%d] = %s\n", i, words[i]);
	}
	return NULL;
}

//入力文字列の中に余分なスペースがあれば短縮する
void shorten_space(char* buf) {
	int i=0, j=0;
	int in_space = 0;
	for (; buf[i] != '\0';) {
		if (buf[i] == ' ') {
			if (!in_space) {
				buf[j++] = ' ';
				in_space = 1;
			}
		}
		else {
			buf[j++] = buf[i];
			in_space = 0;
		}
		i += 1;
	}
	buf[j] = '\0';
}
