#include <limits.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdint.h>

//1 _mm_loadu_si128
    /*128ビット（16バイト）の整数データを、メモリアドレスのアラインメント（整列）条件を問わずに __m128i 型のSIMDレジスタへロード（読み込み）します。
    */
//2 _mm_storeu_si128
    /*128ビット（16バイト）の整数データを、メモリアドレスのアラインメント（整列）条件を問わずに __m128i 型のSIMDレジスタからメモリへストア（書き込み）します。
	*/
//3 __m128iを直接printできないので一旦 _mm_storeu_si128などストア系のメモリにコピーしてprintする
//4 _mm_set1_epi32
    /*1 つの 32 ビット符号付き整数（int32_t 相当）を引数に取り、
	それを 128 ビット SIMD レジスタ（__m128i 型）の全レーン（4 要素）に同じ値でコピー します。
    */
//5 _mm_testz_si128
    /*128 ビット幅のパックド 32 ビット整数同士を要素ごとに比較し、等しいかどうかを判定するものです。
	_mm_testz_si128(a, b)でa,bの中身が全て0であれば1を返す。そうでなければ0を返す。
	*/
//6 _mm_testc_si128
    /*一言で言うと、「ベクトルbで1が立っているすべてのビット位置において、ベクトルaでも1が立っているか
    （完全包摂されているか）」 を判定する関数です。であれば1を返しそうでなければ0を返す
	*/
//7 _mm_set1_epi32
    /*1 つの 32 ビット符号付き整数（int32_t 相当）を引数に取り、
    それを 128 ビット SIMD レジスタ（__m128i 型）の全レーン（4 要素）に同じ値でコピー します。
	*/
//8 _mm_andnot_si128
    /*
    _mm_testc_si128とよく似ている
    一言で表すと、「第1引数をビット反転（NOT）し、第2引数とのビット論理積（AND）を計算する」 関数です。
    ビットごとに次のような計算をする
    result = (~a) & b
    用途としてはaをマスクとして使い、aのbitが１であればbのbitをクリアにできる
    a   b   andnot
    0   0   0
    0   1   1   aが0なのでbは1のまま
    1   0   0   aが1であるがbが0なので0のまま
    1   1   0   aが1なのでbの1をクリアしている
    */

