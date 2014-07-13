/*
Shogidokoro shogi playing engin
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "types.h"
#include "usi.h"
#include "usioption.h"
#include "evaluate.h"
#include "misc.h"
#include "search.h"

/*
https://github.com/sunfish-shogi/sunfish
https://github.com/sunfish-shogi/sunfish3
*/
void profile(void);
int main_test(int argc,char *argv[]);
void init(void);

int main(int argc,char *argv[])
{
    init();
    //�W�����o�͂ƃ����N�����āA�������ƒʐM���ł���
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);
    init_usi_options();
#if !defined(NDEBUG)
    profile();
    main_test(argc,argv); 
#endif
    usi_main_loop();
    return 0;
}

//�A�v���P�[�V�����S�̂̏�����
void init(void)
{
    //��z��]���l�ݒ�
    memset(piece_value,0,sizeof(piece_value));
    piece_value[PAWN] = DPawn;
    piece_value[LANCE] = DLance;
    piece_value[KNIGHT] = DKnight;
    piece_value[SILVER] = DSilver;
    piece_value[GOLD] = DGold;
    piece_value[BISHOP] = DBishop;
    piece_value[ROOK] = DRook;
    piece_value[KING] = DKing;
    piece_value[PPAWN] = DPPawn;
    piece_value[PLANCE] = DPLance;
    piece_value[PKNIGHT] = DPKnight;
    piece_value[PSILVER] = DPSilver;
    piece_value[PBISHOP] = DPBishop;
    piece_value[PROOK] = DPRook;
    //���������ꍇ�̕]���l�i�G�̋���Ƃ�ƑS�̂̕]���l(material)�͎��ꂽ���̋�̉��l+��������̋�̉��l�ƂȂ�j
    memset(cap_piece_value,0,sizeof(cap_piece_value));
    cap_piece_value[PAWN] = DPawn + DPawn;
    cap_piece_value[LANCE] = DLance + DLance;
    cap_piece_value[KNIGHT] = DKnight + DKnight;
    cap_piece_value[SILVER] = DSilver + DSilver;
    cap_piece_value[GOLD] = DGold + DGold;
    cap_piece_value[BISHOP] = DBishop + DBishop;
    cap_piece_value[ROOK] = DRook + DRook;
    cap_piece_value[KING] = DKing + DKing;
    cap_piece_value[PPAWN] = DPPawn + DPawn;
    cap_piece_value[PLANCE] = DPLance + DLance;
    cap_piece_value[PKNIGHT] = DPKnight + DKnight;
    cap_piece_value[PSILVER] = DPSilver + DSilver;
    cap_piece_value[PBISHOP] = DPBishop + DBishop;
    cap_piece_value[PROOK] = DPRook + DRook;
    //����������̕]���l(�������ꍇ�͌��݂̕]���l���琬��O�̋�̉��l�������āA���߂Đ�������̋�̉��l�𑫂����킹��)
    memset(pmoto_piece_value,0,sizeof(pmoto_piece_value));
    pmoto_piece_value[PPAWN] = DPPawn - DPawn;
    pmoto_piece_value[PLANCE] = DPLance - DLance;
    pmoto_piece_value[PKNIGHT] = DPKnight - DKnight;
    pmoto_piece_value[PSILVER] = DPSilver - DSilver;
    pmoto_piece_value[PBISHOP] = DPBishop - DBishop;
    pmoto_piece_value[PROOK] = DPRook - DRook;
}

void profile(void)
{
    int ply = 0;
    ptime_t aPT;

    ptime_init(&aPT);
    from_sfen(start_position);
    game_init(root_position);

    Move m = search(root_position,ply);
    double eapsed_time = ptime_now(&aPT);  //sec
    printf("eapsed_time=%.2f(sec) \n",eapsed_time);
    printf("search node=%dk \n",stats.search_node/1000);
    printf("nps=%.2fk \n",double(stats.search_node/(eapsed_time*1000)));
}
/*
���сi�ǖʂ̍X�V�̂݁j
2014/7
    debug           release
PLY nps             nps             seach node      
1   891k            -               -
2   1,188k          12,474k         2k
3   2,431k                          724k   
4   2,555k          25,119k         19,794k
5   2,561k                          552,915k
6   -���Ԋ|�肷��    4,320k(10min)   15,496,002k      1�ǖ� 38 nsec
7
30��ő����Ă�������
30^1 = 30
30^2 = 0.9k         PLY=1
30^3 = 27k          PLY=2   ?
30^4 = 810k         PLY=3
30^5 = 24,300k      PLY=4
30^6 = 729,000k     PLY=5
30^7 = 21,870,000k  PLY=6
*/

//�e�X�g�N��
int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}
