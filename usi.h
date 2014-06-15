#if !defined(USI_H_INCLUDE)
#define USI_H_INCLUDE

#include <map>

using namespace std;

#include "types.h"
#include "position.h"

//将棋所からのコマンドをパース
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


void usi_main_loop(void);
void game_init(void);
bool handle_command(const string &command);
void set_position(USIInputParser &uip);
Move move_from_string(const Position &pos, const string &cmd);
int square_from_string(const string sq);
void go(void);

#endif
