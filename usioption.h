#if !defined(USIOPTION_H_INCLUDE)
#define USIOPTION_H_INCLUDE
#include <map>

using namespace std;

extern map<string,string> options;      //����������ݒ肳�ꂽ�ݒ��ێ�
extern map<char,string> piece_letters;    //��R�[�h�Ƌ��(P,+P,r,R�Ȃ�)���֌W�Â��Ă���

void init_usi_options(void);
void set_option(USIInputParser &uip);
void set_option_value(const string &option_name,const string &new_value);

#endif
