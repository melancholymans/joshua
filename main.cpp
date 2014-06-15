/*
Shogidokoro shogi playing engin
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "gtest\gtest.h"
#include "types.h"
#include "usi.h"
#include "usioption.h"

using namespace std;

int main_test(int argc,char *argv[]);
void init(void);

int main(int argc,char *argv[])
{
    //�W�����o�͂ƃ����N�����āA�������ƒʐM���ł���
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);
    init_usi_options();
#if !defined(NDEBUG)
    main_test(argc,argv); 
#endif
    usi_main_loop();
    return 0;
}

//�A�v���P�[�V�����S�̂̏�����
void init(void)
{
}

//�e�X�g�N��
int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}
