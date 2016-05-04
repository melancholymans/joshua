#include "types.h"
#include "common.h"

//名前空間使用宣言
using std::thread;
//Global object
OptionsMap options;
extern TranspositionTable TT;

void on_thread(const Option&)
{
}
void on_hash_size(const Option& opt)
{
	//optを引数として渡しているがset_size関数の仮引数型はsize_t型なのでint変換演算子が呼ばれる。
	TT.set_size(opt);
}
void on_clear_hash(const Option&)
{
	TT.clear();
}
//渡された文字列を１文字づつ比較しs1 < s2ならtrueをそれ以外はfalseを返す
bool USI::CaseInsensitiveLess::operator() (const string& s1, const string& s2) const
{
	return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
		[](char c1, char c2) {return tolower(c1) < tolower(c2); });
}
//main関数から１回だけ呼ばれるOptionMapはkeyをstring型,valueをOptionクラスで保持するmapコンテナ
//cout << options << endl;のように使う。
void USI::init(OptionsMap& opt)
{
	const int cpus = std::max(static_cast<int>(std::thread::hardware_concurrency()),1);
	const int min_split_depth = (cpus < 6 ? 4 : (cpus < 8 ? 5 : 7));
	opt["Use_Search_Log"] = Option(false);
	opt["USI_Hash"] = Option(32, 1, 65536, on_hash_size);
	opt["Clear_Hash"] = Option(on_clear_hash);
	opt["Book_File"] = Option("book.bin");
	opt["Best_Book_Move"] = Option(false);
	opt["Own_Book"] = Option(true);
	opt["Min_Book_Ply"] = Option(SHRT_MAX, 0, SHRT_MAX);
	opt["Max_Book_Ply"] = Option(SHRT_MAX, 0, SHRT_MAX);
	opt["Min_Book_Score"] = Option(-180, -ScoreInfinite, ScoreInfinite);
	opt["USI_Ponder"] = Option(true);
	opt["MultiPV"] = Option(1, 1, MAX_LEGAL_MOVE);
	opt["Skill_Level"] = Option(20, 0, 20);
	opt["Max_Random_Score_Diff"] = Option(0, 0, ScoreMate0Ply);
	opt["Max_Random_Score_Diff_Ply"] = Option(40, SHRT_MIN, SHRT_MAX);
	opt["Emergency_Move_Horizon"] = Option(40, 0, 50);
	opt["Emergency_Base_Time"] = Option(200, 0, 30000);
	opt["Emergency_Move_Time"] = Option(70, 0, 5000);
	opt["Slow_Mover"] = Option(100, 10, 1000);
	opt["Minimum_Thinking_Time"] = Option(15400, 0, INT_MAX);
	opt["Min_Split_Depth"] = Option(min_split_depth, 4, 12, on_thread);
	opt["Max_Threads_per_Split_point"] = Option(5, 4, 8, on_thread);
	opt["Threads"] = Option(cpus, 1, MAX_THREADS, on_thread);
	opt["Use_Sleeping_Threads"] = Option(true);
}
//OptionMapのidx順にオプションの内容を文字列化して返す。usiコマンドで呼ばれる
std::ostream& USI::operator << (std::ostream& os, const USI::OptionsMap& om)
{
	for (size_t idx = 0; idx < om.size(); idx++){
		auto it = std::find_if(om.begin(), om.end(), [idx](const OptionsMap::value_type& p)
			{return p.second.idx == idx; });
		const Option& opt = it->second;
		os << "\noption name " << it->first << " type " << opt.type;
		if (opt.type != "buttom"){
			os << " default: " << opt.default_value;
		}
		if (opt.type == "spin"){
			os << " min " << opt.min << " max " << opt.max;
		}
	}
	return os;
}

