#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>>
#include <stdbool.h>
#include <string.h>>
#include <stdarg.h>
#include <time.h>
#include <math.h>
//#include <threads.h>
#include <assert.h>
#include <float.h>

#include "quickRefC.h"

void func0() { puts("This is function func0"); }
void func1() { puts("This is function func1"); }

int type_conversion() {
	double dVar = 3.14159;
	dVar *= 3;
	if (dVar > 10L) {		//10Lはlong型のリテラルで、dVarはdouble型なので、比較の際にlong型がdouble型に変換される
		printf("dVar is greater than 10\n");
	}
	else {
		printf("dVar is not greater than 10\n");
	}
	//-----------------------------この辺の暗黙変換怖い--------------------------
	//配列の名前は暗黙的にポインタに変換される
	int iArray[] = { 1, 2, 3, 4, 5 };
	int array_len = sizeof(iArray) / sizeof(iArray[0]);		//sizeof(iArray)に配列の名前を渡しているが、ポインタには型変換されず、配列のサイズが返される
	printf("array_len: %d\n", array_len);
	int* iPtr = iArray + 3;		//C言語はポインタ計算ができる。
	printf("iArray[4]=%d\n", *iPtr);
	//文字配列をstrlenとsizeofで比較したとき数値が違う
	char msg[80] = "I'm a string literal.";
	printf("The string %d char long\n", (int)strlen(msg));	//文字列の長さ（ヌル文字列までの長さ）を返している,strlenが返すのはsize_t(long log型)なのでintにキャストしている
	printf("The array named msg is %zu byte long\n", sizeof(msg));	//msg配列のサイズを返している,sizeofが返すのはsize_t(long log型)なのでintにキャストするのではなく%zuで表示するとワーニングがでない%zuはC99で導入されたフォーマット指定子で、size_t型の値を表示するために使用されます
	//関数名は関数へのポインタに暗黙的に変換される
	void (*funcTable[2])(void) = { func0, func1 };	//関数ポインタの配列を定義している,上に定義している関数のポインタを配列に格納している
	//文字配列と関数配列を比較してみる
	char* strTable[2] = { "func0", "func1" };	//文字配列の配列を定義している
	/*まず最初にchar が書いてある、これは要素である文字列の型である
	関数ポインタ配列でこれに相当するのが返り値のvoidである、つまり関数の返り値の型がcharに相当する
	次にstrTable[2]とあるのが配列の名前と要素数
	関数ポインタ配列でこれに相当するのが(*funcTable[2])である、なぜカッコで括ってあるのかは理由があるがここでは言及しない
	次に文字配列では{ "func0", "func1" }となっている。
	関数ポインタ配列では{ func0, func1 }となっている、文字配列では文字列リテラルを格納しているが、関数ポインタ配列では関数の名前を格納している。関数の名前は関数へのポインタに暗黙的に変換されるので、
	関数ポインタ配列には関数へのポインタが格納されていることになる
	まず驚きなのが関数の型がvoidであること、intを返す関数の時はここがintになるはず。
	次に驚きなのが引数も型の一部であることである
	関数ポインタ配列を使うときは()を付ける必要がある。()は関数呼び出し演算子で関数ポインタだけでは関数を呼び出すことができない
	*/
	for (int i = 0; i < 2; i += 1) {
		funcTable[i]();	//関数ポインタを使って関数を呼び出している
	}
	//どんな型でもchar型(char, signed char,unsigned char)に変換すると結果はオブジェクトの先頭バイトのポインタになる。これを利用して構造体の先頭バイトをchar型のポインタに変換して、構造体のメンバ変数の値をバイト単位で取得することができる
	struct Data {
		short id;
		short val;
	};
	struct Data data = { 0x123, 0x456 };
	unsigned char* cp = (unsigned char*)&data;	//構造体の先頭バイトのポインタを取得している
	printf("%p: ", cp);
	for (int i = 0; i < sizeof(data); i += 1) {
		printf("%02X ", *(cp + i));	//構造体の先頭バイトから順にバイト単位で取得している)
	}
	putchar('\n');
	//関数ポインタを異なる関数へのポインタに明示的に変換できる（反対に暗黙的に変換するとバグの原因となる）
	//typedef文は「double引数を１つ持ちdouble値を返す関数」型の名前を定義している
	typedef double(func_t)(double);	//func_tという型ではない。double引数を１つ持ちdouble値を返す関数型の名前をfunc_tとして定義しているだけ
	func_t* pFunc = sqrt;	//sqrt関数のポインタを取得している
	printf("The sqrt %f.\n", pFunc(2.0));	//関数ポインタを使って関数を呼び出している)
	pFunc = (func_t*)pow;	//pow関数のポインタに変更しているがpow関数はdouble引数を２つ持ちdouble値を返す関数なので、NGとなる,呼び出せるがおかしな値が返ってくる
	//printf("The pow %f.\n", pFunc(2.0,3.0));
	//voidへのポインタ変換
	//void型は「型がない」ことを意味するので、void型のポインタは「どんな型のポインタでも格納できる」ことを意味する。ある意味万能です
	return 1;
}

