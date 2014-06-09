#if !defined(USIOPTION_H_INCLUDE)
#define USIOPTION_H_INCLUDE
#include <map>

using namespace std;

extern map<string,string> options;      //«ŠûŠ‚©‚çİ’è‚³‚ê‚½İ’è‚ğ•Û
extern map<char,string> piece_letters;    //‹îƒR[ƒh‚Æ‹î•¶š(P,+P,r,R‚È‚Ç)‚ğŠÖŒW‚Ã‚¯‚Ä‚¢‚é

void init_usi_options(void);
void set_option(USIInputParser &uip);
void set_option_value(const string &option_name,const string &new_value);

#endif
