#pragma once

#include "init.h"

usi_handler_t new_usi_handler(FILE* out);
int init_queue(usi_handler_t* hd, int capacity);
void free_queue(comman_queue_t* q);
void push_queue(comman_queue_t* q, const char* str);
char* pop_queue(comman_queue_t* q);
void receive_usi(void* arg);
_Bool begin_thread(usi_handler_t* hd);
void wait_command(usi_handler_t* hd, char* buf);
errno_t usi_start(usi_handler_t* hd);
errno_t handle(usi_handler_t* usi_hd,char* buf);
errno_t handle_setoption(char* cmds);
errno_t handle_position(usi_handler_t* hd,char* cmds[],int count);
errno_t handle_go(usi_handler_t* hd);
errno_t handle_stop(char* cmds);
errno_t handle_ponder_hit(char* cmds);
errno_t handle_quit(char* cmds);
errno_t handle_gameover(usi_handler_t* hd);
errno_t handle_debug(usi_handler_t* hd, char** msg[], int size);
void put_send(usi_handler_t* hd, char* msg);
void put_debug(usi_handler_t* hd, char* msg);
void shorten_space(char* buf);
