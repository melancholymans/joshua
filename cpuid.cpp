﻿// InstructionSet.cpp 
// Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about 
// CPU extended instruction set support.

#include <intrin.h>		//_mm_popcnt_u32 / _mm_popcnt_u64
#include <inttypes.h>	//uint64_t
#include <smmintrin.h>	//SSE4.2
#include <immintrin.h>	//AVX

#include "types.h"
#include "common.h"

class InstructionSet
{
	// forward declarations
	class InstructionSet_Internal;

public:
	// getters
	static std::string Vendor(void) { return CPU_Rep.vendor_; }
	static std::string Brand(void) { return CPU_Rep.brand_; }

	static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
	static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
	static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
	static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
	static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
	static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
	static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
	static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
	static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
	static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
	static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
	static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
	static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
	static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
	static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
	static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

	static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
	static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
	static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
	static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
	static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
	static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
	static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
	static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
	static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

	static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
	static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
	static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
	static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
	static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
	static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
	static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
	static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
	static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
	static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
	static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
	static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
	static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
	static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
	static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

	static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

	static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
	static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
	static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
	static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
	static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
	static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

	static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
	static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
	static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
	static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
	static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
	static const InstructionSet_Internal CPU_Rep;

	class InstructionSet_Internal
	{
	public:
		InstructionSet_Internal()
			: nIds_{ 0 },
			nExIds_{ 0 },
			isIntel_{ false },
			isAMD_{ false },
			f_1_ECX_{ 0 },
			f_1_EDX_{ 0 },
			f_7_EBX_{ 0 },
			f_7_ECX_{ 0 },
			f_81_ECX_{ 0 },
			f_81_EDX_{ 0 },
			data_{},
			extdata_{}
		{
			//int cpuInfo[4] = {-1};
			std::array<int, 4> cpui;

			// Calling __cpuid with 0x0 as the function_id argument
			// gets the number of the highest valid function ID.
			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			// Capture vendor string
			char vendor[0x20];
			memset(vendor, 0, sizeof(vendor));
			*reinterpret_cast<int*>(vendor) = data_[0][1];
			*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
			*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
			vendor_ = vendor;
			if (vendor_ == "GenuineIntel")
			{
				isIntel_ = true;
			}
			else if (vendor_ == "AuthenticAMD")
			{
				isAMD_ = true;
			}

			// load bitset with flags for function 0x00000001
			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			// load bitset with flags for function 0x00000007
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			// Calling __cpuid with 0x80000000 as the function_id argument
			// gets the number of the highest valid extended ID.
			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			// load bitset with flags for function 0x80000001
			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			// Interpret CPU brand string if reported
			if (nExIds_ >= 0x80000004)
			{
				memcpy(brand, extdata_[2].data(), sizeof(cpui));
				memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
				memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
				brand_ = brand;
			}
		};

		int nIds_;
		int nExIds_;
		std::string vendor_;
		std::string brand_;
		bool isIntel_;
		bool isAMD_;
		std::bitset<32> f_1_ECX_;
		std::bitset<32> f_1_EDX_;
		std::bitset<32> f_7_EBX_;
		std::bitset<32> f_7_ECX_;
		std::bitset<32> f_81_ECX_;
		std::bitset<32> f_81_EDX_;
		std::vector<std::array<int, 4>> data_;
		std::vector<std::array<int, 4>> extdata_;
	};
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


// Print out supported instruction set extensions
int cpuid()
{
	auto& outstream = std::cout;

	auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
		outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
	};

	std::cout << InstructionSet::Vendor() << std::endl;
	std::cout << InstructionSet::Brand() << std::endl;

