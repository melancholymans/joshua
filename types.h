#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <stdlib.h>

const int PLY_MAX = 48; //bonanza���� 

typedef int Color;
typedef unsigned int Move;

typedef struct{
    Move *next_move;
    Move *last_move;
}next_move_t;

/*
��̔�����@�ꗗ
char p;

p & NOT_PMOTO       true�Ƃł���s��
p & NOT_PMOTO       false�Ƃł��琬

p > 1               true�Ȃ���
p > 1 || p == 0     true�Ȃ���܂��͋�
p < 0               true�Ȃ���
p <= 0              true�Ȃ���܂��͋�

���������I�Ɍ���
p |= 0xF0
���������I�ɐ���
p &= 0x0F

//��R�[�h�����^�C�v���v�Z
p &= 0x0F;
����̓C�����C���֐��Ƃ��ēo�^���Ă���
inline char type_of_piece(char p)

��R�[�h����J���[�𔻒肵�Ă���
p >> 4;
����̓C�����C���֐��Ƃ��ēo�^���Ă���
inline Color color_of_piece(int p)

�ėp����������̂͂����ɏ����A�ώG�ɏ����悤�ɂȂ�����
inline�֐��ւ��邱��
*/

#endif