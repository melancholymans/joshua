#if !defined(USI_H_INCLUDE)
#define USI_H_INCLUDE

//#include "types.h"
namespace USI{
	//局面を表す文字列
	//先手大文字、後手小文字、数字は空白、/は行区切り +は成駒の印
	const string start_sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
	void usi_main_loop(void);
	bool handle_command(const string &command);
	/*
	void set_position(stringstream& uip)
	void go(void)
	*/
};

/*
void game_init(const Position &pos);
bool handle_command(const string &command);
void set_position(USIInputParser &uip);
void go(void);
*/
#endif
