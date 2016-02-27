#if !defined(USI_H_INCLUDE)
#define USI_H_INCLUDE

#include "types.h"
#include "search.h"

	namespace USI{
		//先行宣言
		class Option;

		struct CaseInsensitiveLess
		{
			bool operator() (const string&, const string&) const;
		};

		typedef map<string, Option, CaseInsensitiveLess> OptionsMap;

		class Option{
			//オプションを設定、変更するときに呼び出される関数ポインタの宣言
			typedef void(Fn)(const Option&);
		public:
			//optionのとり方は４種類ある
			//関数（省略可能）      buttn型
			//bool+関数（省略可能） check型
			//char+関数（省略可能） string型
			//int+関数（省略可能）　spin型
			Option(Fn* = nullptr);
			Option(bool v, Fn* = nullptr);
			Option(const char* v, Fn* = nullptr);
			Option(int v, int min, int max, Fn* = nullptr);
			//代入演算子のオーバライド
			Option& operator = (const string& v);
			//intへの変換演算子
			operator int() const;
			//stringへの変換演算子
			operator string() const;
		private:
			friend std::ostream& operator << (std::ostream&, const OptionsMap&);
			string default_value;
			string current_value;
			string type;
			int min, max;
			size_t idx;
			Fn* on_chage;
		};
		void init(OptionsMap&);
		void loop(const string&);


		//局面を表す文字列
		//先手大文字、後手小文字、数字は空白、/は行区切り +は成駒の印
		const string start_sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
		void usi_main_loop(void);
		bool handle_command(const string &command);
		/*
		void set_position(stringstream& uip)
		void go(void)
		*/
	}	//namespace end
	
	extern USI::OptionsMap options;
#endif
