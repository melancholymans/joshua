#if !defined(MOVE_H_INCLUDE)
#define MOVE_H_INCLUDE

#include "types.h"

/*
Move�\���̂���from������o��
*/
inline int move_from(Move m)
{
    return (m >> 8) & 0xFF;
}

/*
Move�\���̂���to������o��
*/
inline int move_to(Move m)
{
    return m & 0xFF;
}

/*
Move�\���̂���piece������o��
Move�\���̂Ɋi�[����Ă���piece��Color����
�������ꂽ����񂾂��Ȃ̂ŁA�Տ�ړ��̎��ɂ�
�g�p����Ȃ����ł�̎��Ɏg�p����.
�ł�Color����t�^����board���X�V���邱��
*/
inline char move_piece(Move m)
{
    return (m >> 17) & 0x0F; 
}

/*
Move�\���̂���cap_piece������o��
Move�\���̂Ɋi�[����Ă���piece��Color����
�������ꂽ����񂾂�.
*/
inline char move_cap_piece(Move m)
{
    return (m >> 21) & 0X0F;
}

/*
Move�\���̂���pmoto������o��
����Ȃ�0x10000��Ԃ��iPMOTO�Œ�`����Ă���j
����Ȃ��Ȃ�0��Ԃ�
*/
inline int move_pmoto(Move m)
{
    return m & 0x10000;
}

Move move_from_string(const Position &pos,const string &cmd);
int square_from_string(const string sq);
string string_from_move(const Move m);
string string_from_square(int sq);

#endif
