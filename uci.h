#if !defined(UCI_H_INCLUDE)
#define UCI_H_INCLUDE
using namespace std;
/*
bool begin_thread(void);
*/
//将棋所からのコマンドをパース
class UCIInputParser{
public:
    UCIInputParser(const string &line);
    string get_next_token(void);
    string get_rest_of_line(void);
    bool at_end_of_line(void);
private:
    const string &input_line;
    int length,current_index;
    void skip_whitespace(void);
};

void uci_main_loop(void);
void wait_for_command();
void handle_command(const string &command);
/*
extern CRITICAL_SECTION cs;
*/
#endif