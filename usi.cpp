#include "types.h"
#include "common.h"

//名前空間使用宣言
using std::thread;
using Movens::move_from_string;

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
bool CaseInsensitiveLess::operator() (const string& s1, const string& s2) const
{
	return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
		[](char c1, char c2) {return tolower(c1) < tolower(c2); });
}
void Searcher::do_usi_command_loop(int argc, char* argv[])
{
	Position pos(USI::start_sfen, threads.main_thread(), thisptr);
	string token, cmd;

	for (int i = 1; i < argc; i++){
		cmd += string(argv[1]) + " ";
	}

	do{
		if (argc == 1 && !getline(cin, cmd)){
			//改行のみ場合はquit扱い
			cmd = "quit";
		}
		std::istringstream ss_cmd(cmd);
		//std::skipsは空白を読み飛ばす
		ss_cmd >> std::skipws >> token;
		if (token == "quit" || token == "stop" || token == "ponder" || token == "gameover"){
			if (token != "ponder" || signals.stop_on_ponder_hit){
				signals.stop = true;
				threads.main_thread()->notify_one();
			}
			else{
				limits.ponder = false;
			}
			if (token == "ponder" && limits.move_time != 0){
				//経過時間を加算する
				limits.move_time += ptime_now(&search_timer);
			}
		}
		else if (token == "usinewgame"){
			TT.clear();
			for (int i = 0; i < 100; i++){
				//こんな関数はないといわれる
				//std::g_randomTimeSeed();
			}
		}
		else if (token == "usi"){
			sync_cout
				<< "id name joshua"
				<< "\nid author takemori masami"
				<< "\n"
				<< options
				<< "\nusiok"
				<< sync_endl;
		}
		else if (token == "go"){
			USI::go(pos, ss_cmd);
		}
		else if (token == "isready"){
			sync_cout << "readyok" << sync_endl;
		}
		else if (token == "position"){
			USI::set_position(pos, ss_cmd);
		}
		else if (token == "setoption"){
			set_option(ss_cmd);
		}
		else if (token == "bench"){
			benchmark(pos);
		}
		else if (token == "d"){
			Positionns::print_board(pos);
		}
		else{
			sync_cout << "unknow command" << cmd << sync_endl;
		}
	} while (token != "quit" && argc == 1);
	threads.wait_for_think_finised();
}
/*
このような文字列で渡される
"name Threads value 1"
"name MultiPV value 1"
"name OwnBook value false"
"name Max_Random_Score_Diff value 0"
*/
void Searcher::set_option(std::istringstream& uip)
{
	string token, name, value;
	
	uip >> token;	//nameが入る
	uip >> name;	//optionの名前が入る
	while (uip >> token && token != "value"){
		name + " " + token;
	}
	uip >> value;	//valueが入る
	while (uip >> token){
		value += " " + token;
	}
	if (!options.is_legal_option(name)){
		cout << "No  search option: " << name << endl;
	}
	else{
		options[name] = value;
	}
}
//main関数から１回だけ呼ばれるOptionMapはkeyをstring型,valueをOptionクラスで保持するmapコンテナ
void OptionsMap::init(Searcher* sech)
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
//OptionMapのオプションの内容を文字列化して返す。usiコマンドで呼ばれる
//cout << options << endl;のように使う。
std::ostream& operator << (std::ostream& os, const OptionsMap& om)
{
	for (auto& elem : om){
		const Option& opt = elem.second;
		os << "\noption name " << elem.first  << " type " << opt.get_type();
		if (opt.get_type() != "button"){
			os << " default " << opt.get_default_value();
		}
		if (opt.get_type() == "spin"){
			os << " min " << opt.get_min() << " max " << opt.get_max();
		}
	}
	return os;
}
//オプションを設定するときに呼ばれる関数
//char型+関数（省略可）　string型
Option::Option(const char* v, Fn* f) :type("string"), min(0), max(0), idx(options.size()), on_chage(f)
{
	default_value = current_value = v;
}
//bool型+関数（省略可）　check型
Option::Option(bool v, Fn* f) : type("check"), min(0), max(0), idx(options.size()), on_chage(f)
{
	default_value = current_value = (v ? "true" : "false");
}
//関数（省略可）　button型
Option::Option(Fn* f) : type("button"), min(0), max(0), idx(options.size()), on_chage(f)
{}
//int型+int型+int型+関数（省略可）　spin型
Option::Option(int v, int minv, int maxv, Fn* f) : type("spin"), min(minv), max(maxv), idx(options.size()), on_chage(f)
{
	default_value = current_value = std::to_string(v);
}
//intへの変換演算子
Option::operator int() const
{
	_ASSERT(type == "check" || type == "spin");
	return (type == "spin") ? stoi(current_value) : current_value == "true";
}
//stringへの変換演算子
Option::operator string() const
{
	_ASSERT(type == "string");
	return current_value;
}
//代入演算子のオーバライド Option[name] = valueとされたとき起動される演算子オーバーライド
//具体的には	options["Best_Book_Move"] = string("true");と呼び出されたときである。
Option& Option::operator=(const string& v)
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
//USIインターフェイスから呼ばれ、思考開始コマンドｗｐ処理する。
//goコマンドの後に続くオプションをここで設定し、start_thinking関数を呼んで探索を開始させる
void USI::go(const Position& pos, std::istringstream& cmd)
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
//do_usi_command_loopからpositionコマンドから呼ばれる
void USI::set_position(Position& pos, std::istringstream& uip)
{
	string token;
	string sfen;

	uip >> token;
	//平手初期局面
	if (token == "startpos"){
		sfen = USI::start_sfen;
		uip >> token;
	}
	else if (token == "sfen"){
		while (uip >> token && token != "moves"){
			sfen += token + " ";
		}
	}
	else{
		return;
	}
	//指し手再現,局面更新
	pos.position_from_sfen(sfen, pos.get_searcher()->threads.main_thread(), pos.get_searcher());
	//TODO:他にもいろいろ設定しているがわからないのでパス
	while(uip >> token){
		const Move m = move_from_string(pos,token);
		if (m == MoveNone){
			break;
		}
		//TO:StackInfoに関する処理があるが不明のためパス,do_moveに渡しているStateInfoはダミー
		StateInfo st;
		pos.do_move(m, st);
	}
	//TODO:いろいろあるがパス
}
#ifdef _DEBUG
TEST(Options, init)
{
	EXPECT_EQ(static_cast<int>(std::thread::hardware_concurrency()),8);	//この８はマシンによって異なる
	USI::init(options);
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
    std::istringstream uip(command);
	Position pos;
    USI::set_position(pos,uip);

	Positionns::print_board(pos);
	EXPECT_EQ(WLance, pos.get_board(A9));
	EXPECT_EQ(WNight, pos.get_board(B9));
	EXPECT_EQ(EmptyPiece, pos.get_board(C9));
	EXPECT_EQ(EmptyPiece, pos.get_board(D9));
	EXPECT_EQ(EmptyPiece, pos.get_board(E9));
	EXPECT_EQ(EmptyPiece, pos.get_board(F9));
	EXPECT_EQ(BDragon, pos.get_board(G9));
	EXPECT_EQ(WNight, pos.get_board(H9));
	EXPECT_EQ(BProSilver, pos.get_board(I9));

	EXPECT_EQ(EmptyPiece, pos.get_board(A8));
	EXPECT_EQ(EmptyPiece, pos.get_board(B8));
	EXPECT_EQ(EmptyPiece, pos.get_board(C8));
	EXPECT_EQ(EmptyPiece, pos.get_board(D8));
	EXPECT_EQ(EmptyPiece, pos.get_board(E8));
	EXPECT_EQ(EmptyPiece, pos.get_board(F8));
	EXPECT_EQ(EmptyPiece, pos.get_board(G8));
	EXPECT_EQ(BProPawn, pos.get_board(H8));
	EXPECT_EQ(EmptyPiece, pos.get_board(I8));

	EXPECT_EQ(WPawn, pos.get_board(A7));
	EXPECT_EQ(WPawn, pos.get_board(B7));
	EXPECT_EQ(EmptyPiece, pos.get_board(C7));
	EXPECT_EQ(EmptyPiece, pos.get_board(D7));
	EXPECT_EQ(WKing, pos.get_board(E7));
	EXPECT_EQ(WSilver, pos.get_board(F7));
	EXPECT_EQ(EmptyPiece, pos.get_board(G7));
	EXPECT_EQ(EmptyPiece, pos.get_board(H7));
	EXPECT_EQ(WPawn, pos.get_board(I7));

	EXPECT_EQ(EmptyPiece, pos.get_board(A6));
	EXPECT_EQ(EmptyPiece, pos.get_board(B6));
	EXPECT_EQ(BGold, pos.get_board(C6));
	EXPECT_EQ(WPawn, pos.get_board(D6));
	EXPECT_EQ(EmptyPiece, pos.get_board(E6));
	EXPECT_EQ(WGold, pos.get_board(F6));
	EXPECT_EQ(WPawn, pos.get_board(G6));
	EXPECT_EQ(EmptyPiece, pos.get_board(H6));
	EXPECT_EQ(EmptyPiece, pos.get_board(I6));

	EXPECT_EQ(EmptyPiece, pos.get_board(A5));
	EXPECT_EQ(EmptyPiece, pos.get_board(B5));
	EXPECT_EQ(EmptyPiece, pos.get_board(C5));
	EXPECT_EQ(EmptyPiece, pos.get_board(D5));
    EXPECT_EQ(WPawn,pos.get_board(E5));
	EXPECT_EQ(EmptyPiece, pos.get_board(F5));
	EXPECT_EQ(EmptyPiece, pos.get_board(G5));
    EXPECT_EQ(WPawn,pos.get_board(H5));
	EXPECT_EQ(EmptyPiece, pos.get_board(I5));

    EXPECT_EQ(EmptyPiece,pos.get_board(A4));
    EXPECT_EQ(EmptyPiece,pos.get_board(B4));
    EXPECT_EQ(BPawn,pos.get_board(C4));
    EXPECT_EQ(BPawn,pos.get_board(D4));
    EXPECT_EQ(WBishop,pos.get_board(E4));
    EXPECT_EQ(EmptyPiece,pos.get_board(F4));
    EXPECT_EQ(BPawn,pos.get_board(G4));
    EXPECT_EQ(EmptyPiece,pos.get_board(H4));
    EXPECT_EQ(EmptyPiece,pos.get_board(I4));

    EXPECT_EQ(BPawn,pos.get_board(A3));
    EXPECT_EQ(BPawn,pos.get_board(B3));
    EXPECT_EQ(BSilver,pos.get_board(C3));
    EXPECT_EQ(EmptyPiece,pos.get_board(D3));
    EXPECT_EQ(EmptyPiece,pos.get_board(E3));
    EXPECT_EQ(EmptyPiece,pos.get_board(F3));
    EXPECT_EQ(EmptyPiece,pos.get_board(G3));
    EXPECT_EQ(WNight,pos.get_board(H3));
    EXPECT_EQ(BPawn,pos.get_board(I3));

    EXPECT_EQ(EmptyPiece,pos.get_board(A2));
    EXPECT_EQ(BBishop,pos.get_board(B2));
    EXPECT_EQ(WProSilver,pos.get_board(C2));
    EXPECT_EQ(EmptyPiece,pos.get_board(D2));
    EXPECT_EQ(BLance,pos.get_board(E2));
    EXPECT_EQ(EmptyPiece,pos.get_board(F2));
    EXPECT_EQ(WDragon,pos.get_board(G2));
    EXPECT_EQ(EmptyPiece,pos.get_board(H2));
    EXPECT_EQ(EmptyPiece,pos.get_board(I2));

    EXPECT_EQ(BLance,pos.get_board(A1));
    EXPECT_EQ(BNight,pos.get_board(B1));
    EXPECT_EQ(BKing,pos.get_board(C1));
    EXPECT_EQ(EmptyPiece,pos.get_board(D1));
    EXPECT_EQ(EmptyPiece,pos.get_board(E1));
    EXPECT_EQ(EmptyPiece,pos.get_board(F1));
    EXPECT_EQ(EmptyPiece,pos.get_board(G1));
    EXPECT_EQ(EmptyPiece,pos.get_board(H1));
    EXPECT_EQ(BLance,pos.get_board(I1));
    //持ち駒のチエックget_hand(Black, Pawn)
	EXPECT_EQ(0, pos.get_hand(Black,Gold));  //black gold
	EXPECT_EQ(4, pos.get_hand(Black,Pawn));  //black pawn
	EXPECT_EQ(0, pos.get_hand(Black,Lance));  //black lance
	EXPECT_EQ(0, pos.get_hand(Black,Night));  //black knight
	EXPECT_EQ(0, pos.get_hand(Black,Silver));  //black silver
	EXPECT_EQ(0, pos.get_hand(Black,Bishop));  //black bishop
	EXPECT_EQ(0, pos.get_hand(Black,Rook));  //black rook
								 
	EXPECT_EQ(2, pos.get_hand(White,Gold));  //white gold
	EXPECT_EQ(0, pos.get_hand(White,Pawn));  //white pawn
	EXPECT_EQ(0, pos.get_hand(White,Lance));  //white lance
	EXPECT_EQ(0, pos.get_hand(White,Night));  //white knight
	EXPECT_EQ(0, pos.get_hand(White,Silver));  //white silver
	EXPECT_EQ(0, pos.get_hand(White,Bishop));  //white bishop
	EXPECT_EQ(0, pos.get_hand(White,Rook));  //white rook
}
#endif