/*ヘッダファイルが作らなくてもよい理由
	C言語（特に古いC89規格など）には、「プロトタイプ宣言がない関数を突然呼び出しても、コンパイラが自動で『戻り値が int 型の関数』と仮定してコンパイルを継続する」という仕様（暗黙の関数宣言）が存在します
	なのでヘッダファイルを用意しなくてもリンク実行ができる
*/
int sizeof_basic_types() {
	printf("unit ---byte--- \n");
	printf("sizeof(char) = %zu\n", sizeof(char));
	printf("sizeof(short) = %zu\n", sizeof(short));
	printf("sizeof(int) = %zu\n", sizeof(int));
	printf("sizeof(long) = %zu\n", sizeof(long));
	printf("sizeof(long long) = %zu\n", sizeof(long long));
	printf("sizeof(float) = %zu\n", sizeof(float));
	printf("sizeof(double) = %zu\n", sizeof(double));
	printf("sizeof(size_t) = %zu\n", sizeof(size_t));
	return 1;
}

int float_learn() {
	puts("\nCharacteristics of the type float\n");
	printf("Storage size: %d bytes\n"
		"Smallest positive value: %E\n"
		"Greatest positive value: %E\n"
		"Precision: %d decimal digits\n",
		sizeof(float), FLT_MIN, FLT_MAX, FLT_DIG);

	puts("\nAn example of float precision:\n");
	double d_var = 12345.6;          // A variable of type double.
	float f_var = (float)d_var;      // Initializes the float
	// variable with the value of d_var.
	printf("The floating-point number    "
		"%18.10f\n", d_var);
	printf("has been stored in a variable\n"
		"of type float as the value   "
		"%18.10f\n", f_var);
	printf("The rounding error is        "
		"%18.10f\n", d_var - f_var);
	return 1;
}

int enum_learn() {
	enum Color { RED, GREEN, BLUE };
	enum Color myColor = GREEN;
	printf("myColor = %d\n", myColor);
	myColor = RED;
	printf("myColor = %d\n", myColor);
	printf("myColor = %d\n", RED);
	printf("myColor = %d\n", GREEN);
	printf("myColor = %d\n", BLUE);
	enum LANG { C = 1, CPP, JAVA, PYTHON };
	enum LANG lang = CPP;
	printf("myColor = %d\n", lang);

	enum { japan, France, Germany, China, US };	//匿名列挙型といい、列挙型の名前を付けないこともできる
	printf("contry = %d\n", Germany);
	return 1;
}


enum { ARR_LEN = 100 };

int void_learn() {
	int i,                              // Obtain some storage space.
		* pNumbers = malloc(ARR_LEN * sizeof(int));
	if (pNumbers == NULL) {
		fprintf(stderr, "Insufficient memory.\n");
		exit(1);
	}
	srand((unsigned)time(NULL));      // Initialize the
	// random number generator.
	for (i = 0; i < ARR_LEN; ++i) {
		pNumbers[i] = rand() % 10000;    // Store some random numbers.
	}
	printf("\n%d random numbers between 0 and 9999:\n", ARR_LEN);
	for (i = 0; i < ARR_LEN; ++i) {
		printf("%6d", pNumbers[i]);       // Print one number per loop
		if (i % 10 == 9) {
			putchar('\n'); // iteration and a newline after
		}// every 10 numbers.
	}
	free(pNumbers);                   // Release the storage space.
	return 1;
}

int error_exit(unsigned int error_n)   // Print a last error message
{                                       // and exit the program.
	char* error_msg[] = { "Unknown error code.\n",
						   "Insufficient memory.\n",
						   "Illegal memory access.\n" };
	unsigned int arr_len = sizeof(error_msg) / sizeof(char*);

	if (error_n >= arr_len) {
		error_n = 0;
	}
	fputs(error_msg[error_n], stderr);
	return 1;
}


enum { e_unknown, e_nomem, e_badaddr };

int string_literal_learn() {
	char msg[] = "The installation of is now complete.";
	puts(msg);
	char* p = msg + 100;
	if (p > msg + strlen(msg)) {
		error_exit(e_badaddr);
	}
	puts(p);
	return 1;
}

//ポインタ演算
int pointer_learn() {
	double dArr[5] = { 0.0,1.1, 2.2, 3.3, 4.4 };
	double* p = dArr;
	p += 1;	//ポインタの加算、dPtrはdouble型のポインタなので、dPtr+1はdPtrが指すアドレスから8バイト進んだアドレスを指す
	p += 2;	//ポインタの加算、dPtrはdouble型のポインタなので、dPtr+2はdPtrが指すアドレスから16バイト進んだアドレスを指す
	printf("%f\n", *p);
	printf("dArr - p =%zu\n", p - dArr);	//ポインタ間の差はptrdiff_tの型となる符号付の整数(long log型)である	stddef.hに定義してある
	return 1;
}

//増分減分演算子
int incremental_learn() {
	char a[10] = "Jim";
	int i = 0;
	printf("%c\n", a[i++]);	//出力 J printfを実行してからiが増分される。後置きの特徴
	printf("%c\n", a[++i]);	//出力 m iが増分されてからprintfを実行される。前置きの特徴

	i = 0;
	printf("%c\n", ++a[i]);	//出力 L a[i]はJなのでこれに１を増分するとJ->Lになる
	return 1;
}

int struct_op() {
	//定義
	struct Article {
		long number;
		char name[32];
		long price;
	};
	//変数宣言、初期化
	struct Article sw = { 102030L,"Heroes",5995L };
	sw.price = 4995L;
	printf("struct name %s", sw.name);
	//	アロー演算子
	struct Article* pArticle = &sw;
	pArticle->number += 1;
	printf("Article %d\n", pArticle->number);
	printf("Article %s\n", pArticle->name);
	printf("Article %d\n", pArticle->price);

	printf("Article %d\n", (*pArticle).number);	//これはアロー演算子と同じ
	printf("Article %s\n", (*pArticle).name);	//これはアロー演算子と同じ
	printf("Article %d\n", (*pArticle).price);	//これはアロー演算子と同じ
	return 1;
}

