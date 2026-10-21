#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <windows.h>
#include <process.h>

#include "move.h"
#include "usi.h"


usi_handler_t new_usi_handler(FILE* out) {
	usi_handler_t hd;
	hd.bd = NULL;
	hd.ucq = malloc(sizeof(comman_queue_t));
	hd.stream = out;
	hd.name = "shogi";
	hd.author = "joshua";
	hd.book_file_name[0] = '\0';
	hd.ml = NULL;
	hd.hash_size = 0;
	hd.can_ponder = false;
	hd.use_book = false;
	hd.is_stop = false;
	return hd;
}

int init_queue(comman_queue_t* q, int capacity) {
	q->buffer = (char**)malloc(sizeof(char*) * capacity);
	if (!q->buffer) {
		return 1;
	}
	q->capacity = capacity;
	q->head = 0;
	q->tail = 0;
	q->size = 0;
	InitializeCriticalSection(&(q->cs));
	InitializeConditionVariable(&(q->cv_not_full));
	InitializeConditionVariable(&(q->cv_not_empty));
	return 0;
}

void free_queue(comman_queue_t* q) {
	EnterCriticalSection(&(q->cs));
	for (; q->size > 0;) {
		free(q->buffer[q->head]);
		q->head = (q->head + 1) % q->capacity;
		q->size--;
	}
	LeaveCriticalSection(&(q->cs));
	DeleteCriticalSection(&(q->cs));
	free(q->buffer);
}

void push_queue(comman_queue_t* q, const char* str) {
	char* copy_str = _strdup(str);
	EnterCriticalSection(&(q->cs));
	for (; q->size >= q->capacity;) {
		SleepConditionVariableCS(&(q->cv_not_full), &(q->cs), INFINITE);
	}
	q->buffer[q->tail] = copy_str;
	q->tail = (q->tail + 1) % q->capacity;
	q->size++;
	WakeConditionVariable(&(q->cv_not_empty));
	LeaveCriticalSection(&(q->cs));
}

char* pop_queue(comman_queue_t* q) {
	EnterCriticalSection(&(q->cs));
	for (; q->size <= 0;) {
		SleepConditionVariableCS(&(q->cv_not_empty), &(q->cs), INFINITE);
	}
	char* str = q->buffer[q->head];
	q->head = (q->head + 1) % q->capacity;
	q->size--;
	WakeConditionVariable(&(q->cv_not_full));
	LeaveCriticalSection(&(q->cs));
	return str;	//ここ少し心配
}

void receive_usi(void *arg) {
	usi_handler_t* hd = (usi_handler_t*)arg;
	char buf[1024];
	for (;;) {
		if (fget(buf, 2048, stdin) != NULL) {
			buf[strcspn(buf, "\n")] = '\0';
		}
		if (!strncmp(buf, "quit", 4)) {
			free_queue(hd->ucq);
			exit(0);
		}
		if (!strncmp(buf, "stop", 4)) {
			hd->is_stop = true;
		}
		if (!strncmp(buf, "ponderhit", 9)) {
			hd->ponderhit_receive_time = timeGetTime();	//ponderを受信した時間(msec単位)
		}
		if (strncmp(buf, "gameover", 8)) {
			hd->is_stop = true;
		}
		push_queue(hd->ucq, buf);
	}
}

_Bool begin_thread(usi_handler_t* hd) {
	uintptr_t hdl = _beginthread(receive_usi, 0, hd);
	return (hdl != -1);
}

errno_t wait_command(usi_handler_t* hd,char* buf) {
	char* str = pop_queue(hd->ucq);
	strcpy_s(buf, 1024, str);
	free(buf);
}