int simd_128() {
    //_mm_loadu_si128
	int16_t input[8] = { -10, 25, -300, 4000, -32767, 0, -1, -32768 };
	int16_t output[8];
	__m128i a = _mm_loadu_si128((const __m128i*)input);
	__m128i result = _mm_abs_epi16(a);
	_mm_store_si128((__m128i*)output, result);
    for (int i = 0; i < 8; i++) {
        printf("[%d] 入力: %6d  ->  出力: %6d\n", i, input[i], output[i]);
    }
   // _mm_testz_si128()
   // 128ビットのデータとマスクを定義
   __m128i data = _mm_set1_epi32(0x01); // 各32bit要素が 0x00000001
   __m128i mask = _mm_set1_epi32(0x02); // 各32bit要素が 0x00000002

    // data & mask はすべて0になるため、_mm_testz_si128 は 1 を返す
    if (_mm_testz_si128(data, mask)) {
        printf("マスク位置のビットはすべて0です\n");
    }
    data = _mm_set1_epi32(0xFF);
    mask = _mm_set1_epi32(0xFF);
    // data & mask は1になるため、_mm_testz_si128 は 0 を返す
    if (_mm_testz_si128(data, mask)) {
        printf("マスク位置のビットはすべて0です\n");
    }
    else {
        printf("マスク位置のビットはすべて0ではない\n");
    }
    //_mm_testc_si128
    data = _mm_set1_epi32(0xFA);
    mask = _mm_set1_epi32(0xFA);
    if(_mm_testc_si128(data,mask)) {
        printf("aとmaskの１の位置はあっている\n");
    }
    else {
        printf("aとmaskの１の位置はあっていない\n");
    }
    //_mm_set1_epi8
    __m128i data8 = _mm_set1_epi8(0x14);
    int8_t output8[16];
    _mm_store_si128((__m128i*)output8, data8);
    for (int i = 0; i < 16; i +=1) {
        printf("%2d->%2d\n", i, output8[i]);
    }
    //_mm_set1_epi64x
	__m128i data64 = _mm_set1_epi64x(0x0234567890ABCDEF);
    int64_t output64[2];
	_mm_store_si128((__m128i*)output64, data64);
    for (int i = 0; i < 2; i += 1) {
        printf("%d->%zu\n", i, output64[i]);
    }
	//_mm_set_epi8
    /*第1引数が最上位レーンに第16引数が最下位レーンに配置される*/
    __m128i data8_epi = _mm_set_epi8(0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10);
    int8_t output8_epi[16];
    _mm_store_si128((__m128i*)output8_epi, data8_epi);
    for (int i = 0; i < 16; i +=1) {
        printf("%2d->%2d\n", i, output8_epi[i]);
    }

    //_mm_set_epi8 128ビットレジスタを全ビット0で初期化
    __m128i v = _mm_setzero_si128();
    // 確認用に32ビット整数×4個の配列に読み出す
    int32_t r1[4];
    _mm_storeu_si128((__m128i*)r1, v);
    // [0, 0, 0, 0] と出力される
    printf("_mm_setzero_si128 result: %d, %d, %d, %d\n", r1[0], r1[1], r1[2], r1[3]);

    //_mm_andnot_si128() 一言で表すと、「第1引数をビット反転（NOT）し、第2引数とのビット論理積（AND）を計算する」 関数です。
    
    // わかりやすいように64ビット整数2つで初期化
    // a = 0x00000000FFFFFFFF (下位32ビットが1)
    // b = 0x123456789ABCDEF0
    __m128i a4 = _mm_set_epi64x(0x0, 0x00000000FFFFFFFF);
    __m128i b4 = _mm_set_epi64x(0x0, 0x123456789ABCDEF0);

    // result = (~a) & b
    __m128i result4 = _mm_andnot_si128(a4, b4);

    // 結果の出力（下位64ビットを取り出して表示）
    unsigned long long res_val = _mm_cvtsi128_si64(result4);
    printf("0x%016llX\n", res_val);
    // 出力結果: 0x1234567800000000
    // (aで1だった下位32ビットがクリアされ、0だった上位32ビットのbの値が残る)
    
    //_mm_and_si128
    //bit単位のAND演算
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    b4 = _mm_set_epi64x(0x0, 0x0000000089ABCDEF);
    result4 = _mm_and_si128(a4,b4);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_and_si128 : 0x%016llX\n", res_val);
    //a4:0x12345678  0b00010010001101000101011001111000
    //b4:0x89ABCDEF  0b10001001101010111100110111101111
    //r4:0x00204468  0b00000000001000000100010001101000 

    //_mm_or_si128
    //bit単位のOR演算
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    b4 = _mm_set_epi64x(0x0, 0x0000000089ABCDEF);
    result4 = _mm_or_si128(a4, b4);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_or_si128 : 0x%016llX\n", res_val);
    //a4:0x12345678  0b00010010001101000101011001111000
    //b4:0x89ABCDEF  0b10001001101010111100110111101111
    //r4:0x9BBFDFFF  0b10011011101111111101111111111111 

    //_mm_xor_si128
    //2つの 128ビットレジスタの値に対して、ビット単位の排他的論理和（XOR） を実行します
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    b4 = _mm_set_epi64x(0x0, 0x0000000089ABCDEF);
    result4 = _mm_xor_si128(a4, b4);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_xor_si128 : 0x%016llX\n", res_val);
    //a4:0x12345678  0b00010010001101000101011001111000
    //b4:0x89ABCDEF  0b10001001101010111100110111101111
    //r4:0x9B9F9B97  0b10011011100111111001101110010111 

    //_mm_add_epi64
    //
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    b4 = _mm_set_epi64x(0x0, 0x0000000089ABCDEF);
    result4 = _mm_add_epi64(a4, b4);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_add_epi64 : 0x%016llX\n", res_val);
    //a4:0x12345678  0b00010010001101000101011001111000
    //b4:0x89ABCDEF  0b10001001101010111100110111101111
    //r4:0x9BE02467  0b10011011111000000010010001100111

    //_mm_slli_epi64
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    result4 = _mm_slli_epi64(a4, 4*8+4);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_slli_epi64 : 0x%016llX\n", res_val);
    //a4:0x12345678        0b00010010001101000101011001111000
    //r4:0x48D159E0          0b01001000110100010101100111100000 <-2つ左シフトしている
    //r4:0x2345678000000000    0b0010001101000101011001111000000000000000000000000000000000000000 <-36左シフトしている 64bitをオーバーして、先頭の１bitが消えている
	/*結論から言うと、64bitを超えたビットは捨てる（破棄される）ことになり、反対側の空いた下位ビットには 0 が補われます。
    また、隣の64bit領域（上位レーン）へビットが繰り越される（キャリーする）こともありません。*/

    //_mm_srli_epi64
    a4 = _mm_set_epi64x(0x0, 0x0000000012345678);
    result4 = _mm_srli_epi64(a4, 2);
    res_val = _mm_cvtsi128_si64(result4);
    printf("_mm_srli_epi64 : 0x%016llX\n", res_val);
    //a4:0x12345678        0b00010010001101000101011001111000
    //r4:0x048D159E      0b00000100100011010001010110011110 <-2つ右シフトしている
    /*128ビットベクトルの内部にある2つの64ビット整数を、それぞれ指定したビット数だけ右に論理シフト（0埋めシフト）する機能を
    提供します。*/

    //_mm_cmpeq_epi8()
    int8_t d6[16] = { 1, 2, 3, 4, 5, 6, 7, 8,9, 10, 11, 12, 13, 14, 15, 16 };
    int8_t d7[16] = { 1, 2, 3, 4, 5, 6, 7, 17,9, 10, 11, 12, 13, 14, 15, 18 };
    __m128i a6 = _mm_load_si128((__m128i*)d6);
    __m128i b6 = _mm_load_si128((__m128i*)d7);
    result4 = _mm_cmpeq_epi8(a6, b6);
    int8_t result_array[16];
    _mm_store_si128((__m128i*)result_array, result4);
    for (int i = 0; i < 16; i += 1) {
        printf("index %2d: %d %d %X\n", i,d6[i], d7[i], result_array[i]);
    }
    
    //_mm_cmpeq_epi64
    int64_t d8[2] = { 789, 456 };
    int64_t d9[2] = { 788, 456 };
    __m128i a7 = _mm_load_si128((__m128i*)d8);
    __m128i b7 = _mm_load_si128((__m128i*)d9);
    result4 = _mm_cmpeq_epi64(a7, b7);
    int64_t result_array2[2];
    _mm_store_si128((__m128i*)result_array2, result4);
    for (int i = 0; i < 2; i += 1) {
        printf("index %2d: %lld %lld %X\n", i, d8[i], d9[i], result_array2[i]);
    }

    //_mm_shuffle_epi8()
    __m128i mask1 = _mm_set_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    __m128i mask2 = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    __m128i mask3 = _mm_set_epi8(1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14);
    __m128i src =  _mm_set_epi8(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    result4 = _mm_shuffle_epi8(src, mask3);
    _mm_store_si128((__m128i*)result_array, result4);
    for (int i = 0; i < 16; i += 1) {
        printf("index %2d: %d\n", i,  result_array[i]);
    }
    //mask1 変更なし
    /*index  0: 1
    index  1: 2
    index  2: 3
    index  3: 4
    index  4: 5
    index  5: 6
    index  6: 7
    index  7: 8
    index  8: 9
    index  9: 10
    index 10: 11
    index 11: 12
    index 12: 13
    index 13: 14
    index 14: 15
    index 15: 16*/
    //mask2 反転
    /*index  0: 16
    index  1: 15
    index  2: 14
    index  3: 13
    index  4: 12
    index  5: 11
    index  6: 10
    index  7: 9
    index  8: 8
    index  9: 7
    index 10: 6
    index 11: 5
    index 12: 4
    index 13: 3
    index 14: 2
    index 15: 1*/
    //mask3 隣同士交換
    /*index  0: 2
    index  1: 1
    index  2: 4
    index  3: 3
    index  4: 6
    index  5: 5
    index  6: 8
    index  7: 7
    index  8: 10
    index  9: 9
    index 10: 12
    index 11: 11
    index 12: 14
    index 13: 13
    index 14: 16
    index 15: 15*/

    //_mm_unpackhi_epi64()
    __m128i a5 = _mm_set_epi64x(0x0123456701234567, 0x0000000000000000);
    __m128i b5 = _mm_set_epi64x(0x89ABCDEF89ABCDEF, 0x0000000000000000);
    __m128i result5 = _mm_unpackhi_epi64(a5,b5);
    //result5:[ b[1] (bits 127:64) | a[1] (bits 63:0) ]のa[1]を取り出してres_valに代入している
    res_val = _mm_cvtsi128_si64(result5);
    printf("_mm_unpackhi_epi64 下位: 0x%016llX\n", res_val);    //0x0123456701234567
    //ここで上位レーンと下位レーンを交換
    result5 = _mm_shuffle_epi32(result5, _MM_SHUFFLE(3, 2, 1, 0));
    res_val = _mm_cvtsi128_si64(result5);
    printf("_mm_unpackhi_epi64 上位: 0x%016llX\n", res_val);    //0x89ABCDEF89ABCDEF
    //_MM_SHUFFLE(1,0,3,2)した場合
    //_mm_unpackhi_epi64 上位: 0x89ABCDEF89ABCDEF
    //_MM_SHUFFLE(0,1,2,3)した場合
    //0xCDEF89ABCDEF89AB が期待できるが
    //_mm_unpackhi_epi64 上位 : 0x89ABCDEF89ABCDEF
    //とかわらない
    //_MM_SHUFFLE(2,3,0,1)した場合
    //_mm_unpackhi_epi64 上位: 0x0123456701234567
    //となる。
    //_MM_SHUFFLE(3,2,1,0)した場合
    //_mm_unpackhi_epi64 上位 : 0x0123456701234567
    //となる。64bit単位のシャッフルは有効に働いているが、32bit単位のシャッフルは全く利いていない

    //_mm_unpacklo_epi64
    __m128i a8 = _mm_set_epi64x(0x0000000000000000,0x0123456701234567);
    __m128i b8 = _mm_set_epi64x(0x0000000000000000,0x89ABCDEF89ABCDEF);
    __m128i result8 = _mm_unpacklo_epi64(a8,b8);
    //result8:[ b[0] (bits 127:64) | a[0] (bits 63:0) ]のa[0]を取り出してres_valに代入している
    res_val = _mm_cvtsi128_si64(result8);
    printf("_mm_unpacklo_epi64 下位: 0x%016llX\n", res_val);    //0x0123456701234567

    return 0;
}

//1 _mm256_loadu_si256  
    /*メモリのアドレス256ベクトルにデータをロードする。変数の型は符号あり整数loaduとあるのはunaligned（非アラインメント）なデータでも読み込める。
    loadとあるのはアライメントがそろっている必要がある。つまり32バイト境界に揃っている必要がある。
    si256とあるのは符号付き整数(256bit幅)*/
//2 _mm256_storeu_si256
    /* 「アラインメントを強制しない 256ビット整数ベクトルのストア（書き込み）」 を行う組み込み関数です。
    si256は上の説明と同じ*/
//3 _mm256_min_epi32 
    /*SIMD 組み込み関数（intrinsic） で、 256 ビット幅のベクトルに詰められた 32 ビット符号付き整数の要素ごとの最小値を計算する命令です。
    引数a,b(__mm256i型）をとりa,bのレーンごとに最小値を返す a[i] < b[i] ならb[i]を返す
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    結果は
    dst = [min(a0,b0), min(a1,b1), min(a2,b2), min(a3,b3), min(a4,b4), min(a5,b5), min(a6,b6), min(a7,b7)]*/
//4 _mm256_max_epi32
    /*SIMD 組み込み関数（intrinsic） で、 256 ビット幅のベクトルに詰められた 32 ビット符号付き整数の要素ごとの最大値を計算する命令です。
    引数a, b(__mm256i型）をとりa, bのレーンごとに最大値を返す a[i] < b[i] ならb[i]を返す
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    結果は
    dst = [max(a0, b0), max(a1, b1), max(a2, b2), max(a3, b3), max(a4, b4), max(a5, b5), max(a6, b6), max(a7, b7)] */
//5 _mm256_set1_epi32
    /*1 つの 32 ビット符号付き整数（int32_t 相当）を引数に取り、
    それを 256 ビット SIMD レジスタ（__m256i 型）の全レーン（8 要素）に同じ値でコピー します。*/

//6 _mm256_cmpeq_epi32
    /*256 ビット幅のパックド 32 ビット整数同士を要素ごとに比較し、等しいかどうかを判定するものです。
    _mm256_cmpeq_epi32(a256, key256)でkey256に探す値をレーンに設定してある。このkeyとa256を比較して
    同じであれば(a256[i]=＝key256[i])32bitが全て１（0xFFFFFFFF）を返し等しくなければ0を返す*/
//7 _mm256_testz_si256
    /*256 ビット幅のパックド 32 ビット整数同士を要素ごとに比較し、等しいかどうかを判定するものです。
    _mm256_testz_si256(equals256, equals256)でequals256の中身が全て0であれば1を返す。つまりkeyと等しい値がなければ1を返す
    つまりインデックス検査をする価値があるかどうかをここでチエックしている*/
//8 _mm256_castsi256_ps
    /*__m256i型の整数ベクトルを__m256型に読み替える*/
//9 _mm256_movemask_ps
    /*__m256型の浮動小数点ベクトルの各要素の符号ビットを抽出し、8 ビットのマスクを作成する
    __m256 vec = _mm256_setr_ps(1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f);
    int mask = _mm256_movemask_ps(vec);
    このとき、符号ビットの並びは（LSB → MSB の順に）：
    レーン0: +1.0 → 0  1.0は正の数なので符号ビットは0   LSB
    レーン1: -2.0 → 1  2.0は負の数なので符号ビットは1   LSB + 1
    レーン2: +3.0 → 0  3.0は正の数なので符号ビットは0   LSB + 2
    レーン3: -4.0 → 1  4.0は負の数なので符号ビットは1   LSB + 3
    レーン4: +5.0 → 0  5.0は正の数なので符号ビットは0   LSB + 4
    レーン5: -6.0 → 1  -6.0は負の数なので符号ビットは1  LSB + 5
    レーン6: +7.0 → 0  7.0は正の数なので符号ビットは0   LSB + 6
    レーン7: -8.0 → 1  -8.0は負の数なので符号ビットは1  LSB + 7
    0b10101010 = 0xAA となる。つまり、maskのビットパターンは、各レーンの符号ビットを表す8ビットの整数として表される
    もしベクトルが全て正の数であれば、maskは0b00000000 = 0x00となる。
    */
//10 _mm256_setzero_si256
    /*256 ビット幅の整数ベクトルをゼロで初期化する*/

//11 _mm256_mullo_epi32
    /*32ビット符号付き整数（int32_t）のベクトル同士を要素ごとに掛け算し、その下位 32 ビットを結果として返す機能を持ちます.
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    __m256i c = _mm256_mullo_epi32(a, b);
    c = [a0*b0, a1*b1, a2*b2, a3*b3, a4*b4, a5*b5, a6*b6, a7*b7]
    各要素の乗算結果は 64 ビットになり得ますが、この命令はその下位 32 ビットだけを結果として格納します（オーバーフローは無視されます）。
*/
//12 _mm256_add_epi32
    /*32ビット符号付き整数（int32_t）のベクトル同士を要素ごとに加算する機能を持ちます。
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    __m256i c = _mm256_add_epi32(a, b);
    c = [a0+b0, a1+b1, a2+b2, a3+b3, a4+b4, a5+b5, a6+b6, a7+b7]
    各要素の加算結果は 32 ビットで表現されます。オーバーフローが発生した場合、結果はラップアラウンドします。
    */
//13 _mm256_permute2x128_si256
    /*256 ビット幅の整数ベクトルを 128 ビット単位で入れ替える機能を持ちます。
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    __m256i c = _mm256_permute2x128_si256(a, b, imm8);
    imm8 の値によって、a と b の 128 ビット単位のブロックをどのように組み合わせるかが決まります。
    例えば、imm8 = 0x01 の場合、c は次のようになります：
    c = [a0, a1, a2, a3, b0, b1, b2, b3]
    つまり、a の下位 128 ビットと b の下位 128 ビットを結合した結果となります。
    */
//14 _mm256_hadd_epi32
    /*256 ビット幅の整数ベクトルの要素を水平方向に加算する機能を持ちます。
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    b = [b0, b1, b2, b3, b4, b5, b6, b7]
    __m256i c = _mm256_hadd_epi32(a, b);
    c = [a0+a1, a2+a3, a4+a5, a6+a7, b0+b1, b2+b3, b4+b5, b6+b7]
    つまり、各 128 ビット単位で隣接する要素を加算し、新しいベクトルを生成します。
    */
//15_mm256_extract_epi32
    /*256 ビット幅の整数ベクトルから指定したインデックスの 32 ビット整数を抽出する機能を持ちます。
    a = [a0, a1, a2, a3, a4, a5, a6, a7]
    int value = _mm256_extract_epi32(a, index);
    index の値に応じて、a の中から対応する要素が抽出されます。
    例えば、index = 3 の場合、value は a3 の値となります。
*/

// 32 ビット符号付整数の 8 個の要素を持つベクトルの中から、
// 最初に負の要素が見つかったインデックスを求める関数。
unsigned long find_first_non_zero_index_epi32(__m256i a) {
    unsigned long index;
    __m256 floating_point_a = _mm256_castsi256_ps(a);
    int mask = _mm256_movemask_ps(floating_point_a);
    _BitScanForward(&index, mask);
    return index;
}

int simd_256() {
    // 配列 a の中から最小値を求める関数。
    int a[] = { 3,1,4,1,5,9,2,6 };
    int b[] = { 1,2,3,4,5,6,7,8 };
    int length = sizeof(a) / sizeof(a[0]);

    // 最小値を最大の整数で初期化。
    __m256i min_value256 = _mm256_set1_epi32(INT_MAX);
    // 各要素を 8 個ずつ処理。
    int i = 0;
    for (; i + 7 < length; i += 8) {
        __m256i a256 = _mm256_loadu_si256((__m256i*) & a[i]);
        min_value256 = _mm256_min_epi32(min_value256, a256);
    }
    // 最小値をスカラー値に変換。
    int result[8];
    _mm256_storeu_si256((__m256i*)result, min_value256);
    int min_value = result[0];
    for (int j = 1; j < 8; j++) {
        if (result[j] < min_value) {
            min_value = result[j];
        }
    }
    // 残りの要素を処理。
    // ここは汎用命令。
    for (; i < length; i++) {
        if (a[i] < min_value) {
            min_value = a[i];
        }
    }
    printf("min_value:%d\n", min_value);

    // 配列 a の中から最大値を求める関数。    
    i = 0;
    // 最大値を最小の整数で初期化。
    __m256i max_value256 = _mm256_set1_epi32(INT_MIN);
    // 各要素を 8 個ずつ処理。
    for (; i + 7 < length; i += 8) {
        __m256i a256 = _mm256_loadu_si256((__m256i*) & a[i]);
        max_value256 = _mm256_max_epi32(max_value256, a256);
    }
    // 最大値をスカラー値に変換。
    for (int i = 0; i < 8; i += 1) {
        result[i]=0;
    }
    _mm256_storeu_si256((__m256i*)result, max_value256);
    int max_value = result[0];
    for (int j = 1; j < 8; j++) {
        if (result[j] > max_value) {
            max_value = result[j];
        }
    }
    // 残りの要素を処理。
    // ここは汎用命令。
    for (; i < length; i++) {
        if (a[i] > max_value) {
            max_value = a[i];
        }
    }
    // 最大値を返す。
    printf("max_value:%d\n",max_value);


    // 配列 a の中から key と等しい要素のインデックスを求める関数。
    int key = 9;
    i = 0;
    __m256i key256 = _mm256_set1_epi32(key);
    // 各要素を 8 個ずつ処理。
    for (; i + 7 < length; i += 8) {
        __m256i a256 = _mm256_loadu_si256((__m256i*)(&a[i]));
        __m256i equals256 = _mm256_cmpeq_epi32(a256, key256);
        // 8 個の要素の中に key と等しい要素があるかどうかを判定。
        if (!_mm256_testz_si256(equals256, equals256)) {
            return i + find_first_non_zero_index_epi32(equals256);
        }
    }
    // 残りの要素を処理。ここは汎用命令。
    for (; i < length; i++) {
        if (key == a[i]) {
            return i;
        }
    }
    printf("index: %d\n", i);

    // ベクトルの内積を求める関数。
    i = 0;
    // 合計を 0 で初期化。
    __m256i dot_product256 = _mm256_setzero_si256();
    // 各要素を 8 個ずつ処理。
    for (; i + 7 < length; i += 8) {
        __m256i a256 = _mm256_loadu_si256((__m256i*)(&a[i]));
        __m256i b256 = _mm256_loadu_si256((__m256i*)(&b[i]));

        __m256i product256 = _mm256_mullo_epi32(a256, b256);
        dot_product256 = _mm256_add_epi32(dot_product256, product256);
    }
    // スカラー値に変換。
    __m256i permute = _mm256_permute2x128_si256(dot_product256, dot_product256, 1);
    __m256i sum256 = _mm256_hadd_epi32(dot_product256, permute);
    sum256 = _mm256_hadd_epi32(sum256, sum256);
    sum256 = _mm256_hadd_epi32(sum256, sum256);
    int dot_product = _mm256_extract_epi32(sum256, 0);
    // 残りの要素を処理。ここは汎用命令。
    for (; i < length; i++) {
        dot_product += a[i] * b[i];
    }

    //test_mm256_extract_epi32
    int a1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    __m256i a256 = _mm256_loadu_si256((__m256i*)a1);
    int value = _mm256_extract_epi32(a256, 3);
    printf("a[%d] = %d\n", 3, value);
}

void scalar_multiplication(int* a, int row, int column, int scalar){
    int i = 0;
    __m256i scalar256 = _mm256_set1_epi32(scalar);
    // 8 要素ずつ計算する。
    for (; i < row * column; i += 8){
        __m256i a256 = _mm256_loadu_si256((__m256i*)(&a[i]));
        __m256i product256 = _mm256_mullo_epi32(a256, scalar256);
        _mm256_storeu_si256((__m256i*)(&a[i]), product256);
    }
    // 残りの要素を処理。ここは汎用命令。
    for (; i < row * column; i++){
        a[i] *= scalar;
    }
}

void get_cpuid(int eax, int ecx, int regs[4]) {
    __cpuidex(regs, eax, ecx);
}

static void print_support(const char* name, int supported) {
    printf("%-12s : %s\n", name, supported ? "[○] Supported" : "[×] Not Supported");
}

int simd_info() {
    int regs[4]; // eax, ebx, ecx, edx
    // EAX=1: 標準機能フラグの取得
    get_cpuid(1, 0, regs);
    int ecx1 = regs[2];
    int edx1 = regs[3];
    // EAX=7, ECX=0: 拡張機能フラグの取得
    get_cpuid(7, 0, regs);
    int ebx7 = regs[1];
    printf("======================================\n");
    printf("     SIMD Instruction Support Check   \n");
    printf("======================================\n\n");
    // SSE 系の判定 (EAX=1)
    printf("--- MMX / SSE Series ---\n");
    print_support("MMX", (edx1 & (1 << 23)) != 0);
    print_support("SSE", (edx1 & (1 << 25)) != 0);
    print_support("SSE2", (edx1 & (1 << 26)) != 0);
    print_support("SSE3", (ecx1 & (1 << 0)) != 0);
    print_support("SSSE3", (ecx1 & (1 << 9)) != 0);
    print_support("SSE4.1", (ecx1 & (1 << 19)) != 0);
    print_support("SSE4.2", (ecx1 & (1 << 20)) != 0);
    // AVX / FMA 系の判定 (EAX=1 / EAX=7)
    printf("\n--- AVX Series ---\n");
    print_support("FMA3", (ecx1 & (1 << 12)) != 0);
    print_support("AVX", (ecx1 & (1 << 28)) != 0);
    print_support("AVX2", (ebx7 & (1 << 5)) != 0);
    // AVX-512 系の判定 (EAX=7)
    printf("\n--- AVX-512 Series ---\n");
    print_support("AVX512F", (ebx7 & (1 << 16)) != 0); // Foundation
    print_support("AVX512DQ", (ebx7 & (1 << 17)) != 0); // Doubleword/Quadword
    print_support("AVX512CD", (ebx7 & (1 << 28)) != 0); // Conflict Detection
    print_support("AVX512BW", (ebx7 & (1 << 30)) != 0); // Byte/Word
    print_support("AVX512VL", (ebx7 & (1 << 31)) != 0); // Vector Length Extensions
    return 0;
}