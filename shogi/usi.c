#include <stdio.h>
#include <string.h>
#include <errno.h>

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
		return handle_usi(usi_hd,cmds);
	}else if(!strcmp("isready", cmds[0])){
		return handle_is_ready(usi_hd,cmds);
	}
	else if (!strcmp("setoption",cmds[0])) {
		return handle_setoption(cmds);
	}
	else if (!strcmp("usinewgame", cmds[0])) {
		return handle_usi_newgame(cmds);
	}
	else if (!strcmp("stop", cmds[0])) {
		return handle_stop(cmds);
	}
	else if (!strcmp("go", cmds[0])) {
		return handle_go(cmds);
	}
	else if (!strcmp("position", cmds[0])) {
		return handle_position(usi_hd,cmds,count);
	}
	else if (!strcmp("ponderhit", cmds[0])) {
		return handle_ponder_hit(cmds);
	}
	else if (!strcmp("quit", cmds[0])) {
		return handle_quit(cmds);
	}
	else if (!strcmp("gameover", cmds[0])) {
		return handle_gameover(usi_hd);
	}
	else if (!strcmp("debug", cmds[0])) {
		return handle_debug(usi_hd,cmds,count);
	}
	else {
		char msg[32];
		strcpy_s(msg,12, "unknow cmd ");
		strcat_s(msg,12+16,cmds[0]);
		put_debug(usi_hd,msg);
		return 202;
	}
}

errno_t handle_usi(usi_handler_t* hd,char* cmds) {
	char msg[32];
	strcpy_s(msg, 12, "id name ");
	strcat_s(msg, 12 + 16, hd->name);
	put_send(hd, msg);
	strcpy_s(msg, 12, "id author ");
	strcat_s(msg, 12 + 16, hd->author);
	put_send(hd, msg);
	put_send(hd, "usiok");
	fflush(hd->stream);
	return 200;
}

errno_t handle_is_ready(usi_handler_t* hd,char* cmds) {
	new_board(hd);
	if (hd->bd == NULL) {
		return 203;
	}
	put_send(hd, "readyok");
	return 200;
}

errno_t handle_setoption(char* cmds) {
	//TODO:
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_usi_newgame(char* cmds) {
	//:nothing
	return 200;
}

errno_t handle_position(usi_handler_t* hd,char* cmds[],int count) {
	// 入力されるcmds[]
	//	cmds[0] = position
	//  cmds[1] = startpos <- sfen 
	//  cmds[2] = moves 
	//  cmds[3] = 2g2f
	//  cmds[4] = 8c8d 
	//  cmds[5] = 2f2e
	//  ...
	// 又は
	//  cmds[0] = position
	//  cmds[1] = sfen 
	//  cmds[2] = lnsgkgsnl/9/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL sfen = cmds[2]+cmds[3]+cmds[4]+cmds[5]   
	//  cmds[3] = w
	//  cmds[4] = -
	//  cmds[5] = 1
	//  cmds[6] = moves 
	//  cmds[7] = 5a6b
	//  cmds[8] = 7g7f 
	//  cmds[9] = 3a3b
	//  ...
	printf("%s\n", __func__);
	char sfen[128];
	int mark = 0;
	sfen[0] = '\0';
	if (!strcmp("sfen", cmds[1])) {
		for (int i = 2; i < 6; i++) {
			strcat_s(sfen, strlen(cmds[2])+6+1, cmds[i]);
			if (i < 5) {
				strcat_s(sfen, strlen(cmds[2]) + 6 + 1, " ");
			}
		}
		mark = 6;	//次に見る配列のindex
	}
	else if (!strcmp("startpos", cmds[1])) {
		strcpy_s(sfen,63+1,"lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1");
		mark = 2;	//次に見る配列のindex
	}
	set_board(hd, sfen);
	if (strcmp(cmds[mark],"moves")) {	//sfen文字列にmovesの文字列がなかったら、ここで終わり
		return 200;
	}
	int size = count - mark - 1;
	for (int i=0;i<size;i++) {
		//TODO: boardを更新するpush_moveみたいな関数をここに書く
		printf("moves = %s\n", cmds[mark + 1 + i]);
	}
	return 200;
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
	return 201;		
}

errno_t handle_gameover(usi_handler_t* hd) {
	new_board(hd);
	if (hd->bd == NULL) {
		return 203;
	}
	put_send(hd, "readyok");
	return 200;
}

errno_t handle_debug(usi_handler_t *hd,char** msg[],int size) {
	printf("%s\n", __func__);
	for (int i = 0; i < size; i++) {
		put_debug(hd, msg[i]);
	}
	return NULL;
}

void put_send(usi_handler_t *hd,char* msg) {
	fprintf(hd->stream,"%s\n",msg);
}

void put_debug(usi_handler_t *hd,char* msg) {
	put_send(hd, msg);
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