int Alignof_learn() {
	/*_Alignof は、指定した型または式の「アラインメント要件（alignment requirement）」を取得する演算子です。
	C11 から標準で導入されました。
	戻り値は size_t 型で、バイト単位のアラインメント値を表します。
	「アラインメント」とは、その型のオブジェクトがメモリ上でどのアドレス境界に配置されるべきか（何バイトごとの境界に揃えるべきか）というメモリ配置の制約です。
	このAlignofはその変数、構造体をアライメントに揃える命令ではない*/
	printf("Alignof char %zu\n", _Alignof(char));
	printf("Alignof int %zu\n", _Alignof(int));
	printf("Alignof int64_t %zu\n", _Alignof(int64_t));

	printf("Alignof char* %zu\n", _Alignof(char*));
	printf("Alignof int* %zu\n", _Alignof(int*));
	return 1;
}

static int help_func() {
	puts("help function");
}

extern int function_learn() {
	//関数の記憶域クラス指定子
	//関数のデフォルトの記憶域クラス指定子はexternである。なのでどのソースファイルからでもアクセスできる。なのでわざわざこのexternを付ける必要はない
	//宣言は必要で使用する前に（定義する前）宣言できていれば良い。またC言語では関数内関数は定義できない。
	//関数を他のファイルからの呼び出しを隠ぺいするにはstaticキーワードをつけるとよい
	help_func();
	return 1;
}

//この関数はシグネェクチャがint funct_name()ではないのでヘッダファイルがないと返り値がおかしくなるバグが発生する
int factorial(register unsigned int n) {
	//関数のregister指定子はコンパイラに変数にできる限りアクセスするように要請する、無視されることもある
	double f = 1.0L;
	while (n > 1) {
		f *= n--;
	}
	printf("%f\n", f);
	return 1;
}

//関数に配列を渡す時のフォーマット、配列を関数に渡すと実質配列のポインタを渡すことと等価、配列の大きさは無視されるので別途配列の大きさを渡す必要がある
int addArray(register float a1[], register float a2[], const int len) {
	register float* end = a1 + len;
	for (; a1 < end; a1++, a2++) {
		*a1 += *a2;
	}
	printf("addArray\n");
	for (int i = 0; i < 9; i += 1) {
		printf("%.2f ", a2[i]);
	}
	putchar('\n');
	return 1;
}

//構造体を関数に渡す
//文字列の取り扱いは注意が必要
//strcpy関数は使用禁止必ずstrcpy_sを使うこと
int initNode(struct Node_Type* pNode) {
	memset(pNode, 0, sizeof(*pNode));
	char s[] = "Hello";
	strcpy_s(pNode->name, strlen(s) + 1, s);
	return 1;
}