	support_message("3DNOW", InstructionSet::_3DNOW());
	support_message("3DNOWEXT", InstructionSet::_3DNOWEXT());
	support_message("ABM", InstructionSet::ABM());
	support_message("ADX", InstructionSet::ADX());
	support_message("AES", InstructionSet::AES());
	support_message("AVX", InstructionSet::AVX());
	support_message("AVX2", InstructionSet::AVX2());
	support_message("AVX512CD", InstructionSet::AVX512CD());
	support_message("AVX512ER", InstructionSet::AVX512ER());
	support_message("AVX512F", InstructionSet::AVX512F());
	support_message("AVX512PF", InstructionSet::AVX512PF());
	support_message("BMI1", InstructionSet::BMI1());
	support_message("BMI2", InstructionSet::BMI2());
	support_message("CLFSH", InstructionSet::CLFSH());
	support_message("CMPXCHG16B", InstructionSet::CMPXCHG16B());
	support_message("CX8", InstructionSet::CX8());
	support_message("ERMS", InstructionSet::ERMS());
	support_message("F16C", InstructionSet::F16C());
	support_message("FMA", InstructionSet::FMA());
	support_message("FSGSBASE", InstructionSet::FSGSBASE());
	support_message("FXSR", InstructionSet::FXSR());
	support_message("HLE", InstructionSet::HLE());
	support_message("INVPCID", InstructionSet::INVPCID());
	support_message("LAHF", InstructionSet::LAHF());
	support_message("LZCNT", InstructionSet::LZCNT());
	support_message("MMX", InstructionSet::MMX());
	support_message("MMXEXT", InstructionSet::MMXEXT());
	support_message("MONITOR", InstructionSet::MONITOR());
	support_message("MOVBE", InstructionSet::MOVBE());
	support_message("MSR", InstructionSet::MSR());
	support_message("OSXSAVE", InstructionSet::OSXSAVE());
	support_message("PCLMULQDQ", InstructionSet::PCLMULQDQ());
	support_message("POPCNT", InstructionSet::POPCNT());
	support_message("PREFETCHWT1", InstructionSet::PREFETCHWT1());
	support_message("RDRAND", InstructionSet::RDRAND());
	support_message("RDSEED", InstructionSet::RDSEED());
	support_message("RDTSCP", InstructionSet::RDTSCP());
	support_message("RTM", InstructionSet::RTM());
	support_message("SEP", InstructionSet::SEP());
	support_message("SHA", InstructionSet::SHA());
	support_message("SSE", InstructionSet::SSE());
	support_message("SSE2", InstructionSet::SSE2());
	support_message("SSE3", InstructionSet::SSE3());
	support_message("SSE4.1", InstructionSet::SSE41());
	support_message("SSE4.2", InstructionSet::SSE42());
	support_message("SSE4a", InstructionSet::SSE4a());
	support_message("SSSE3", InstructionSet::SSSE3());
	support_message("SYSCALL", InstructionSet::SYSCALL());
	support_message("TBM", InstructionSet::TBM());
	support_message("XOP", InstructionSet::XOP());
	support_message("XSAVE", InstructionSet::XSAVE());
	return 0;
}


