#pragma once

#include "init.h"

usi_handler_t new_usi_handler(FILE* out);
errno_t handle(usi_handler_t* usi_hd,char* buf);
errno_t handle_usi(usi_handler_t* hd,char* cmds);
errno_t handle_is_ready(usi_handler_t* hd,char* cmds);
errno_t handle_setoption(char* cmds);
errno_t handle_usi_newgame(char* cmds);
errno_t handle_position(usi_handler_t* hd,char** cmds[],int count);
errno_t handle_go(char* cmds);
errno_t handle_stop(char* cmds);
errno_t handle_ponder_hit(char* cmds);
errno_t handle_quit(char* cmds);
errno_t handle_gameover(char* cmds);
errno_t handle_debug(usi_handler_t* hd, char** msg[], int size);
void put_send(usi_handler_t* hd, char* msg);
void put_debug(usi_handler_t* hd, char* msg);
void shorten_space(char* buf);