//inline関数
inline void swapf(float* p1, float* p2) {
	const float tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void selection_sortf(float a[], int n) {
	for (int i = 0; i < n - 1; ++i) {
		int mini = i;
		for (int j = i + 1; j < n; ++j) {
			if (a[j] < a[mini]) {
				mini = j;
			}
		}
		swapf(a + i, a + mini);
	}
}

const float* binarySearch(const float val, const float array[], const int n) {
	const int m = n / 2;
	if (n <= 0) { return NULL; }
	if (val == array[m]) { return array + m; }
	if (val < array[m]) { return binarySearch(val, array, m); }
	else { return binarySearch(val, array + m + 1, n - m - 1); }
}

//可変引数
/*
stdarg.hをインクルードすること
void va_start(va_list argptr,lastparam)
	マクロva_startは引数ポインタargptrの最初のポインタ引数の位置に初期化されている
	マクロの第2引数は関数の最後の名前付き引数の名前出なければならない
	このマクロは関数がオプション引数を妻う前に呼び出す
type va_arg(va_list argptr,type)
	マクロva_argは現在argptrが参照しているオプション引数を取り出し、argptr
	を次の引数に進める。第2引数は読み込まれている引数の型である
void va_end(valist argptr)
	引数ポインタをもう使わないならこのマクロva_endを呼ぶ
void va_copy(va_list dest,va_list src)
	マクロva_copy引数ポインタdestをsrcの現在値で初期化する。destにあるコピーを使って
	オプション引数のリストに,srcで参照されている位置からサイドアクセスできる

注意事項
	可変引数で実数（浮動小数点数）を受け取る場合は、必ず double型 を指定して va_arg(argptr, double) と書く必要があります。
	決定的な注意点：va_arg のハマりどころ
		va_arg(ap, float) や va_arg(ap, char) は未定義動作
		C言語規格（ISO C）では、昇格後の型と異なる型を va_arg に指定することを明確に禁止しています。
		// ❌ 誤り：未定義動作になる（クラッシュや値の破損の原因）
		float f = va_arg(argptr, float);
		char c = va_arg(argptr, char);

		// ⭕️ 正解：昇格後の型で受け取り、必要ならキャストする
		float f = (float)va_arg(argptr, double);
		char c = (char)va_arg(argptr, int);
*/

double add(const int n, ...) {
	va_list argptr;
	va_start(argptr, n);

	double sum = 0.0;
	for (int i = 0; i < n; i += 1) {
		sum += va_arg(argptr, double);
	}
	va_end(argptr);
	return sum;
}

int array_init() {
	//構造体の初期化リスト
	typedef struct {
		unsigned long pin;
		char name[64];
	}Person;
	Person team[6] = { {1000,"Mary"},{2000,"Harry"} };
	printf("team:%ld\n", team[0].pin);
}

int string_init() {
	//C言語には文字列型というものがない
	//なので要素がchar wchar_t char16_t char32_tの配列で代用している
	char str1[30] = "Let's go";	//文字列長が配列長より短くても問題ない、ただし配列長は文字列長+１以上であること（終端ヌル文字のため１つ大きい））
	printf("string長さ:%zu\n", strlen(str1));
	char str2[] = " to London!";	//自動で配列長を決めてくれる
	printf("string長さ:%zu\n", strlen(str2));
}

int mul_array_init() {
	//多次元配列の初期化
	int a3d[2][2][3] = {
		{{1,0,0},{4,0,0}},
		{{7,8,0},{13,0,0}}
	};
	for (int i = 0; i < 2; i += 1) {
		for (int j = 0; j < 2; j += 1) {
			for (int k = 0; k < 3; k += 1) {
				printf("%d ", a3d[i][j][k]);
			}
			putchar('\n');
		}
	}
}

int void_pointer() {
	//mallocはvoid* ポインタを返すがint* iPtrに代入されることでint*に変換される
	int* iPtr = malloc(1000 * sizeof(int));
	printf("iPtr %zu\n", sizeof(iPtr));	//iPtr 8
	double x = 1.5;
	//double xのポインタはdouble*なので
	int* cPtr = &x;
	printf("int*ではdoubleを指せない？エラーは出ないが値がおかしくなる%f\n", *cPtr);
	double* dPtr = &x;
	printf("double*でdoubleを当然指せる%f\n", *dPtr);
	free(iPtr);
}

int const_pointer() {
	int var = 100;
	int* const c_ptr = &var;	//intの定数ポインタ、constはvarを指すポインタであり、それ自体が定数であるが、指しているint varは定数ではない
	var = 123;
	printf("int:%d\n", var);
	//しかしc_ptr自体はへんこうできない
	//c_ptr += 1;	//エラーとなる
}

int array_pointer() {
	//ポインタを要素として持っている配列をポインタ配列という
	//ポインタ配列は2次元配列の代用として使われることが多い
	//この配列は文字長256、文字列数100までを格納できる配列
	char mtStrings[100][256] = {
		"If anything can go wrong, it will.",
		"Nothing is foolproof because fools are so ingen",
		"Every solution breesd new pro"
	};
	for (int i = 0; i < 3; i += 1) {
		printf("mtStrings:%s\n", mtStrings[i]);
	}
	//上の方法はメモリを食いすぎなのでポインタ配列を使う
	char* myStrPtr[100] = {			//もう少し正確に書くにはconst char* myStrPtr[100]が良い
		"If anything can go wrong, it will.",
		"Nothing is foolproof because fools are so ingen",
		"Every solution breesd new pro"
	};
	for (int i = 0; i < 3; i += 1) {
		printf("mtStrings:%s\n", myStrPtr[i]);
	}
}

static double Add(double x, double y) { return x + y; }
static double Sub(double x, double y) { return x - y; }
static double Mul(double x, double y) { return x * y; }
static double Div(double x, double y) { return x / y; }

int func_pointer() {
	//関数の型へのポインタの書き方、型doubleの２つの引数をとりdoubleを返す関数の型
	double (*funcPtr)(double, double);
	typedef double func_t(double, double);
	func_t* const funcTable[4] = { Add ,Sub,Mul,Div };
	for (int i = 0; i < 4; i += 1) {
		printf("func Table: %f\n", (float)funcTable[i](6.0, 3.0));
	}

}

struct Date {	//このDateはタグ
	short month;	//month,day,yearはメンバー名、可変長配列はメンバーには入れてはならない
	short day;
	short year;
};

typedef struct {	//typedefから直接構造体を定義するとタグ名を書かなくてもよい
	char title[64];
	char artist[32];
	char composer[32];
	short duration;
	struct Date published;
}Song_t;

static void printSong(const Song_t* pSong) {
	const int m = pSong->duration / 60;
	const int s = pSong->duration % 60;
	printf("--------------------------\n"
		"title:		%s\n"
		"artist:    %s\n"
		"composer:  %s\n"
		"playing time:  %d:%02d\n",
		pSong->title, pSong->artist, pSong->composer, m, s
	);
	//pSong->artist = "test";	//変更不可のエラーがでる
}

static char* dateAsString(struct Date d) {
	static char strDate[12];
	sprintf_s(strDate, 12, "%02d/%02d/%04d", d.month, d.day, d.year);
	return strDate;
}

int struct_unit_bitfiled() {
	struct Song {
		char title[64];
		char artist[32];
		char composer[32];
		short duration;
		struct Date published;	//上のstruct Dateを埋め込むことができる。
		//struct Song music;	//ただし自分自身は埋め込めない
	};

	struct Song s = { "song-title","artist-name","composer-name",7,{1,25,2023} };
	printf("title:%s\n", s.title);
	printf("artist:%s\n", s.artist);
	printf("year:%d\n", s.published.year);

	//typedefを使うと構造体の定義が楽になる
	typedef struct Song Song_t;	//Song_tがstruct Songと同義語となる
	Song_t song1;	//逐一structと書かなくてよくなる
	//構造体の代入コピーは簡単、メンバー同士をコピーしなくて良い、まとめてコピーできる
	Song_t s1 = { "song-title1","artist-name1","composer-name1",90,{2,26,2024} };
	Song_t s2;
	s2 = s1;
	printf("title:%s\n", s2.title);
	printf("artist:%s\n", s2.artist);
	printf("year:%d\n", s2.published.year);
	//関数の引数に構造体が使われていれば当然値渡しになる。構造体がでかいと効率が落ちるので参照渡しが良い
	//参照渡しで渡した構造体を関数内でいじられたくなかったらconstを付ける
	printSong(&s2);
	//構造体の初期化で特定のメンバーだけ初期化したいとき
	Song_t song3 = {
		.title = "I've Just Seen a Face",
		.composer = "Joho Lennon"
	};
	printf("song1:title: %s\n", song3.title);	//初期化した文字列を表示
	printf("song1:artist: %s\n", song3.artist);	//なにも表示しない
	//メモリ内の構造体メンバー
	/*先頭メンバーのアドレスは構造体のアドレスと同じ
	構造体の開始アドレスからオフセットを求めるとなにかと便利
	stddef.hに定義されているoffsetで求められる。返される値はsize_t
	//pSongが構造体のアドレスとするとメンバーcomposerへのアドレスは次のように求められる
	*/
	char* ptr = (char*)&song3 + offsetof(Song_t, composer);
	printf("song3->composer: %s\n", ptr);

	//---------------共用体-------------------------------
	//メンバーは全て同じアドレスをもつ
	union Data {
		int i;
		double x;
		char str[16];
	};
	printf("union sizeof: %zu\n", sizeof(union Data));	//16
	//共有体の初期化
	union Data data = { 77 };	//何も指定せずに初期化すると第一メンバーを初期化する
	printf("union i: %d\n", data.i);
	union Data data1 = { .x = 3.14 };	//先頭メンバー以外を初期化する方法
	printf("union x: %f\n", data1.x);
	//-----------------ビットフィールド--------------------
	/*ビットフィールドは指定されたビット幅の整数*/
	struct Date_Time {
		unsigned int month : 4;
		unsigned int day : 5;
		signed int year : 22;
		int tmp : 1;	//パッデング
	};
	printf("bit fild size: %zu\n", sizeof(struct Date_Time));		//4bit + 5bit + 22bit = 31bit 31/8bit=4byteの1bitあまり
	struct Date_Time dt = { 1,15,2023 };
	printf("bit fild init %d/%d/%d\n", dt.year, dt.month, dt.day);
}

int DynamicMemoryManagement() {
	//malloc,calloc 新しいメモリブロックを割り当てる
	//realloc 割り当てたメモリブロックのサイズを変更する
	//gree 割り当てたメモリブロックを解放する
	//void* malloc(size_t size);
	//	サイズがsizeのメモリブロックを確保する
	//void* calloc(size_t count,size_t count);
	//	サイズがcount x sizeのメモリブロックを確保する.確保したメモリを0で初期化する
	//malloc,callocともメモリブロックを確保できなかったらNULLをかえす
	//void free(void* ptr);
	//	ptrが指すメモリブロックを解放する
	//  freeにメモリブロックを指すポインタを指定してメモリを開放する。
	// すでに解放されているメモリのポインタを渡すとバグとなる
	//	freeは開放するべきメモリブロックのサイズはどうやってわかっているのだ？
	//		C言語の free がサイズを知らなくてもメモリを解放できるのは、malloc が確保したメモリブロック
	//		の直前に「管理情報（ヘッダー）」を隠し持っているからです。
	//		仕組みの概要
	//		malloc でメモリを確保するとき、実際にはユーザーが要求したサイズだけでなく、
	//		少し余分な領域が確保されます。
	//		[ 管理情報（ヘッダー） ][  ユーザーに返される領域  ]
	//		内部で管理するアドレス    mallocの戻り値（ユーザーが使うポインタ）
	//void* realloc(void* ptr,size_t size)
	//	ptrで示したメモリブロックを解放し、sizeで指定された大きさのメモリブロックを確保して
	//	そのアドレスをかえす。元のアドレスと同じになるか、異なるかは決まっていない
	double* dPtr = malloc(sizeof(double));	//mallocからの返り値はvoid*であるがdPtrに代入されるときdouble*型に自動的に型変換される
	if (dPtr == NULL) {
		return NULL;
	}
	*dPtr = 0.02;
	printf("DynamicMemoryManagement: %f\n", *dPtr);
	free(dPtr);
}

//**************************BSTree*******************************************************************
//二分木探索木の実装例でメモリ管理の例とする
typedef int CmpFunc_t(const void* pKey1, const void* pKey2);
typedef const void* GetKeyFunc_t(const void* pData);

typedef struct {
	struct Node_Type* pRoot;       // Pointer to the root.
	CmpFunc_t* cmp;         // Compares two keys.
	GetKeyFunc_t* getKey;      // Converts data into a key value
} BST_t;

BST_t* newBST(CmpFunc_t* cmp, GetKeyFunc_t* getKey);
_Bool       BST_insert(BST_t* pBST, const void* pData, size_t size);
const void* BST_search(BST_t* pBST, const void* pKey);
_Bool       BST_erase(BST_t* pBST, const void* pKey);
void        BST_clear(BST_t* pBST);

// 特定の順序でツリーを走査し、各ノードに対して
// action で指定された関数を呼び出します。action がノードのデータを
// 変更する場合、ツリーのソート順序を維持するために、少なくとも
// キーの値は変更せずに残す必要があります。
int BST_inorder(BST_t* pBST, _Bool (*action)(void* pData));
int BST_rev_inorder(BST_t* pBST, _Bool (*action)(void* pData));
int BST_preorder(BST_t* pBST, _Bool (*action)(void* pData));
int BST_postorder(BST_t* pBST, _Bool (*action)(void* pData));


typedef struct Node {
	struct Node* left, * right;   // left,right child nodes.
	size_t size;          // Size of the data payload.
	char data[];          // The data itself.
} Node_t;

const void* defaultGetKey(const void* pData) { return pData; }

//関数 newBST() は、新しい二分探索木を作成します。
// newBST() が返す BST_t へのポインタは、他のすべての二分木関連関数の第1引数となります。
BST_t* newBST(CmpFunc_t* cmp, GetKeyFunc_t* getKey)
{
	BST_t* pBST = NULL;
	if (cmp != NULL)
		pBST = malloc(sizeof(BST_t));
	if (pBST != NULL)
	{
		pBST->pRoot = NULL;
		pBST->cmp = cmp;
		pBST->getKey = (getKey != NULL) ? getKey : defaultGetKey;
	}
	return pBST;
}

// BST_insert() 関数は、データ項目を木内の新しい葉ノードにコピーします。
// この関数は、二分木の並べ替え条件に従った位置に新しい葉を挿入します。
// BST_insert() は、新しいデータの挿入に成功した場合は true を返し、
// そうでない場合は false を返します。
static _Bool insert(BST_t* pBST, Node_t** ppNode, const void* pData, size_t size);

_Bool BST_insert(BST_t* pBST, const void* pData, size_t size)
{
	if (pBST == NULL || pData == NULL || size == 0)
		return false;
	return insert(pBST, &(pBST->pRoot), pData, size);
}

// 再帰的なヘルパー関数 insert():
static _Bool insert(BST_t* pBST, Node_t** ppNode, const void* pData, size_t size)
{
	Node_t* pNode = *ppNode;                 // Pointer to the root node of the
	// subtree to insert the new node in.
	if (pNode == NULL)
	{                                        // There's a place for a new leaf here.
		pNode = malloc(sizeof(Node_t) + size);
		if (pNode != NULL)
		{
			pNode->left = pNode->right = NULL;   // Initialize the new node's
			// members.
			memcpy(pNode->data, pData, size);
			*ppNode = pNode;                     // Insert the new node.
			return true;
		}
		else
			return false;
	}
	else                                     // Continue looking for a place ...
	{
		const void* key1 = pBST->getKey(pData),
			* key2 = pBST->getKey(pNode->data);
		if (pBST->cmp(key1, key2) < 0)     // ... in the left subtree,
			return insert(pBST, &(pNode->left), pData, size);
		else                                   // or in the right subtree.
			return insert(pBST, &(pNode->right), pData, size);
	}
}

// BST_search() 関数は、二分探索アルゴリズムを使用して、指定されたキーに一致するデータ項目を検索します。
// 戻り値は、キーに一致する最初のノードのデータ項目へのポインタ、または一致するものが見つからなかった場合はヌルポインタです。
static const void* search(BST_t* pBST, const Node_t* pNode, const void* pKey);

const void* BST_search(BST_t* pBST, const void* pKey)
{
	if (pBST == NULL || pKey == NULL)
		return NULL;
	return search(pBST, pBST->pRoot, pKey);    // Start at the root of the tree.
}

// 再帰的なヘルパー関数 search():
static const void* search(BST_t* pBST, const Node_t* pNode, const void* pKey)
{
	if (pNode == NULL)
		return NULL;                                    // No subtree to search;
	// no match found.
	else
	{                                                 // Compare data:
		int cmp_res = pBST->cmp(pKey, pBST->getKey(pNode->data));
		if (cmp_res == 0)                             // Found a match.
			return pNode->data;
		else if (cmp_res < 0)                         // Continue the search
			return search(pBST, pNode->left, pKey);   // in the left subtree,
		else
			return search(pBST, pNode->right, pKey);  // or in the right subtree.
	}
}

// BST_erase() 関数は、指定されたキーに一致するノードを検索し、
// 見つかった場合はそのノードを削除します。
// 削除対象のノードが見つからなかった場合は false を、
// 削除に成功した場合は true を返します。

// 削除アルゴリズムでは、再帰的なヘルパー関数 detachMin() を使用します。
// この関数は、指定された部分木から最小のノードを抜き出し、
// そのノードへのポインタを返します。
static Node_t* detachMin(Node_t** ppNode)
{
	Node_t* pNode = *ppNode;                  // A pointer to the current node.
	if (pNode == NULL)
		return NULL;                            // pNode is an empty subtree.
	else if (pNode->left != NULL)
		return detachMin(&(pNode->left));     // The minimum is in the left subtree.
	else
	{                                         // pNode points to the minimum node.
		*ppNode = pNode->right;                 // Attach the right child to the parent.
		return pNode;
	}
}

static _Bool erase(BST_t* pBST, Node_t** ppNode, const void* pKey);

_Bool BST_erase(BST_t* pBST, const void* pKey)
{
	if (pBST == NULL || pKey == NULL)
		return false;
	return erase(pBST, &(pBST->pRoot), pKey);      // Start at the root of the tree.
}

// 再帰的なヘルパー関数 erase():
static _Bool erase(BST_t* pBST, Node_t** ppNode, const void* pKey)
{
	Node_t* pNode = *ppNode;            // Pointer to the current node.
	if (pNode == NULL)
		return false;                    // No match found.
	// Compare data:
	int cmp_res = pBST->cmp(pKey, pBST->getKey(pNode->data));

	if (cmp_res < 0)                            // Continue the search
		return erase(pBST, &(pNode->left), pKey);  // in the left subtree,
	else if (cmp_res > 0)
		return erase(pBST, &(pNode->right), pKey); // or in the right subtree.
	else
	{                                   // Found the node to be deleted.
		if (pNode->left == NULL)        // If no more than one child,
			*ppNode = pNode->right;         // attach the child to the parent.
		else if (pNode->right == NULL)
			*ppNode = pNode->left;
		else                              // Two children: replace the node with
		{                                 // the minimum from the right subtree.
			Node_t* pMin = detachMin(&(pNode->right));
			*ppNode = pMin;            // Graft it onto the deleted node's parent.
			pMin->left = pNode->left;       // Graft the deleted node's children.
			pMin->right = pNode->right;
		}
		free(pNode);                    // Release the deleted node's storage.
		return true;
	}
}

//BST_clear() 関数は、木のすべてのノードを削除します。
static void clear(Node_t* pNode);
void BST_clear(BST_t* pBST)
{
	if (pBST != NULL)
	{
		clear(pBST->pRoot);
		pBST->pRoot = NULL;
	}
}

// 再帰的なヘルパー関数 clear():
static void clear(Node_t* pNode)
{
	if (pNode != NULL)
	{
		clear(pNode->left);
		clear(pNode->right);
		free(pNode);
	}
}

// 以下の各走査関数は、第2引数として「アクション」関数へのポインタを受け取ります。
// このアクション関数は、走査中に各ノードを訪問するたびに呼び出されます。
// 戻り値は、アクションが正常に実行された回数です。
static int inorder(Node_t* pNode, _Bool (*action)(void* pData));

int BST_inorder(BST_t* pBST, _Bool (*action)(void* pData))
{
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return inorder(pBST->pRoot, action);
}

// 再帰的なヘルパー関数 inorder():
static int inorder(Node_t* pNode, _Bool (*action)(void* pData))
{
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = inorder(pNode->left, action);        // L: Traverse the left
	// subtree.
	if (action(pNode->data))                    // N: Visit the current
		++count;                                     // node itself.
	count += inorder(pNode->right, action);      // R: Traverse the right
	// subtree.
	return count;
}

static int rev_inorder(Node_t* pNode, _Bool (*action)(void* pData));

int BST_rev_inorder(BST_t* pBST, _Bool (*action)(void* pData))
{
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return rev_inorder(pBST->pRoot, action);
}

// 再帰的なヘルパー関数 rev_inorder():
static int rev_inorder(Node_t* pNode, _Bool (*action)(void* pData))
{
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = rev_inorder(pNode->right, action);   // R: Traverse the right
	// subtree.
	if (action(pNode->data))                    // N: Visit the current
		++count;                                     // node itself.
	count += rev_inorder(pNode->left, action);   // L: Traverse the left
	// subtree.
	return count;
}

// -------------------------------------------------------------------------------
static int preorder(Node_t* pNode, _Bool (*action)(void* pData));

int BST_preorder(BST_t* pBST, _Bool (*action)(void* pData))
{
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return preorder(pBST->pRoot, action);
}

// 再帰的なヘルパー関数 preorder():
static int preorder(Node_t* pNode, _Bool (*action)(void* pData))
{
	int count = 0;
	if (pNode == NULL)
		return 0;

	if (action(pNode->data))                    // N: The current node.
		++count;
	count += preorder(pNode->left, action);      // L: Traverse the left
	// subtree.
	count += preorder(pNode->right, action);     // R: Traverse the right
	// subtree.
	return count;
}

// -------------------------------------------------------------------------------
static int postorder(Node_t* pNode, _Bool (*action)(void* pData));

int BST_postorder(BST_t* pBST, _Bool (*action)(void* pData))
{
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return postorder(pBST->pRoot, action);
}

// 再帰的なヘルパー関数 postorder():
static int postorder(Node_t* pNode, _Bool (*action)(void* pData))
{
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = postorder(pNode->left, action);      // L: Traverse the left
	// subtree.
	count += postorder(pNode->right, action);    // R: Traverse the right
	// subtree.
	if (action(pNode->data))                    // N: Visit the current
		++count;                                     // node itself.

	return count;
}

#define LEN_MAX 1000              // Maximum length of a line.
char buffer[LEN_MAX];

// Action to perform for each line:
_Bool printStr(void* str) { return printf("%s", str) >= 0; }

BTree_main() {
	BST_t* pStrTree = newBST((CmpFunc_t*)strcmp, NULL);
	int n;

	while (fgets(buffer, LEN_MAX, stdin) != NULL) {   // Read each line.
		size_t len = strlen(buffer);                   // Length incl.
		// newline character.
		if (!BST_insert(pStrTree, buffer, len + 1))     // Insert the line in
			break;                                        // the tree.
	}
	if (!feof(stdin)) {                                     // If unable to read the entire text:
		fprintf(stderr, "sortlines: "
			"Error reading or storing text input.\n");
		exit(EXIT_FAILURE);
	}
	n = BST_inorder(pStrTree, printStr);     // Print each line, in sorted order.
	fprintf(stderr, "\nsortlines: Printed %d lines.\n", n);
	BST_clear(pStrTree);                     // Discard all nodes.
	return 0;
}

_Bool isReadWriteable(const char* filename) {
	FILE* fp = fopen(filename, "r+");
	if (fp != NULL) {	//ファイルオープンは成功
		fclose(fp);
		return true;
	}
	else {	//失敗
		return false;
	}
}

_Bool file_error(const char* filename) {
	FILE* fp = fopen(filename, "r+");
	if (ferror(fp)) {
		printf("Error writeing\n");
	}
	return 0;
}


int print_errno(const char* filename) {
	FILE* fp = fopen(filename, "r+");
	long pos = ftell(fp);
	if (pos == 0L) {
		perror("ftell()");	//perrorは現在のエラーメッセージを表示する
	}
	return 0;
}

static int file_get(const char* filename) {
	FILE* fp = fopen(filename, "r+");
	printf("%c\n", fgetc(fp));	//#
	printf("%c\n", fgetc(fp));	//p	mdump.hの1行目は#pragma onceと書いてある、読み込む度に歩進している
	printf("%c\n", getc(fp));	//r	mdump.hの1行目は#pragma onceと書いてある、読み込む度に歩進している
	return 0;
}

int File_stream() {
	printf("fileopen %d\n", isReadWriteable("mdump.h"));	//1
	//file_error("mdump1.h"); //runtimeエラーがでて継続できない
	print_errno("mdump.h");		//ftell(): No error
	file_get("mdump.h");
	return 0;
}

int scanf_test() {
	int age = 0;
	char name[64] = "";
	printf("Please enter your name and age:\n");
	scanf_s("%s %d", name, (unsigned int)sizeof(name), &age);
	/*このままだとnameにはいる文字列がTaro Yamadaのように間に空白が入ると
	おかしくなるそこで
	scanf_s("%99[^\n] %d", name, (unsigned)sizeof(name), &age);
	のようにすると空白入りでも対応できるが色々面倒なことはあるので
	nameはfgetsで入力してそのあとsscanf_sで対応するのが面倒くさくないです
	char buf[256];	//fgets用のバッファ
	char name[100];
	int age;

	fgets(buf, sizeof(buf), stdin);
	sscanf_s(buf, "%99[^\n] %d", name, (unsigned)sizeof(name), &age);
	*/
	printf("name: %s age: %d\n", name, age);
	return 0;
}

#define MAX_THREADS 8
#define MIN_BLOCK_SIZE 10

typedef struct {
	float* start;
	int len;
	int block_size;
	double sum;
}Sum_arg;

int sig_sum(float arr[]) {
	double total = 0;
	for (int i = 0; i < 100; i += 1) {
		total += arr[i];
	}
	printf("total %f\n", total);
}
/*
int parallel_sum(void* arg);

bool sum(float arr[], int len, double* sumPtr) {
	int block_size = len / MAX_THREADS;
	if (block_size < MIN_BLOCK_SIZE) {
		block_size = len;
	}
	Sum_arg args = { arr,len,block_size,0.0L };
	if (parallel_sum(&args)) {
		*sumPtr = args.sum;
		return true;
	}
	else {
		return false;
	}
}

int parallel_sum(void* arg) {
	Sum_arg* argp = (Sum_arg*)arg;
	if (argp->len <= argp->block_size) {
		for (int i = 0; i < argp->len; ++i) {
			argp->sum += argp->start[i];
		}
		return 1;
	}
	else {
		int mid = argp->len / 2;
		Sum_arg arg2 = { argp->start + mid,argp->len - mid,argp->block_size,0 };
		argp->len = mid;
		thrd_t th;
		if (thrd_create(&th, parallel_sum, arg) != thrd_success) {
			return 0;
		}
		if (!parallel_sum(&arg2)) {
			thrd_detach(th);
			return 0;
		}
		int res = 0;
		thrd_join(th, &res);
		if (!res) {
			return 0;
		}
		argp->sum += arg2.sum;
		return 1;
	}
}
*/

int const_basic() {
	const int n;
	//n = 1;	constを付けると変更できない
	const int m = 42;
	printf("m=%d\n", m);
	int a = 43, b = 47;
	const int* ciPtr = &a;
	int c = *ciPtr + 3;
	printf("c=%d\n", c);	//46 ciPtr自体を変更しているのではないのでこれはOK
	//*ciPtr = 53;		//ciPtrが指しているものは変更できない、ciPtr
	a = 48;
	ciPtr = &b;	//指しているものがconstであるがポインタそのものは変更可能
	printf("*ciptr=%d\n", *ciPtr);	//47
	//しかしciPtrが指しているｂ自体は変更可能
	b = 107;
	printf("*ciptr=%d\n", *ciPtr);	//107
	//つまりconst int* ciPtrはポインタを通じてのもともとの変数を変更することができない
	//あくまでもポインタ経由の変更、関数の引数の時に便利かも、ポインタで渡したけど関数
	//内で変更されたくないときに使えばよい
	//このようなポインタは読込専用ポインタと呼ばれる
	int* const c_ptr = &a;
	//constの位置が変更になった。この位置のconstはポインタが指しているものを変更不可
	//にするのではなくポインタそのものを変更不可にする
	printf("*c_ptr=%d\n", *c_ptr);	//48
	*c_ptr = 203;
	printf("*c_ptr=%d\n", *c_ptr);	//203
	//c_ptr = &b;	ポインタそのものの書き換えは不可
	//組込み・ハードウェア制御（レジスタへのアクセス）などに使用されている
	//ポインタも変更不可、ポインタ経由での変更も不可は次のように書く
	const int* const p = &b;
	printf("*p=%d\n", *p);	//107
}
