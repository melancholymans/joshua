#include <string>
#include <map>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "usi.h"
#include "usioption.h"

map<string,string> options;
map<char,string> piece_letters;
map<string,char> csa_letters;

void init_usi_options()
{
    options["USI_Ponder"] = "";
    options["USI_Hash"] = "";
    options["BookFile"] = "";
    options["UseBook"] = "";
    
    piece_letters[BP_PAWN] = "+P";
    piece_letters[BP_LANCE] = "+L";
    piece_letters[BP_KNIGHT] = "+N";
    piece_letters[BP_SILVER] = "+S";
    piece_letters[BP_BISHOP] = "+B";
    piece_letters[BP_ROOK] = "+R";
    piece_letters[B_KING] = "K";
    piece_letters[B_GOLD] = "G";
    piece_letters[B_PAWN] = "P";
    piece_letters[B_LANCE] = "L";
    piece_letters[B_KNIGHT] = "N";
    piece_letters[B_SILVER] = "S";
    piece_letters[B_BISHOP] = "B";
    piece_letters[B_ROOK] = "R";

    piece_letters[WP_PAWN] = "+p";
    piece_letters[WP_LANCE] = "+l";
    piece_letters[WP_KNIGHT] = "+n";
    piece_letters[WP_SILVER] = "+s";
    piece_letters[WP_BISHOP] = "+b";
    piece_letters[WP_ROOK] = "+r";
    piece_letters[W_KING] = "k";
    piece_letters[W_GOLD] = "g";
    piece_letters[W_PAWN] = "p";
    piece_letters[W_LANCE] = "l";
    piece_letters[W_KNIGHT] = "n";
    piece_letters[W_SILVER] = "s";
    piece_letters[W_BISHOP] = "b";
    piece_letters[W_ROOK] = "r";

    csa_letters["FU"] = 10;
    csa_letters["KY"] = 11;
    csa_letters["KE"] = 12;
    csa_letters["GI"] = 13;
    csa_letters["KI"] = 9;
    csa_letters["KA"] = 14;
    csa_letters["HI"] = 15;
    csa_letters["OU"] = 8;
    csa_letters["TO"] = 2;
    csa_letters["NY"] = 3;
    csa_letters["NK"] = 4;
    csa_letters["NG"] = 5;
    csa_letters["UM"] = 6;
    csa_letters["RY"] = 7;
    csa_letters["* "] = 0;   //駒文字ではないが
    return;
}

//stockfishを参考にvalue値を送ってくるオプションにはとりあえず対応
void set_option(USIInputParser &uip)
{
    string cmd;

    if(!uip.at_end_of_line()){
        cmd = uip.get_next_token();
        if(cmd == "name" && !uip.at_end_of_line()){
            string name = uip.get_next_token();
            string next_cmd;
            while(!uip.at_end_of_line() && (next_cmd = uip.get_next_token()) != "value"){
                name += (" " + next_cmd);
            }
            if(next_cmd == "value"){
                set_option_value(name,uip.get_rest_of_line());
            }
        }
    }
}

void set_option_value(const string &option_name,const string &new_value)
{
    //初期登録しているオプションしか設定できない
    if(options.find(option_name) != options.end()){
        options[option_name] = new_value;
    }
}

TEST(usoption,set_option)
{
    init_usi_options();
    string command("setoption name USI_Ponder value true");
    USIInputParser uip(command);
    string cmd = uip.get_next_token();

    set_option(uip);
    ASSERT_EQ("true",options["USI_Ponder"]);

    command = "setoption name USI_Hash value 32";
    USIInputParser uip1(command);
    cmd = uip1.get_next_token();
    set_option(uip1);
    ASSERT_EQ("32",options["USI_Hash"]);
    //初期化されていない（事前に登録されていないオプション項目は設定しない
    command = "setoption name USI_Option value 123";
    USIInputParser uip2(command);
    cmd = uip1.get_next_token();
    set_option(uip2);
    ASSERT_EQ("",options["USI_Option"]);
}

TEST(usoption,set_option_value)
{
    string cmd;

    cmd = "USI_Ponder";
    init_usi_options();
    set_option_value(cmd,"1232");
    ASSERT_EQ("1232",options[cmd]);
}