//オプションを設定するときに呼ばれる関数
//char型+関数（省略可）　string型
USI::Option::Option(const char* v, Fn* f) :type("string"), min(0), max(0), idx(options.size()), on_chage(f)
{
	default_value = current_value = v;
}
//bool型+関数（省略可）　check型
USI::Option::Option(bool v, Fn* f) : type("check"), min(0), max(0), idx(options.size()), on_chage(f)
{
	default_value = current_value = (v ? "true" : "false");
}
//関数（省略可）　button型
USI::Option::Option(Fn* f) : type("button"), min(0), max(0), idx(options.size()), on_chage(f)
{}
//int型+int型+int型+関数（省略可）　spin型
USI::Option::Option(int v,int minv,int maxv,Fn* f) : type("spin"), min(minv), max(maxv), idx(options.size()), on_chage(f)
{
	default_value = current_value = std::to_string(v);
}
//intへの変換演算子
USI::Option::operator int() const
{
	_ASSERT(type == "check" || type == "spin");
	return (type == "spin") ? stoi(current_value) : current_value == "true";
}
//stringへの変換演算子
USI::Option::operator string() const
{
	_ASSERT(type == "string");
	return current_value;
}
//代入演算子のオーバライド Option[name] = valueとされたとき起動される演算子オーバーライド
//具体的には	options["Best_Book_Move"] = string("true");と呼び出されたときである。
USI::Option& Option::operator=(const string& v)
{
	_ASSERT(!type.empty());
	if ((type != "button" && v.empty())
		|| (type == "check" && v != "true" && v != "false")
		|| (type == "spin" && (stoi(v) < min || stoi(v) > max))){
		return *this;
	}
	if (type != "button"){
		current_value = v;
	}
	if (on_chage){
		(*on_chage)(*this);
	}
	return *this;
}

void USI::usi_main_loop(void)
{
    string token,cmd;
	Position pos(start_sfen);

    do{
		if (!getline(cin, cmd)){
            //改行のみ場合はquit扱い
            cmd = "quit";
        }

    }while(handle_command(cmd));
}


//usi->isready->usinewgameで対局開始
//positionで思考局面の付与->goで思考
//bestmoveで思考結果を返信,gameoverで終局
bool USI::handle_command(const string &command)
{
	string token;
    std::stringstream uip(command);

	uip >> std::skipws >> token;
    if(token == "quit"){
        //後始末
        return false;
    }
	/*
    else if(token == "usi"){
        sync_cout	<< "id name " << "joshua" << "\n"
					<< "id author takemori masami"
					//optionがあったらここで送信
					<< "option name BookFile value public.bin"
					<< "option name UseBook type default true"
					<< "usiok" << sync_endl;
    }
    else if(token == "isready"){
        sync_cout << "readyok" << sync_endl;
    }
    else if(token == "usinewgame"){
        //色々初期など準備,このコマンドに対して将棋所に返すものはない
        //from_sfen(start_position);
        //game_init(root_position);
    }
    else if(token == "position"){
        //エンジンに思考させる局面を逐一送ってくるので解釈させ内部のデータ構造に変換する
        set_position(pos,uip);
    }
    else if(token == "setoption"){
        //エンジンに対する値を設定するとき送信
        //set_option(uip);
    }
    else if(token == "go"){
        //思考開始
        //go();
    }
    else if(token == "d"){
        //print_board(root_position);
    }
    else if(token == "csa"){
        //csa(uip);
    }
    else if(token == "gameover"){
        //GUI側からの終局の通知
    }
    else{
		sync_cout << "Unknow command" << uip.str() << sync_endl;
    }
	*/
    return true;    //明示的な終了以外は動作継続(ユーザーからの終了コマンド(quit)のみ)
}

void set_position(std::stringstream& uip)
{
	/*
    string token;
    short dummy[8];

    uip >> token;
    //平手初期局面
    if(token == "startpos"){
        from_sfen(start_position);
    }
    //駒落ち初期局面
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    //指し手再現,局面更新
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token(); //cmdには指し手ごと分割されて渡す
                Move m = move_from_string(root_position,cmd);
                DoMove(root_position.turn,root_position,m,dummy);
            }
        }
    }
	*/
}

/*
void game_init(const Position &pos)
{
//対局ごと初期化する必要がある変数
next_move[0].last_move = mlist;
next_modify[0].last_dirty = modifylist;
sech.material = eval_material(pos);
}
*/

//USIインターフェイスから呼ばれ、思考開始コマンドｗｐ処理する。
//goコマンドの後に続くオプションをここで設定し、start_thinking関数を呼んで探索を開始させる
void go(const Position& pos,std::istringstream& cmd)
{
	LimitsType limits;
	std::vector<Move> moves;
	string token;

	while (cmd >> token){
		if (token == "ponder"){
			limits.ponder = true;
		}
		else if (token == "btime"){
			cmd >> limits.time[Black];
		}
		else if (token == "wtime"){
			cmd >> limits.time[White];
		}
		else if (token == "infinite"){
			limits.infinite = true;
		}
		else if (token == "depth"){
			cmd >> limits.depth;
		}
	}
	//ss_cmdでlimitsを設定する
	pos.get_searcher()->threads.start_thinking(pos, limits, moves);
}