void implementation_avx(void)
{
	//参考になるスライド
	//http://www.slideshare.net/herumi/x86opti6?from_action=save
	//リファレンス
	//http://nf.nci.org.au/facilities/software/intel-ct/13.5.192/Documentation/ja_JP/compiler_c/main_cls/GUID-E6E297BB-E575-4525-AE1C-DFD7D5B662F5.htm
//LZCNT命令(ABM)
	//ソースオペランドの先頭からのゼロ・ビットの数をカウントします。 ソースオペランドが 0 の場合は、オペランドのサイズを返します。 対応するインテル® AVX2 命令は LZCNT です。
	//変数全体に含まれている0bitの数ではなく最上位bitから0bitが連続で何個並んでいるかをカウントする。
	//32bit 0x13E7FFは２進数では  0000 0000 0001 0011 1110 0111 1111 1111なので最上位から0が11個」並んでいるので答えは11
	//64bit 0xFFFFFFFFFFFFは２進数では0000 0000 0000 0000 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111なので最上位から0が16個並んでいるので答えは16
	unsigned int source = 0x13E7FF;	//0bit 15個
	cout << "_lzcnt_u32= " << _lzcnt_u32(source) << endl;
	unsigned __int64 source64 = 0xFFFFFFFFFFFF;	//0bit 16個
	cout << "_lzcnt_u64= " << _lzcnt_u64(source64) << endl;
//TZCNT命令(BMI1)
	//ソースオペランドの最後(最下位ビット) からのゼロ・ビットの数をカウントし、結果を返します。ソースオペランドが 0 の場合、ソースオペランドのサイズ(ビット数) が返されます。対応するインテル® AVX2 命令は TZCNT です。
	//変数全体に含まれている0bitの数ではなく最下位bitから0bitが連続で何個並んでいるかをカウントする。
	source = 0x13E700;	//0bit 8個
	cout << "_tzcnt_u32= " << _tzcnt_u32(source) << endl;
	source64 = 0xC00;	//0bit 10個
	cout << "_tzcnt_u64= " << _tzcnt_u64(source64) << endl;
//ANDN(BMI1) ~x & y
	unsigned int source1 = 0x457;	//010001010111
	unsigned int source2 = 0x876;	//100001110110
	//~x & y						
	//~x							  11111111111111111111101110101001
	//y								  00000000000000000000100001110110
	//~x & y						  00000000000000000000100000100000	0x820
	//計算結果と手計算が合わない、pythonで計算させてみたが計算結果が正しい、そもそも~xの計算が間違っているが
	//いったん置いておく
	cout << "_andn_u32= " << _andn_u32(source1, source2) << endl;
//BEXTR(BMI1)
	//たぶん使うことなさそうなのでパス
	//BLSI(BMI1) x & -x	
	//この操作の意味
	//最下位から見て最初の１がたったビットを抽出して返す
	//例　0x457の場合 -> 010001010111なので最初にビットが立っているのは0bit目なの 0001が抽出され10進数に変換されて返されるので1となる
	//例　0x88の場合 -> 10001000なので最初にビットがたっているのは3bit目なので01000が抽出され10進数に変換されて返されるので８となる
	source1 = 0x457;	//010001010111
	cout << "_blsi_u32= " << _blsi_u32(source1) << endl;	//1
	source2 = 0x88;	//10001000
	cout << "_blsi_u32= " << _blsi_u32(source2) << endl;	//8
	__int64 source3 = 0x807C00000ULL; //100000000111110000000000000000000000
	cout << "_blsi_u64= " << _blsi_u64(source3) << endl;	//4194304(10進数)
//BLSR(BMI1）
	//この操作の意味、最下位からみて初めて１になったまで0クリアする１が立っていた場所も0クリア
	source1 = 0x457;	//010001010111->010001010110
	cout << "_blsr_u32= " << _blsr_u32(source1) << endl;	//1110(10進数)
	source2 = 0x88;	//10001000->10000000
	cout << "_blsr_u64= " << _blsr_u64(source2) << endl;	//128(10進数)
//BLSMSK(BMI1)
	//この操作の意味、最下位からみて初めて１になった場所までマスクを作る、初めて１になった場所から上位はもちろん0
	source1 = 0x88;	//10001000 -> 00001111 
	cout << "_blsmsk_u32= " << _blsmsk_u32(source1) << endl;	//15 (10進数)
	source3 = 0x807C00000ULL; //100000000111110000000000000000000000 -> 000000000000011111111111111111111111
	cout << "_blsmsk_u32= " << _blsmsk_u64(source3) << endl;	//8388607 (10進数)
//BZHI(BMI2)
	//この操作の意味、index(第二引数 インデックスは1から始まる)から上位を0クリアする
	source1 = 0x1E0;	//111100000 -> 100000　　indexは１からスタート 
	cout << "_bzhi_u32= " << _bzhi_u32(source1, 6) << endl;	//55
	source3 = 0x807C00000ULL; //100000000111110000000000000000000000 -> 000000000000010000000000000000000000
	cout << "_bzhi_u64= " << _bzhi_u64(source3, 23) << endl; //4194304
//PDEP(BMI2)
	//この操作の意味、マスクmakに応じてsource1のbitをコピーしたものを返す
	//source1 y3 y2 y1 y0
	//mask    1  1  0  0  1  1  0  0
	//        y3 y2 0  0  y1 y0 0  0
	source1 = 85;	//1010101
	unsigned int mask = 0x3F8;	//1111111000
	unsigned int dest = _pdep_u32(source1,mask);
	cout << "_pdep_u32= " << dest << endl;		//680(10進数)   1010101000
//MULX
	//演算の結果で、フラグを変更しない乗算、利用方法思いつかず、パス
//RORX
	//演算の結果で、フラグを変更しない右シフト（ローテート）、利用方法思いつかずパス
	//SARX,SHRX,SHLXなどもフラグを変更しないbit操作でるが、利用方法思いつかずパス
//pext命令(BMI2)
	//unsigned int _pext_u32(unsigned int a,unsigned int mask)
	//unsigned __int64 _pext_u64(unsigned __int64 a,unsigned __int64 mask)
	//x64プロセッサでは__m64型の変数は使用できない.
	//__int64型を使用すること
	uint64_t a = 0x00;
	uint64_t b = 0x1040093042500548;
	uint64_t mask64 = 0x1044414000504441;
	/*
	b0
	最初に書かれているのが下位bit
	下の行に行くほど上位bit
	111000011
	101001100
	000000000
	000000000
	000000000
	000000000
	000000000
	b1
	110111000
	000000000
	*/
	a = _pext_u64(b, mask64);
	//正解は0xcb6となる
	BitBoard bb(0x1040093042500548, 0);
	cout << "pext 64 " << endl;		//680(10進数)   1010101000
	//BitBoardns::print(bb);
	BitBoard bb1(0x1044414000504441, 0);
	//BitBoardns::print(bb1);
}