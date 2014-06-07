#if !defined(USI_H_INCLUDE)
#define USI_H_INCLUDE

using namespace std;

//����������̃R�}���h���p�[�X
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
void wait_for_command();
void handle_command(const string &command);
void set_position(USIInputParser &uip);

#endif