#ifdef _DEBUG
TEST(Options, init)
{
	EXPECT_EQ(static_cast<int>(std::thread::hardware_concurrency()),8);	//この８はマシンによって異なる
	USI::init(options);
	cout << options << endl;	//目視で画面を確認
	EXPECT_EQ(options["Use_Search_Log"],false);
	EXPECT_EQ(options["USI_Hash"], 32);
	TT.set_size(options["USI_Hash"]);
	EXPECT_EQ(TT.get_size(), 2097152 - 4);	//2097152はoptions["USI_Hash"]に32が設定された場合の数
	string str1 = options["Book_File"];
	string str2("book.bin");
	EXPECT_EQ(str1, str2);
	EXPECT_EQ(options["Best_Book_Move"], false);
	EXPECT_EQ(options["Own_Book"], true);
	EXPECT_EQ(options["Min_Book_Ply"], SHRT_MAX);
	EXPECT_EQ(options["Max_Book_Ply"], SHRT_MAX);
	EXPECT_EQ(options["Min_Book_Score"], -180);
	EXPECT_EQ(options["USI_Ponder"], true);
	EXPECT_EQ(options["MultiPV"], 1);
	EXPECT_EQ(options["Skill_Level"], 20);
	EXPECT_EQ(options["Max_Random_Score_Diff"], 0);
	EXPECT_EQ(options["Max_Random_Score_Diff_Ply"], 40);
	EXPECT_EQ(options["Emergency_Move_Horizon"], 40);
	EXPECT_EQ(options["Emergency_Base_Time"], 200);
	EXPECT_EQ(options["Emergency_Move_Time"], 70);
	EXPECT_EQ(options["Slow_Mover"], 100);
	EXPECT_EQ(options["Minimum_Thinking_Time"], 15400);
	EXPECT_EQ(options["Min_Split_Depth"], 7);	//このテストまテストするPCによって答えが違う
	EXPECT_EQ(options["Max_Threads_per_Split_point"], 5);
	EXPECT_EQ(options["Threads"], 8);	//このテストまテストするPCによって答えが違う
	EXPECT_EQ(options["Use_Sleeping_Threads"], true);
	//option値変更ルーチンチエック
	options["Use_Search_Log"] = string("true");
	EXPECT_EQ(options["Use_Search_Log"], true);
	options["USI_Hash"] = string("64");
	EXPECT_EQ(options["USI_Hash"], 64);
	options["Book_File"] = string("book2.bin");
	str1 = options["Book_File"];
	str2 = string("book2.bin");
	EXPECT_EQ(str1, str2);
	options["Best_Book_Move"] = string("true");
	EXPECT_EQ(options["Best_Book_Move"], true);
	options["Own_Book"] = string("false");
	EXPECT_EQ(options["Own_Book"], false);
	options["Min_Book_Ply"] = string("123");
	EXPECT_EQ(options["Min_Book_Ply"], 123);
	options["Max_Book_Ply"] = string("456");
	EXPECT_EQ(options["Max_Book_Ply"], 456);
	options["Min_Book_Score"] = string("270");
	EXPECT_EQ(options["Min_Book_Score"], 270);
	options["USI_Ponder"] = string("false");
	EXPECT_EQ(options["USI_Ponder"], false);
	options["MultiPV"] = string("789");
	EXPECT_EQ(options["MultiPV"], 789);
	options["Skill_Level"] = string("19");
	EXPECT_EQ(options["Skill_Level"], 19);
	options["Max_Random_Score_Diff"] = string("314");
	EXPECT_EQ(options["Max_Random_Score_Diff"], 314);
	options["Max_Random_Score_Diff_Ply"] = string("3014");
	EXPECT_EQ(options["Max_Random_Score_Diff_Ply"], 3014);
	options["Emergency_Move_Horizon"] = string("45");
	EXPECT_EQ(options["Emergency_Move_Horizon"], 45);
	options["Emergency_Base_Time"] = string("10448");
	EXPECT_EQ(options["Emergency_Base_Time"], 10448);
	options["Emergency_Move_Time"] = string("224");
	EXPECT_EQ(options["Emergency_Move_Time"], 224);
	options["Slow_Mover"] = string("159");
	EXPECT_EQ(options["Slow_Mover"], 159);
	options["Minimum_Thinking_Time"] = string("6626");
	EXPECT_EQ(options["Minimum_Thinking_Time"], 6626);
	options["Min_Split_Depth"] = string("11");
	EXPECT_EQ(options["Min_Split_Depth"], 11);	//このテストまテストするPCによって答えが違う
	options["Max_Threads_per_Split_point"] = string("6");
	EXPECT_EQ(options["Max_Threads_per_Split_point"], 6);
	options["Threads"] = string("32");
	EXPECT_EQ(options["Threads"], 32);	//このテストまテストするPCによって答えが違う
	options["Use_Sleeping_Threads"] = string("false");
	EXPECT_EQ(options["Use_Sleeping_Threads"], false);
}
TEST(usi,set_position)
{
    string command("startpos moves 7g7f 3c3d 2g2f 4c4d 3i4h 3a4b 5g5f 5c5d 2f2e 2b3c 5i6h 4b4c 6h7h 7a6b 7i6h 6b5c 4i5h 7c7d 6g6f 4a3b 6h7g 4d4e 5h6g 5a4b 3g3f 5d5e 2i3g 5c5d 5f5e 3c5e 2e2d 2c2d 2h2d 4b3a P*2c 4e4f 4g4f P*4g 4h4g 6a5b 3g4e 5d4e 4f4e 5e3g+ S*2b 3a4b 2b1a+ 3b3c 2d2e P*2d P*5c 2d2e 5c5b+ 8b5b G*3h 3g3h 4g3h R*4h L*5h 4h3h+ 2c2b+ N*2g B*3a 4b4a P*5c 5b3b 4e4d 3c4d P*4b 3b4b 3a4b+ 4a4b R*7a P*5g 7a3a+ 4b5c 6g5g P*5f 5g5f P*5e 5f6e 6c6d 6e7d B*5f 7h6h S*6g 6h7i P*7h 6i7h 6g7h+");
    std::stringstream uip(command);
    //set_position(uip);
	/*
    EXPECT_EQ(W_LANCE,root_position.board[SQ_9A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_8A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4A]);
    EXPECT_EQ(BP_ROOK,root_position.board[SQ_3A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2A]);
    EXPECT_EQ(BP_SILVER,root_position.board[SQ_1A]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3B]);
    EXPECT_EQ(BP_PAWN,root_position.board[SQ_2B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1B]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_9C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_8C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6C]);
    EXPECT_EQ(W_KING,root_position.board[SQ_5C]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_4C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_1C]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8D]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_7D]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_6D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5D]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_4D]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_3D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1D]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6E]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_5E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3E]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_2E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1E]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_7F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_6F]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_5F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_3F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1F]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_9G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_8G]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_7G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3G]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_1G]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9H]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_8H]);
    EXPECT_EQ(WP_SILVER,root_position.board[SQ_7H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6H]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_5H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4H]);
    EXPECT_EQ(WP_ROOK,root_position.board[SQ_3H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1H]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_9I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_8I]);
    EXPECT_EQ(B_KING,root_position.board[SQ_7I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2I]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_1I]);
    //持ち駒のチエック
    EXPECT_EQ(0,root_position.board[208]);  //black gold
    EXPECT_EQ(4,root_position.board[209]);  //black pawn
    EXPECT_EQ(0,root_position.board[210]);  //black lance
    EXPECT_EQ(0,root_position.board[211]);  //black knight
    EXPECT_EQ(0,root_position.board[212]);  //black silver
    EXPECT_EQ(0,root_position.board[213]);  //black bishop
    EXPECT_EQ(0,root_position.board[214]);  //black rook

    EXPECT_EQ(2,root_position.board[215]);  //white gold
    EXPECT_EQ(0,root_position.board[216]);  //white pawn
    EXPECT_EQ(0,root_position.board[217]);  //white lance
    EXPECT_EQ(0,root_position.board[218]);  //white knight
    EXPECT_EQ(0,root_position.board[219]);  //white silver
    EXPECT_EQ(0,root_position.board[220]);  //white bishop
    EXPECT_EQ(0,root_position.board[221]);  //white rook
	*/
}
#endif