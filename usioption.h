#if !defined(USIOPTION_H_INCLUDE)
#define USIOPTION_H_INCLUDE
#include <map>

using namespace std;

extern map<string,string> options;      //将棋所から設定された設定を保持
extern map<char,string> piece_letters;    //駒コードと駒文字(P,+P,r,Rなど)を関係づけている

void init_usi_options(void);
void set_option(USIInputParser &uip);
void set_option_value(const string &option_name,const string &new_value);

#endif