/*
option:
	エンジン固有の値をGUIに送るためのコマンド
	usiコマンドとusiokの間に送る
	option name <optionname> type <optiontype> <paramter>
	これを送るとGUI側からsetoptionコマンドで送り返してくる
	optionで送らなくても必ずGUI側から送ってくるoptionが２つある
	USI_HashとUSI_Ponderになる

	<optiontype<n>には6種類設定できる
	check: エンジン設定ダイヤログでチックボックスになる指定
	spin: 数値を指定
	combo:ポップアップメニュー
	string:文字列を指定
	filename:ファイル名を指定
	default:デフォルト値を指定
	min,max:種類がspinなら上限下限を指定できる
	var:種類がcomboなら選択できる文字列を指定できる（ポップアップに出てくる文字列を指定できる）
	このoptionはGUIにエンジン登録するときに設定される値を送っている。
setoption:
	USI_HashとUSI_Ponderとエンジン登録時に行ったoptionを送ってくるので
	その値を使ってエンジンの状態を決定する
具体例:Lesserkai
	option name BookFile type string default public.bin
	option name UseBook type check default true

	返ってくるsetoption
	setoption name USI_Ponder value true	//これはoptionで設定しなくてもGUIから送ってくる
	setoption name USI_Hash value 256		//これはoptionで設定しなくてもGUIから送ってくる
	setoption name BookFile value public.bin	//optionで送った値がGUI側で保存してあるので便利
	setoption name UseBook value true			//optionで送った値がGUI側で保存してあるので便利
エンジンから送るoptionはGUIにエンジン管理のために登録するもので、登録後はGUI側の設定が優先される
*/
int usi_start(usi_handler_t* hd) {
	char cmds[64];
	char msg[64];
	char hash_size[16];
	char use_book[8];
	for (;;) {
		wait_command(cmds,sizeof(cmds));
		int len = strlen(cmds);
		if (!strncmp(cmds, "usi", len)) {
			sprintf_s(msg, sizeof(msg), "id name %s", hd->name);
			put_send(hd, msg);
			sprintf_s(msg, sizeof(msg), "id author %s", hd->author);
			put_send(hd, "option name USI_Ponder type check default false");
			put_send(hd, "option name UseBook type check default false");
			put_send(hd, "usiok");
		}
		else if (!strncmp(cmds, "isready", len)) {
			put_send(hd, "readyok");
		}
		else if (!strncmp(cmds, "setoption name USI_Ponder value true", len)) {
			hd->can_ponder = true;
		}
		else if (!strncmp(cmds, "setoption name USI_Ponder value false", len)) {
			hd->can_ponder = false;
		}
		else if (!strncmp(cmds, "setoption name USI_Hash value ", strlen("setoption name USI_Hash value"))) {
			strcpy_s(hash_size,sizeof(hash_size),cmds + strlen("setoption name USI_Hash value "));
			hd->hash_size = atoi(hash_size);
		}
		else if (!strncmp(cmds, "setoption name BookFile value ", strlen("setoption name BookFile value "))) {
			strcpy_s(hd->book_file_name,sizeof(hd->book_file_name),cmds + strlen("setoption name BookFile value "));
		}
		else if (!strncmp(cmds, "setoption name UseBook value ", strlen("setoption name UseBook value "))) {
			strcpy_s(use_book,sizeof(use_book),cmds + strlen("setoption name UseBook value "));
			if (!strncmp(use_book, "true", 4)) {
				hd->use_book = true;
			}
			else {
				hd->use_book = false;
			}
		}
		else if (!strncmp(cmds, "usinewgame", len)) {
			return true;
		}
	}
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
	if (!strcmp("setoption",cmds[0])) {
		return handle_setoption(cmds);
	}
	else if (!strcmp("stop", cmds[0])) {
		return handle_stop(cmds);
	}
	else if (!strcmp("go", cmds[0])) {
		return handle_go(usi_hd);
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


errno_t handle_setoption(char* cmds) {
	//TODO:
	return NULL;
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

//goコマンドに対応する。思考を開始し、その結果moveを受取り局面を更新してGUIに手を返す
errno_t handle_go(usi_handler_t* hd) {
	//:TODO
	// 思考開始に対応する
	int move = 0;
	move = go(hd);
	if (move == 0){
		put_send(hd, "bestmove resign");
	}
	else {
		char usi_move[8];
		char str[16];
		do_move(hd, move);
		move_to_usi(move, usi_move);
		snprintf(str,sizeof(str), "bestmove %s", usi_move);
		put_send(hd, str);
	}
	return 200;
}

errno_t handle_stop(char* cmds) {
	//:TODO
	// 即座に思考を中断しbestmoveを返す
	printf("%s\n", __func__);
	return NULL;
}

errno_t handle_ponder_hit(char* cmds) {
	//:TODO
	// ポンダ処理
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
