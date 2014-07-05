#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"

typedef struct Position{
    void set_posiition();
    char board[16*13 + 7*2 + 2 + 32];  //�Ō�̂R�Q�͔z��̑傫����256�ɂ��邽�߂̂��̈Ӗ��͂Ȃ�
    //int king_square[2];
    int turn;
}position_t;

//position�̒萔
const int BOARD_UPPER = 16*13;
const int STAND_BLACK_START = 16*13;
const int STAND_WHITE_START = 16*13 + 7;
const int LIMIT = 16*13 + 7*2 + 2;  //16*13�͔Տ�7*2�͋��A�Ō�̂Q��KING�p�̍��W

extern string start_position;
extern position_t root_position;
extern const int BLACK;
extern const int WHITE;
extern const int NO_COLOR;
extern int turn;

const extern char EMPTY;
const extern char EDGE;
//const extern char NOT_PMOTO;
enum{
    PPAWN=2,PLANCE,PKNIGHT,PSILVER,PBISHOP,PROOK,
    KING=8,GOLD,PAWN,LANCE,KNIGHT,SILVER,BISHOP,ROOK
};
enum{
    UUL = -33,UUR = -31,UL = -17,U = -16,UR = -15,L = -1,
    R = 1,DL = 15,D = 16,DR = 17,DDL = 31,DDR = 33
};

const extern char BP_PAWN;      //2
const extern char BP_LANCE;     //3
const extern char BP_KNIGHT;    //4   
const extern char BP_SILVER;    //5
const extern char BP_BISHOP;    //6
const extern char BP_ROOK;      //7
const extern char B_KING;       //8
const extern char B_GOLD;       //9
const extern char B_PAWN;       //10
const extern char B_LANCE;      //11
const extern char B_KNIGHT;     //12
const extern char B_SILVER;     //13
const extern char B_BISHOP;     //14
const extern char B_ROOK;       //15

const extern char WP_PAWN;      //2-16=-14
const extern char WP_LANCE;     //3-16=-13
const extern char WP_KNIGHT;    //4-16=-12
const extern char WP_SILVER;    //5-16=-11
const extern char WP_BISHOP;    //6-16=-10
const extern char WP_ROOK;      //7-16=-9
const extern char W_KING;       //8-16=-8
const extern char W_GOLD;       //9-16=-7
const extern char W_PAWN;       //10-16=-6
const extern char W_LANCE;      //11-16=-5
const extern char W_KNIGHT;     //12-16=-4
const extern char W_SILVER;     //13-16=-3
const extern char W_BISHOP;     //14-16=-2
const extern char W_ROOK;       //15-16=-1

//���W�L��
enum{
    SQ_9A=33,SQ_8A,SQ_7A,SQ_6A,SQ_5A,SQ_4A,SQ_3A,SQ_2A,SQ_1A,
    SQ_9B=49,SQ_8B,SQ_7B,SQ_6B,SQ_5B,SQ_4B,SQ_3B,SQ_2B,SQ_1B,
    SQ_9C=65,SQ_8C,SQ_7C,SQ_6C,SQ_5C,SQ_4C,SQ_3C,SQ_2C,SQ_1C,
    SQ_9D=81,SQ_8D,SQ_7D,SQ_6D,SQ_5D,SQ_4D,SQ_3D,SQ_2D,SQ_1D,
    SQ_9E=97,SQ_8E,SQ_7E,SQ_6E,SQ_5E,SQ_4E,SQ_3E,SQ_2E,SQ_1E,
    SQ_9F=113,SQ_8F,SQ_7F,SQ_6F,SQ_5F,SQ_4F,SQ_3F,SQ_2F,SQ_1F,
    SQ_9G=129,SQ_8G,SQ_7G,SQ_6G,SQ_5G,SQ_4G,SQ_3G,SQ_2G,SQ_1G,
    SQ_9H=145,SQ_8H,SQ_7H,SQ_6H,SQ_5H,SQ_4H,SQ_3H,SQ_2H,SQ_1H,
    SQ_9I=161,SQ_8I,SQ_7I,SQ_6I,SQ_5I,SQ_4I,SQ_3I,SQ_2I,SQ_1I,
    SQ_LIMIT
};
//col,row���W����sq���W���v�Z
inline int make_square(int col,int row)
{
    return ((row+1) << 4) | col;
}

/*
sq�Ս��W����col,row���W�ɕϊ�
*/
inline void make_col_row(int sq,int *col,int *row)
{
    *row = sq/16;   //�����̊���Z�͏����_�͐؂�̂�
    *col = sq - *row*16;
    *row = *row - 1;
}

//��R�[�h�����^�C�v���v�Z
inline char type_of_piece(char p)
{
    return p & 0x0F;
}

inline Color color_of_piece(int p)
{
    //��R�[�h����J���[�𔻒肵�Ă���
    //��������char�^���E�V�t�g���Ă���̂�ANSI C�ł͋�������`����Ă��Ȃ�
    //VC++2010�ŎZ�p�V�t�g���̗p���Ă���悤�Ȃ̂ł��̂܂܂Ƃ��邪�����n��
    //�ύX�ɂȂ�����e�X�g�ŃG���[���ł�悤�ɂ��Ă���
    //BLACK�����0��WHITE�����-1��Ԃ�
    //return p >> 4;����WHITE PIECE,BLACK PIECE��-1�A0��Ԃ���
    //EMPTY,EDGE��BLACK PIECE�Ɣ��ʂ���
    if(p < 0){
        return -1;
    }
    else if(p > 1){
        return 0;
    }
    return NO_COLOR;
}

/*
��R�[�h��^���Đ����Ă��Ȃ������肷��
�����Ă��Ȃ�������true,����Ȃ�false
*/
inline int is_not_pmoto(char p)
{
    return p & 8;
}

/*
����R�[�h�������I�Ɍ���R�[�h�ɂ���
*/
inline char do_white(char p)
{
    return p | 0xF0;
}

/*
����R�[�h�������I�ɐ���R�[�h�ɂ���
*/
inline char do_black(char p)
{
    return p & 0x0F;
}

/*
Move
31-30-29-28-27-26-25-24-23-22-21-20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01-00
                     | cap piece |src piece |pm |       from square      |       to square    |

cap piece �������icolor�͂��Ȃ����̂݁@�������ߖ�̂��߁j     21-24bit
from piece ��������icolor�͂��Ȃ����̂݁@�������ߖ�̂��߁j    17-20bit
pm->pmoto�̃t���O�P�Ȃ琬�蓮��    1bit 16bit
from square �ړ����̍��W 8bit 8-15bit
to square �ړ���̍��W 8bit 0-7bit
25-31bit�͋�
*/
inline Move make_move(int from,int to,int pmoto,char piece,char cap_piece)
{
    return (unsigned int(to) | unsigned int(from) << 8 | unsigned int(pmoto) << 16 | unsigned int(piece & 0x0F) << 17 | unsigned int(cap_piece & 0x0F) << 21);
}


void from_sfen(string &sfen);
string to_sfen(const Position &pos);
void print_board(const Position &pos);
void put_piece(char p,int sq,int num);
short *do_move(Position &pos,Move m,short *mf);
void undo_move(Position &pos,int ply);

#endif

