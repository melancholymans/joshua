#include <string>
#include <map>

#include "gtest\gtest.h"
#include "usi.h"
#include "usioption.h"

using namespace std;

map<string,string> options;

void init_usi_options()
{
    options["USI_Ponder"] = "";
    options["USI_Hash"] = "";
    options["BookFile"] = "";
    options["UseBook"] = "";
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

void set_option_value(const string &option_name,const string &new_value)
{
    //初期登録しているオプションしか設定できない
    if(options.find(option_name) != options.end()){
        options[option_name] = new_value;
    }
}
TEST(usoption,set_option_value)
{
    string cmd;

    cmd = "USI_Ponder";
    init_usi_options();
    set_option_value(cmd,"1232");
    ASSERT_EQ("1232",options[cmd]);
}

