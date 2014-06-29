#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <stdlib.h>

const int PLY_MAX = 48; //bonanza���� 

typedef int Color;
typedef unsigned int Move;

//����������̃R�}���h���p�[�X
class USIInputParser{
public:
    USIInputParser(const string &line);
    string get_next_token(void);
    string get_rest_of_line(void);
    bool at_end_of_line(void);
private:
    const string &input_line;
    int length,current_index;
    void skip_whitespace(void);
};

typedef struct{
    Move *next_move;
    Move *last_move;
}next_move_t;

/*
��̔�����@�ꗗ
char p;

inline is_not_pmoto(char p)�ɓo�^
p & NOT_PMOTO       true�Ƃł���s��
p & NOT_PMOTO       false�Ƃł��琬

p > 1               true�Ȃ���
p > 1 || p == 0     true�Ȃ���܂��͋�
p < 0               true�Ȃ���
p <= 0              true�Ȃ���܂��͋�

inline do_white(char p)
���������I�Ɍ���
inline do_black(char p)
p |= 0xF0
���������I�ɐ���
p &= 0x0F

//��R�[�h�����^�C�v���v�Z
p &= 0x0F;
����̓C�����C���֐��Ƃ��ēo�^���Ă���
inline char type_of_piece(char p)

��R�[�h����J���[�𔻒肵�Ă���
p >> 4;����̓{�c
����̓C�����C���֐��Ƃ��ēo�^���Ă���
inline Color color_of_piece(int p)

row,col���W����sq���W�ɕϊ�
postion.h�ɂ���make_square�@inline�֐���
���p�̂���

�ėp����������̂͂����ɏ����A�ώG�ɏ����悤�ɂȂ�����
inline�֐��ւ��邱��
*/

#endif