#include <stdio.h>
#include <string.h>

#include "usi.h"

usi_handler_t new_usi_handler(FILE* out) {
	usi_handler_t hd;
	hd.bd = NULL;
	hd.stream = out;
	hd.name = "shogi";
	hd.author = "joshua";
	hd.mv = NULL;
	return hd;
}

errno_t handle(usi_handler_t* usi_hd, char* buf){
	char* cmds[512];	//ポインタの配列、300手ぐらいを想定して512まで用意
	int count = 0;
	shorten_space(buf);	
	char* ptr = NULL;
	char* token = strtok_s(buf, " ",&ptr);
	for (; token != NULL && count < 512;) {
		cmds[count++] = token;
		token = strtok_s(NULL, " ",&ptr);
	}
	if (!strcmp("usi", cmds[0])) {
		handle_usi(cmds);
	}else if(!strcmp("isready", cmds[0])){
		handle_is_ready(cmds);
	}
	else if (!strcmp("setoption",cmds[0])) {
		handle_setoption(cmds);
	}
	else if (!strcmp("usinewgame", cmds[0])) {
		handle_usi_newgame(cmds);
	}
	else if (!strcmp("stop", cmds[0])) {
		handle_stop(cmds);
	}
	else if (!strcmp("go", cmds[0])) {
		handle_go(cmds);
	}
	else if (!strcmp("position", cmds[0])) {
		handle_position(cmds);
	}
	else if (!strcmp("ponderhit", cmds[0])) {
		handle_ponder_hit(cmds);
	}
	else if (!strcmp("quit", cmds[0])) {
		handle_quit(cmds);
	}
	else if (!strcmp("gameover", cmds[0])) {
		handle_gameover(cmds);
	}
	else if (!strcmp("debug", cmds[0])) {
		handle_debug(usi_hd,cmds,count);
	}
	else {
		char msg[32];
		strcpy_s(msg,12, "unknow cmd ");
		strcat_s(msg,12+16,cmds[0]);
		put_debug(usi_hd,msg);
	}
	return NULL;
}

errno_t handle_usi(char* cmds) {
	//:TODO
	printf("%s\n",__func__);
	return NULL;
}

errno_t handle_is_ready(char* cmds) {
	//TODO:
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_setoption(char* cmds) {
	//TODO:
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_usi_newgame(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_position(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_go(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_stop(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_ponder_hit(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_quit(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_gameover(char* cmds) {
	//:TODO
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_debug(usi_handler_t *hd,char** msg[],int size) {
	//:TODO
	printf("%s\n", __func__);
	for (int i = 0; i < size; i += 1) {
		put_debug(hd, msg[i]);
	}
	return NULL;
}


void send(usi_handler_t *hd,char* msg) {
	fputs(msg, hd->stream);
}

void put_debug(usi_handler_t *hd,char* msg) {
	//:TODO
	send(hd, msg);
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
