// Nakamichi is 100% FREE LZSS SUPERFAST decompressor.
// Home of Nakamichi: www.sanmayce.com/Nakamichi/index.html

// silk used in artwork : kenpon
// Kinu - silk, Kinuno - silken
// tiger : tora

// Nakamichi_Kinutora.c, using 4KB/1MB or (16-4)bit/(24-4)bit windows with 2/3 bytes long offsets.
// Nakamichi_Butsuhira.c, using 8KB/2MB or (16-3)bit/(24-3)bit windows with 2/3 bytes long offsets.
// Nakamichi_Kinroba.c, using 4KB/1MB/256MB or 12bit/20bit/28bit windows with 2/3/4 bytes long offsets.
// Nakamichi_Keigan.c, a branchless 'Kaibutsu' it is using 2MB/512MB or 21bit/29bit windows with 3/4 bytes long offsets.
// Nakamichi_Washi.c, a branchless 'Kaibutsu' it is using 4MB window.
/*
D:\Nakamichi_Kinroba>Nakamichi_Kinroba_YMMless.exe dickens
Nakamichi 'Kinroba', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Compressing 10192446 bytes ...
\; Each rotation means 64KB are encoded; Done 100%
NumberOfFullLiterals (lower-the-better): 81
NumberOfTinyMatchesSmallWindow (4): 226718
NumberOfShortMatchesSmallWindow (8): 89670
NumberOfMediumMatchesSmallWindow (16): 9084
NumberOfLongMatchesSmallWindow (32): 540
NumberOfTinyMatchesRegularWindow (4): 233284
NumberOfShortMatchesRegularWindow (8): 616461
NumberOfMediumMatchesRegularWindow (16): 45814
NumberOfLongMatchesRegularWindow (32): 3109
NumberOfTinyMatchesBigWindow (4): 0
NumberOfShortMatchesBigWindow (8): 114466
NumberOfMediumMatchesBigWindow (16): 39893
NumberOfLongMatchesBigWindow (32): 1861
RAM-to-RAM performance: 1 KB/s.

D:\Nakamichi_Kinroba>dir

08/13/2014  06:49 AM         3,153,408 CalgaryCorpus.tar
08/13/2014  06:46 AM         1,346,675 CalgaryCorpus.tar.Nakamichi
08/13/2014  08:17 AM        10,192,446 dickens
08/13/2014  08:13 AM         4,111,829 dickens.Nakamichi
06/03/2014  07:35 PM         5,582,655 shaks12.txt
08/13/2014  08:50 AM         2,294,269 shaks12.txt.Nakamichi
*/

// Nakamichi_Kaiju.c, a branchless 'Kaibutsu' it is.
// ML=9
// 68,352,060 enwik8.Kaiju.Nakamichi
// ML=8
// 63,748,036 enwik8.Kaiju.Nakamichi
// ML=7
// 59,771,603 enwik8.Kaiju.Nakamichi
// ML=6
// 57,090,382 enwik8.Kaiju.Nakamichi
// ML=5
// 56,188,976
// ML=4
// 58,954,436 enwik8.Kaiju.Nakamichi
// Nakamichi_Kaibutsu.c, three small tweaks in Kaidanji, a good idea to remove shiftings altogether by m^2 was used.
// Nakamichi_Kaidanji.c, is the very same '1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX'.

// Nakamichi, revision 1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX, written by Kaze, babealicious suggestion by m^2 enforced.
// Fixed! TO-DO: Known bug: the decompressed file sometimes has few additional bytes at the end.
// Change #1: Now instead of looking first in the leftmost end of the window a "preemptive" search is done 16*8*128 bytes before the rightmost end of the window, there is the hottest (cachewise&matchwise) zone, as a side effect the compression speed is much higher. Maybe in the future I will try hashing as well.
// Change #2: The full 16bits are used for offsets, 64KB window, that is.

// Compile line:
//icl /O3 Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.c -D_N_GP /FAcs
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.cod Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_GP.cod
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.exe Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_GP.exe
//icl /O3 /QxSSE2 Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.c -D_N_XMM /FAcs
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.cod Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_XMM.cod
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.exe Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_XMM.exe
//icl /O3 /QxAVX Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.c -D_N_YMM /FAcs
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.cod Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_YMM.cod
//ren Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX.exe Nakamichi_r1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX_Kaidanji_FIX_YMM.exe

// Nakamichi, revision 1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy_FIX, written by Kaze, babealicious suggestion by m^2 enforced.
// Change #1: Nasty bug in Swampshine was fixed.
// Change #2: Sanity check in compression section was added thus avoiding 'index-Min_Match_Length' going below 0.

// Nakamichi, revision 1-RSSBO_1GB_Wordfetcher_TRIAD_NOmemcpy, written by Kaze, babealicious suggestion by m^2 enforced.
// Change #1: 'memcpy' replaced by GP/XMM/YMM TRIADs.
// Nakamichi, revision 1-RSSBO_1GB_Wordfetcher_TRIAD, written by Kaze.
// Change #1: Decompression fetches WORD instead of BYTE+BYTE.
// Change #2: Decompression stores three times 64bit instead of memcpy() for all transfers <=24 bytes.
// Change #3: Fifteenth bit is used and then unused, 16KB -> 32KB -> 16KB.
// 32KB window disappoints speedwise, also sizewise:
/*
D:\_KAZE\_KAZE_GOLD\Nakamichi_projectOLD\Nakamichi_vs_Yappy>Nakamichi_r1-RSSBO_1GB_15bit_Wordfetcher.exe enwik8
Nakamichi, revision 1-RSSBO_1GB_15bit_Wordfetcher, written by Kaze, based on Nobuo Ito's LZSS source.
Compressing 100000000 bytes ...
-; Each rotation means 128KB are encoded; Done 100%
RAM-to-RAM performance: 130 KB/s.

D:\_KAZE\_KAZE_GOLD\Nakamichi_projectOLD\Nakamichi_vs_Yappy>Nakamichi_r1-RSSBO_1GB_15bit_Wordfetcher.exe enwik8.Nakamichi
Nakamichi, revision 1-RSSBO_1GB_15bit_Wordfetcher, written by Kaze, based on Nobuo Ito's LZSS source.
Decompressing 65693566 bytes ...
RAM-to-RAM performance: 358 MB/s.

D:\_KAZE\_KAZE_GOLD\Nakamichi_projectOLD\Nakamichi_vs_Yappy>Nakamichi_r1-RSSBO_1GB_15bit_Wordfetcher.exe enwik9
Nakamichi, revision 1-RSSBO_1GB_15bit_Wordfetcher, written by Kaze, based on Nobuo Ito's LZSS source.
Compressing 1000000000 bytes ...
/; Each rotation means 128KB are encoded; Done 100%
RAM-to-RAM performance: 150 KB/s.

D:\_KAZE\_KAZE_GOLD\Nakamichi_projectOLD\Nakamichi_vs_Yappy>Nakamichi_r1-RSSBO_1GB_15bit_Wordfetcher.exe enwik9.Nakamichi
Nakamichi, revision 1-RSSBO_1GB_15bit_Wordfetcher, written by Kaze, based on Nobuo Ito's LZSS source.
Decompressing 609319736 bytes ...
RAM-to-RAM performance: 379 MB/s.
*/
// 1-RSSBO_1GB vs 1-RSSBO_1GB_15bit_Wordfetcher (16KB/32KB respectively):
// 069,443,065 vs 065,693,566
// 641,441,055 vs 609,319,736

// Nakamichi, revision 1-RSSBO_1GB, written by Kaze.
// Based on Nobuo Ito's source, thanks Ito.
// The main goal of Nakamichi is to allow supersimple and superfast decoding for English x-grams (mainly) in pure C, or not, heh-heh.
// Natively Nakamichi is targeted as 64bit tool with 16 threads, helping Kazahana to traverse faster when I/O is not superior.
// In short, Nakamichi is intended as x-gram decompressor.

// Eightfold Path ~ the Buddhist path to nirvana, comprising eight aspects in which an aspirant must become practised; 
// eightfold way ~ (Physics), the grouping of hadrons into supermultiplets by means of SU(3)); (b) adverb to eight times the number or quantity: OE.

// Note1: Fifteenth bit is not used, making the window wider by 1bit i.e. 32KB is not tempting, rather I think to use it as a flag: 8bytes/16bytes.
// Note2: English x-grams are as English texts but more redundant, in other words they are phraselists in most cases, sometimes wordlists.
// Note3: On OSHO.TXT, being a typical English text, Nakamichi's compression ratio is among the worsts:
//        206,908,949 OSHO.TXT
//        125,022,859 OSHO.TXT.Nakamichi
//        It struggles with English texts but decomprression speed is quite sweet (Core 2 T7500 2200MHz, 32bit code):
//        Nakamichi, revision 1-, written by Kaze.
//        Decompressing 125022859 bytes ...
//        RAM-to-RAM performance: 477681 KB/s.      
// Note4: Also I wanted to see how my 'Railgun_Swampshine_BailOut', being a HEAVYGUN i.e. with big overhead and latency, hits in a real-world application.

// Quick notes on PAGODAs (the padded x-gram lists):
// Every single word in English has its own PAGODA, in example below 'on' PAGODA is given (Kazahana_on.PAGODA-order-5.txt):
// PAGODA order 5 (i.e. with 5 tiers) has 5*(5+1)/2=15 subtiers, they are concatenated and space-padded in order to form the pillar 'on':
/*
D:\_KAZE\Nakamichi_r1-RSSBO>dir \_GW\ka*

04/12/2014  05:07 AM                14 Kazahana_on.1-1.txt
04/12/2014  05:07 AM         1,635,389 Kazahana_on.2-1.txt
04/12/2014  05:07 AM         1,906,734 Kazahana_on.2-2.txt
04/12/2014  05:07 AM        10,891,415 Kazahana_on.3-1.txt
04/12/2014  05:07 AM        15,797,703 Kazahana_on.3-2.txt
04/12/2014  05:07 AM        20,419,280 Kazahana_on.3-3.txt
04/12/2014  05:07 AM        22,141,823 Kazahana_on.4-1.txt
04/12/2014  05:07 AM        36,002,113 Kazahana_on.4-2.txt
04/12/2014  05:07 AM        33,236,772 Kazahana_on.4-3.txt
04/12/2014  05:07 AM        33,902,425 Kazahana_on.4-4.txt
04/12/2014  05:07 AM        24,795,989 Kazahana_on.5-1.txt
04/12/2014  05:07 AM        30,766,220 Kazahana_on.5-2.txt
04/12/2014  05:07 AM        38,982,816 Kazahana_on.5-3.txt
04/12/2014  05:07 AM        38,089,575 Kazahana_on.5-4.txt
04/12/2014  05:07 AM        34,309,057 Kazahana_on.5-5.txt
04/12/2014  05:07 AM       846,351,894 Kazahana_on.PAGODA-order-5.txt

D:\_KAZE\Nakamichi_r1-RSSBO>type \_GW\Kazahana_on.1-1.txt
9,999,999       on

D:\_KAZE\Nakamichi_r1-RSSBO>type \_GW\Kazahana_on.2-1.txt
9,999,999       on_the
1,148,054       on_his
0,559,694       on_her
0,487,856       on_this
0,399,485       on_your
0,381,570       on_my
0,367,282       on_their
...

D:\_KAZE\Nakamichi_r1-RSSBO>type \_GW\Kazahana_on.2-2.txt
0,545,191       based_on
0,397,408       and_on
0,334,266       go_on
0,329,561       went_on
0,263,035       was_on
0,246,332       it_on
0,229,041       down_on
0,202,151       going_on
...

D:\_KAZE\Nakamichi_r1-RSSBO>type \_GW\Kazahana_on.5-5.txt
0,083,564       foundation_osho_s_books_on
0,012,404       medium_it_may_be_on
0,012,354       if_you_received_it_on
0,012,152       medium_they_may_be_on
0,012,144       agree_to_also_provide_on
0,012,139       a_united_states_copyright_on
0,008,067       we_are_constantly_working_on
0,008,067       questions_we_have_received_on
0,006,847       file_was_first_posted_on
0,006,441       of_we_are_already_on
0,006,279       you_received_this_ebook_on
0,005,865       you_received_this_etext_on
0,005,833       to_keep_an_eye_on
...

D:\_KAZE\Nakamichi_r1-RSSBO>dir

04/12/2014  05:07 AM       846,351,894 Kazahana_on.PAGODA-order-5.txt

D:\_KAZE\Nakamichi_r1-RSSBO>Nakamichi.exe Kazahana_on.PAGODA-order-5.txt
Nakamichi, revision 1-RSSBO, written by Kaze.
Compressing 846351894 bytes ...
/; Each rotation means 128KB are encoded; Done 100%
RAM-to-RAM performance: 512 KB/s.

D:\_KAZE\Nakamichi_r1-RSSBO>dir

04/12/2014  05:07 AM       846,351,894 Kazahana_on.PAGODA-order-5.txt
04/15/2014  06:30 PM       293,049,398 Kazahana_on.PAGODA-order-5.txt.Nakamichi

D:\_KAZE\Nakamichi_r1-RSSBO>Nakamichi.exe Kazahana_on.PAGODA-order-5.txt.Nakamichi
Nakamichi, revision 1-RSSBO, written by Kaze.
Decompressing 293049398 bytes ...
RAM-to-RAM performance: 607 MB/s.

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 4096
YAPPY: [b 4K] bytes 846351894 -> 191149889  22.6%  comp  33.8 MB/s  uncomp 875.4 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 8192
YAPPY: [b 8K] bytes 846351894 -> 184153244  21.8%  comp  35.0 MB/s  uncomp 898.3 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 16384
YAPPY: [b 16K] bytes 846351894 -> 180650931  21.3%  comp  28.8 MB/s  uncomp 906.4 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 32768
YAPPY: [b 32K] bytes 846351894 -> 178902966  21.1%  comp  35.0 MB/s  uncomp 906.4 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 65536
YAPPY: [b 64K] bytes 846351894 -> 178027899  21.0%  comp  34.5 MB/s  uncomp 914.6 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>Yappy.exe Kazahana_on.PAGODA-order-5.txt 131072
YAPPY: [b 128K] bytes 846351894 -> 177591807  21.0%  comp  34.9 MB/s  uncomp 906.4 MB/s

D:\_KAZE\Nakamichi_r1-RSSBO>
*/

// During compilation use one of these, the granularity of the padded 'memcpy', 4x2x8/2x2x16/1x2x32/1x1x64 respectively as GP/XMM/YMM/ZMM, the maximum literal length reduced from 127 to 63:
#define _N_GP
//#define _N_XMM
//#define _N_YMM
//#define _N_ZMM

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint64_t needed
#include <time.h>
#include <string.h>

#ifdef _N_XMM
#include <emmintrin.h> // SSE2 intrinsics
#include <smmintrin.h> // SSE4.1 intrinsics
#endif
#ifdef _N_YMM
#include <emmintrin.h> // SSE2 intrinsics
#include <smmintrin.h> // SSE4.1 intrinsics
#include <immintrin.h> // AVX intrinsics
#endif
#ifdef _N_ZMM
#include <emmintrin.h> // SSE2 intrinsics
#include <smmintrin.h> // SSE4.1 intrinsics
#include <immintrin.h> // AVX intrinsics
#include <zmmintrin.h> // AVX2 intrinsics, definitions and declarations for use with 512-bit compiler intrinsics.
#endif

#ifdef _N_XMM
void SlowCopy128bit (const char *SOURCE, char *TARGET) { _mm_storeu_si128((__m128i *)(TARGET), _mm_loadu_si128((const __m128i *)(SOURCE))); }
#endif
#ifdef _N_YMM
void SlowCopy128bit (const char *SOURCE, char *TARGET) { _mm_storeu_si128((__m128i *)(TARGET), _mm_loadu_si128((const __m128i *)(SOURCE))); }
#endif
#ifdef _N_ZMM
void SlowCopy128bit (const char *SOURCE, char *TARGET) { _mm_storeu_si128((__m128i *)(TARGET), _mm_loadu_si128((const __m128i *)(SOURCE))); }
#endif
/*
 * Move Unaligned Packed Integer Values
 * **** VMOVDQU ymm1, m256
 * **** VMOVDQU m256, ymm1
 * Moves 256 bits of packed integer values from the source operand to the
 * destination
 */
//extern __m256i __ICL_INTRINCC _mm256_loadu_si256(__m256i const *);
//extern void    __ICL_INTRINCC _mm256_storeu_si256(__m256i *, __m256i);
#ifdef _N_YMM
void SlowCopy256bit (const char *SOURCE, char *TARGET) { _mm256_storeu_si256((__m256i *)(TARGET), _mm256_loadu_si256((const __m256i *)(SOURCE))); }
#endif
//extern __m512i __ICL_INTRINCC _mm512_loadu_si512(void const*);
//extern void    __ICL_INTRINCC _mm512_storeu_si512(void*, __m512i);
#ifdef _N_ZMM
void SlowCopy512bit (const char *SOURCE, char *TARGET) { _mm512_storeu_si512((__m512i *)(TARGET), _mm512_loadu_si512((const __m512i *)(SOURCE))); }
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

// Comment it to see how slower 'BruteForce' is, for Wikipedia 100MB the ratio is 41KB/s versus 197KB/s.
#define ReplaceBruteForceWithRailgunSwampshineBailOut

static void SearchIntoSlidingWindow(unsigned int* ShortMediumLongOFFSET, unsigned int* retIndex, unsigned int* retMatch, char* refStart,char* refEnd,char* encStart,char* encEnd);
static unsigned int SlidingWindowVsLookAheadBuffer(char* refStart, char* refEnd, char* encStart, char* encEnd);
unsigned int KinutoraCompress(char* ret, char* src, unsigned int srcSize);
unsigned int KinutoraDecompress(char* ret, char* src, unsigned int srcSize);
static char * Railgun_Swampshine_BailOut(char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern);
static char * Railgun_Doublet (char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern);

// The pair SHORT/LONG to be respectively in range 3..8/9..24:
// 4/12:
// 846,351,894 Kazahana_on.PAGODA-order-5.txt
// 219,459,398 Kazahana_on.PAGODA-order-5.txt.Nakamichi
// 6/13:
// 846,351,894 Kazahana_on.PAGODA-order-5.txt
// 213,629,110 Kazahana_on.PAGODA-order-5.txt.Nakamichi
// 5/13:
//   846,351,894 Kazahana_on.PAGODA-order-5.txt
//   210,396,428 Kazahana_on.PAGODA-order-5.txt.Nakamichi
//   206,908,949 OSHO.TXT
//    99,739,184 OSHO.TXT.Nakamichi
// 1,000,000,000 enwik9
//   531,893,878 enwik9.Nakamichi
// 6/14:
// 846,351,894 Kazahana_on.PAGODA-order-5.txt
// 207,213,691 Kazahana_on.PAGODA-order-5.txt.Nakamichi
// 5/14:
// 846,351,894 Kazahana_on.PAGODA-order-5.txt
// 205,946,653 Kazahana_on.PAGODA-order-5.txt.Nakamichi
// 5/8:
// 1,000,000,000 enwik9
//   525,215,362 enwik9.Nakamichi
//   846,351,894 Kazahana_on.PAGODA-order-5.txt
//   271,833,018 Kazahana_on.PAGODA-order-5.txt.Nakamichi
//   206,908,949 OSHO.TXT
//    96,001,059 OSHO.TXT.Nakamichi

// Min_Match_Length=THRESHOLD=4 means 4 and bigger are to be encoded:
#define Min_Match_BAILOUT_Length (8)
#define Min_Match_Length (32)
#define Min_Match_Length_SHORT (5)
#define OffsetBITS (24-4)
#define LengthBITS (1)

//12bit
//#define REF_SIZE (4095+Min_Match_Length)
//#define REF_SIZE ( ((1<<OffsetBITS)-1) + Min_Match_Length )
#define REF_SIZE ( ((1<<OffsetBITS)-1) )
//3bit
//#define ENC_SIZE (7+Min_Match_Length)
#define ENC_SIZE ( ((1<<LengthBITS)-1) + Min_Match_Length )
/*
int main( int argc, char *argv[] ) {
	FILE *fp;
	int SourceSize;
	int TargetSize;
	char* SourceBlock=NULL;
	char* TargetBlock=NULL;
	char* Nakamichi = ".Nakamichi\0";
	char NewFileName[256];
	clock_t clocks1, clocks2;

char *pointerALIGN;
int i, j;
clock_t clocks3, clocks4;
double duration;
int BandwidthFlag=0;

unsigned long long k;

	printf("Nakamichi 'Kinutora', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced, muffinesque suggestion by Jim Dempsey enforced.\n");
	if (argc==1) {
		printf("Usage: Nakamichi filename\n"); exit(13);
	}
	if (argc==3) BandwidthFlag=1;
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		printf("Nakamichi: Can't open '%s' file.\n", argv[1]); exit(13);
	}
	fseek(fp, 0, SEEK_END);
	SourceSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	// If filename ends in '.Nakamichi' then mode is decompression otherwise compression.
	if (strcmp(argv[1]+(strlen(argv[1])-strlen(Nakamichi)), Nakamichi) == 0) {
	SourceBlock = (char*)malloc(SourceSize+512);
	//TargetBlock = (char*)malloc(5*SourceSize+512);
	TargetBlock = (char*)malloc(1111*1024*1024+512);
	fread(SourceBlock, 1, SourceSize, fp);
	fclose(fp);
		printf("Decompressing %d bytes ...\n", SourceSize );
		clocks1 = clock();
		while (clocks1 == clock());
		clocks1 = clock();
		TargetSize = Decompress(TargetBlock, SourceBlock, SourceSize);
		clocks2 = clock();
		k = (((float)1000*TargetSize/(clocks2 - clocks1 + 1))); k=k>>20;
		printf("RAM-to-RAM performance: %d MB/s.\n", k);
		strcpy(NewFileName, argv[1]);
		*( NewFileName + strlen(argv[1])-strlen(Nakamichi) ) = '\0';
	} else {
	SourceBlock = (char*)malloc(SourceSize+512);
	TargetBlock = (char*)malloc(SourceSize+512+32*1024*1024); //+32*1024*1024, some files may be expanded instead of compressed.
	fread(SourceBlock, 1, SourceSize, fp);
	fclose(fp);
		printf("Compressing %d bytes ...\n", SourceSize );
		clocks1 = clock();
		while (clocks1 == clock());
		clocks1 = clock();
		TargetSize = Compress(TargetBlock, SourceBlock, SourceSize);
		clocks2 = clock();
		k = (((float)1000*SourceSize/(clocks2 - clocks1 + 1))); k=k>>10;
		printf("RAM-to-RAM performance: %d KB/s.\n", k);
		strcpy(NewFileName, argv[1]);
		strcat(NewFileName, Nakamichi);
	}
	if ((fp = fopen(NewFileName, "wb")) == NULL) {
		printf("Nakamichi: Can't write '%s' file.\n", NewFileName); exit(13);
	}
	fwrite(TargetBlock, 1, TargetSize, fp);
	fclose(fp);

	if (BandwidthFlag) {
// Benchmark memcpy() [
pointerALIGN = TargetBlock + 64 - (((size_t)TargetBlock) % 64);
//offset=64-int((long)data&63);
printf("Memory pool starting address: %p ... ", pointerALIGN);
if (((uintptr_t)(const void *)pointerALIGN & (64 - 1)) == 0) printf( "64 byte aligned, OK\n"); else printf( "NOT 64 byte aligned, FAILURE\n");
clocks3 = clock();
while (clocks3 == clock());
clocks3 = clock();
printf("Copying a %dMB block 1024 times i.e. %dGB READ + %dGB WRITTEN ...\n", 256, 256, 256);
	for (i = 0; i < 1024; i++) {
	memcpy(pointerALIGN+256*1024*1024, pointerALIGN, 256*1024*1024);
	}
clocks4 = clock();
duration = (double) (clocks4 - clocks3 + 1);
printf("memcpy(): (%dMB block); %dMB copied in %d clocks or %.3fMB per clock\n", 256, 1024*( 256 ), (int) duration, (float)1024*( 256 )/ ((int) duration));
// Benchmark memcpy() ]
k = (((float)1000*TargetSize/(clocks2 - clocks1 + 1))); k=k>>20;
j = (float)1000*1024*( 256 )/ ((int) duration);
printf("RAM-to-RAM performance vs memcpy() ratio (bigger-the-better): %d%%\n", (int)(k*100/j));
	}

	free(TargetBlock);
	free(SourceBlock);
	exit(0);
}*/

void SearchIntoSlidingWindow(unsigned int* ShortMediumLongOFFSET, unsigned int* retIndex, unsigned int* retMatch, char* refStart,char* refEnd,char* encStart,char* encEnd){
	char* FoundAtPosition;
	unsigned int match=0;

// No need of COLD, HOT and HOTTER for big window, but if the priority is swapped i.e. big is sought first then they are needed:

	char* refStartHOT = refEnd-(256*8*128-2);
	char* refStartHOTTER = refEnd-(4*8*128-2);
	char* refStartCOLDERbig = refStart;

	*retIndex=0;
	*retMatch=0;
	*ShortMediumLongOFFSET=0;

#ifdef ReplaceBruteForceWithRailgunSwampshineBailOut

// It appears to be more wise to follow once sizewise order path then speedwise order.
// speedwise order is:
// 32:2 = 16
// 16:2 = 8
// 8:2 = 4
// 4:2 = 2
// 32:3 = 10.6
// 16:3 = 5.3
// 8:3 = 2.6
// 4:3 = 1.3
// sizewise order is:
// 32:2 = 16
// 32:3 = 10.6
// 16:2 = 8
// 16:3 = 5.3
// 8:2 = 4
// 8:3 = 2.6
// 4:2 = 2
// 4:3 = 1.3
// CHOSEN order is:
// 32:2 = 16
// 16:2 = 8
// 32:3 = 10.6
// 8:2 = 4
// 16:3 = 5.3
// 8:3 = 2.6
// 4:2 = 2
// 4:3 = 1.3

// |1stLSB     |2ndLSB  |3rdLSB   |
// --------------------------------
// |T|LL|O|xxxx|xxxxxxxx|xxxxxx|xx|
// --------------------------------
// [1bit           16bit]    24bit]
// T = 0 means Literal
// T = 1 means Match
// LL = 00b means Long MatchLength, 32>>LL or 32
// LL = 01b means Long MatchLength, 32>>LL or 16
// LL = 10b means Long MatchLength, 32>>LL or 8
// LL = 11b means Long MatchLength, 32>>LL or 4
// O = 0 means Long MatchOffset, 3 bytes long i.e. Sliding Window is 3*8-F-LL-O=3*8-4=20 or 1MB
// O = 1 means Short MatchOffset, 2 bytes long i.e. Sliding Window is 2*8-F-LL-O=2*8-4=12 or 4KB

// 32:2 = 16

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOTTER >= refStart)
	if (refStartHOTTER < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOTTER, encStart, (uint32_t)(refEnd-refStartHOTTER), ((Min_Match_Length)>>0));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>0);
				// The first four bits should be:
				                                                                  // 1001b = 9
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFF0)|0x0009; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=2;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 16:2 = 8

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOTTER >= refStart)
	if (refStartHOTTER < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOTTER, encStart, (uint32_t)(refEnd-refStartHOTTER), ((Min_Match_Length)>>1));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>1);
				// The first four bits should be:
				                                                                  // 1011b = B
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFF0)|0x000B; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=2;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 32:3 = 10.6

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOT >= refStart)
	if (refStartHOT < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOT, encStart, (uint32_t)(refEnd-refStartHOT), ((Min_Match_Length)>>0));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>0);
				// The first four bits should be:
				                                                                    // 0001b = 1
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0001; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartCOLDERbig >= refStart)
	if (refStartCOLDERbig < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartCOLDERbig, encStart, (uint32_t)(refEnd-refStartCOLDERbig), ((Min_Match_Length)>>0));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>0);
				// The first four bits should be:
				                                                                    // 0001b = 1
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0001; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 8:2 = 4

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOTTER >= refStart)
	if (refStartHOTTER < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOTTER, encStart, (uint32_t)(refEnd-refStartHOTTER), ((Min_Match_Length)>>2));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>2);
				// The first four bits should be:
				                                                                  // 1101b = D
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFF0)|0x000D; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=2;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 16:3 = 5.3

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOT >= refStart)
	if (refStartHOT < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOT, encStart, (uint32_t)(refEnd-refStartHOT), ((Min_Match_Length)>>1));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>1);
				// The first four bits should be:
				                                                                    // 0011b = 3
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0003; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartCOLDERbig >= refStart)
	if (refStartCOLDERbig < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartCOLDERbig, encStart, (uint32_t)(refEnd-refStartCOLDERbig), ((Min_Match_Length)>>1));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>1);
				// The first four bits should be:
				                                                                    // 0011b = 3
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0003; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 8:3 = 2.6

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOT >= refStart)
	if (refStartHOT < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOT, encStart, (uint32_t)(refEnd-refStartHOT), ((Min_Match_Length)>>2));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>2);
				// The first four bits should be:
				                                                                    // 0101b = 5
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0005; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartCOLDERbig >= refStart)
	if (refStartCOLDERbig < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartCOLDERbig, encStart, (uint32_t)(refEnd-refStartCOLDERbig), ((Min_Match_Length)>>2));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>2);
				// The first four bits should be:
				                                                                    // 0101b = 5
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0005; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 4:2 = 2

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOTTER >= refStart)
	if (refStartHOTTER < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOTTER, encStart, (uint32_t)(refEnd-refStartHOTTER), ((Min_Match_Length)>>3));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>3);
				// The first four bits should be:
				                                                                  // 1111b = F
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFF0)|0x000F; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=2;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

// 4:3 = 1.3

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartHOT >= refStart)
	if (refStartHOT < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartHOT, encStart, (uint32_t)(refEnd-refStartHOT), ((Min_Match_Length)>>3));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>3);
				// The first four bits should be:
				                                                                    // 0111b = 7
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0007; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) [
	if (refStartCOLDERbig >= refStart)
	if (refStartCOLDERbig < refEnd) {
	FoundAtPosition = Railgun_Swampshine_BailOut(refStartCOLDERbig, encStart, (uint32_t)(refEnd-refStartCOLDERbig), ((Min_Match_Length)>>3));	
		if (FoundAtPosition!=NULL) {
				*retMatch=((Min_Match_Length)>>3);
				// The first four bits should be:
				                                                                    // 0111b = 7
				*retIndex=(((refEnd-FoundAtPosition)<<4)&0xFFFFF0)|0x0007; // xx ... x[OLLT]
				*ShortMediumLongOFFSET=3;
				return;
		}
	}
	// Pre-emptive strike, matches should be sought close to the lookahead (cache-friendliness) ]

#else				
	while(refStart < refEnd){
		match=SlidingWindowVsLookAheadBuffer(refStart,refEnd,encStart,encEnd);
		if(match > *retMatch){
			*retMatch=match;
			*retIndex=refEnd-refStart;
		}
		if(*retMatch >= Min_Match_BAILOUT_Length) break;
		refStart++;
	}
#endif
}

unsigned int SlidingWindowVsLookAheadBuffer( char* refStart, char* refEnd, char* encStart,char* encEnd){
	int ret = 0;
	while(refStart[ret] == encStart[ret]){
		if(&refStart[ret] >= refEnd) break;
		if(&encStart[ret] >= encEnd) break;
		ret++;
		if(ret >= Min_Match_BAILOUT_Length) break;
	}
	return ret;
}

unsigned int KinutoraCompress(char* ret, char* src, unsigned int srcSize){
	unsigned int srcIndex=0;
	unsigned int retIndex=0;
	unsigned int index=0;
	unsigned int match=0;
	unsigned int notMatch=0;
	unsigned char* notMatchStart=NULL;
	char* refStart=NULL;
	char* encEnd=NULL;
	/*int Melnitchka=0;
	char *Auberge[4] = {"|\0","/\0","-\0","\\\0"};
	int ProgressIndicator;*/

	unsigned int NumberOfFullLiterals=0;
	int GLOBALlongS=0;
	int GLOBALmediumS=0;
	int GLOBALshortS=0;
	int GLOBALtinyS=0;
	int GLOBALlongM=0;
	int GLOBALmediumM=0;
	int GLOBALshortM=0;
	int GLOBALtinyM=0;
	int GLOBALlongMbig=0;
	int GLOBALmediumMbig=0;
	int GLOBALshortMbig=0;
	int GLOBALtinyMbig=0;
	unsigned int ShortMediumLongOFFSET=0;

	while(srcIndex < srcSize){
		if(srcIndex>=REF_SIZE)
			refStart=&src[srcIndex-REF_SIZE];
		else
			refStart=src;
		if(srcIndex>=srcSize-ENC_SIZE)
			encEnd=&src[srcSize];
		else
			encEnd=&src[srcIndex+ENC_SIZE];
		// Fixing the stupid 'search-beyond-end' bug:
		if(srcIndex+ENC_SIZE < srcSize) {
			SearchIntoSlidingWindow(&ShortMediumLongOFFSET,&index,&match,refStart,&src[srcIndex],&src[srcIndex],encEnd);
			if ( ShortMediumLongOFFSET==3 && match==16 ) GLOBALmediumMbig++;
			if ( ShortMediumLongOFFSET==3 && match==32 ) GLOBALlongMbig++;
		}
		else
			match=0; // Nothing to find.
		//if ( match<Min_Match_Length ) {
		//if ( match<Min_Match_Length || match<8 ) {
  		if ( match==0 ) {
			if(notMatch==0){
				notMatchStart=&ret[retIndex];
				retIndex++;
			}
			//else if (notMatch==(127-64-32)) {
			else if (notMatch==(127-64-32-16)) {
NumberOfFullLiterals++;
				//*notMatchStart=(unsigned char)((127-64-32)<<3);
				*notMatchStart=(unsigned char)((127-64-32-16)<<(4));
				notMatch=0;
				notMatchStart=&ret[retIndex];
				retIndex++;
			}
			ret[retIndex]=src[srcIndex];
			retIndex++;
			notMatch++;
			srcIndex++;
			/*if ((srcIndex-1) % (1<<16) > srcIndex % (1<<16)) {
				ProgressIndicator = (int)( (srcIndex+1)*(float)100/(srcSize+1) );
				printf("%s; Each rotation means 64KB are encoded; Done %d%%\r", Auberge[Melnitchka++], ProgressIndicator );
				Melnitchka = Melnitchka & 3; // 0 1 2 3: 00 01 10 11
			}*/
		} else {
			if(notMatch > 0){
				*notMatchStart=(unsigned char)((notMatch)<<(4));
				notMatch=0;
			}
// ---------------------| 
//                     \ /

			//ret[retIndex] = 0x80; // Assuming seventh/fifteenth bit is zero i.e. LONG MATCH i.e. Min_Match_BAILOUT_Length*4
	  		//if ( match==Min_Match_BAILOUT_Length ) ret[retIndex] = 0xC0; // 8bit&7bit set, SHORT MATCH if seventh/fifteenth bit is not zero i.e. Min_Match_BAILOUT_Length
//                     / \
// ---------------------|
/*
			ret[retIndex] = 0x01; // Assuming seventh/fifteenth bit is zero i.e. LONG MATCH i.e. Min_Match_BAILOUT_Length*4
	  		if ( match==Min_Match_BAILOUT_Length ) ret[retIndex] = 0x03; // 2bit&1bit set, LONG MATCH if 2bit is not zero i.e. Min_Match_BAILOUT_Length
*/
// No need of above, during compression we demanded lowest 2bits to be not 00.
			// 1bit+3bits+12bits:
			//ret[retIndex] = ret[retIndex] | ((match-Min_Match_Length)<<4);
			//ret[retIndex] = ret[retIndex] | (((index-Min_Match_Length) & 0x0F00)>>8);
			// 1bit+1bit+14bits:
			//ret[retIndex] = ret[retIndex] | ((match-Min_Match_Length)<<(8-(LengthBITS+1))); // No need to set the matchlength
// The fragment below is outrageously ineffective - instead of 8bit&7bit I have to use the lower TWO bits i.e. 2bit&1bit as flags, thus in decompressing one WORD can be fetched instead of two BYTE loads followed by SHR by 2.
// ---------------------| 
//                     \ /
			//ret[retIndex] = ret[retIndex] | (((index-Min_Match_Length) & 0x3F00)>>8); // 2+4+8=14
			//retIndex++;
			//ret[retIndex] = (char)((index-Min_Match_Length) & 0x00FF);
			//retIndex++;
//                     / \
// ---------------------|
			// Now the situation is like LOW:HIGH i.e. FF:3F i.e. 0x3FFF, 16bit&15bit used as flags,
			// should become LOW:HIGH i.e. FC:FF i.e. 0xFFFC, 2bit&1bit used as flags.
/*
			ret[retIndex] = ret[retIndex] | (((index-Min_Match_Length) & 0x00FF)<<2); // 6+8=14
			//ret[retIndex] = ret[retIndex] | (((index-Min_Match_Length) & 0x00FF)<<1); // 7+8=15
			retIndex++;
			ret[retIndex] = (char)(((index-Min_Match_Length) & 0x3FFF)>>6);
			//ret[retIndex] = (char)(((index-Min_Match_Length) & 0x7FFF)>>7);
			retIndex++;
*/
// No need of above, during compression we demanded lowest 2bits to be not 00, use the full 16bits and get rid of the stupid '+/-' Min_Match_Length.
			//if (index>0xFFFF) {printf ("\nFatal error: Overflow!\n"); exit(13);}
			//memcpy(&ret[retIndex],&index,2+1); // copy lower 2 bytes
			//retIndex++;
			//retIndex++;
			 //retIndex++;
			memcpy(&ret[retIndex],&index,ShortMediumLongOFFSET);
			retIndex = retIndex + ShortMediumLongOFFSET;
//                     / \
// ---------------------|
			srcIndex+=match;
			/*if ((srcIndex-match) % (1<<16) > srcIndex % (1<<16)) {
				ProgressIndicator = (int)( (srcIndex+1)*(float)100/(srcSize+1) );
				printf("%s; Each rotation means 64KB are encoded; Done %d%%\r", Auberge[Melnitchka++], ProgressIndicator );
				Melnitchka = Melnitchka & 3; // 0 1 2 3: 00 01 10 11
			}*/
		}
	}
	if(notMatch > 0){
		*notMatchStart=(unsigned char)((notMatch)<<(4));
	}
	/*printf("%s; Each rotation means 64KB are encoded; Done %d%%\n", Auberge[Melnitchka], 100 );
	printf("NumberOfFullLiterals (lower-the-better): %d\n", NumberOfFullLiterals );
printf("NumberOfMediumMatchesBigWindow (%d): %d\n", 32>>1, GLOBALmediumMbig);
printf("NumberOfLongMatchesBigWindow (%d): %d\n", 32>>0, GLOBALlongMbig);*/

	return retIndex;
}

/*
unsigned int Decompress(char* ret, char* src, unsigned int srcSize){
	unsigned int srcIndex=0;
	unsigned int retIndex=0;
	unsigned int DWORDtrio;
	unsigned int Flag;
	while(srcIndex < srcSize){
		DWORDtrio = *(unsigned int*)&src[srcIndex];
// |1stLSB     |2ndLSB  |3rdLSB   |
// --------------------------------
// |T|L|O|xxxxx|xxxxxxxx|xxxxxx|xx|
// --------------------------------
// [1bit           16bit]    24bit]
// T = 0 means Literal
// T = 1 means Match
// L = 0 means Long MatchLength, 16>>(L+O) or 8/16
// L = 1 means Short MatchLength, 16>>(L+O) or 4/8
// O = 0 means Long MatchOffset, 3 bytes long i.e. Sliding Window is 3*8-F-L-O=3*8-3=21 or 2MB
// O = 1 means Short MatchOffset, 2 bytes long i.e. Sliding Window is 2*8-F-L-O=2*8-3=13 or 8KB
		if (DWORDtrio & 0x01) {
				#ifndef _N_XMM
		memcpy((ret+retIndex), (const char *)( (uint64_t)(ret+retIndex-((DWORDtrio&(0xFFFFFF>>((DWORDtrio & 0x04)<<1)))>>3)) ), 16);
				#endif
				#ifdef _N_XMM
		SlowCopy128bit( (const char *)( (uint64_t)(ret+retIndex-((DWORDtrio&(0xFFFFFF>>((DWORDtrio & 0x04)<<1)))>>3)) ), ret+retIndex );
				#endif
		srcIndex+= (uint64_t)(3-((DWORDtrio & 0x04)>>2));
		retIndex+= (uint64_t)( Min_Match_Length>>( ((DWORDtrio & 0x04)>>2) + ((DWORDtrio & 0x02)>>1) ) );
		} else {
				#ifndef _N_XMM
		memcpy((ret+retIndex), (const char *)( (uint64_t)(src+srcIndex+1) ), 16);
				#endif
				#ifdef _N_XMM
		SlowCopy128bit( (const char *)( (uint64_t)(src+srcIndex+1+16*(0)) ), ret+retIndex );
				#endif
		srcIndex+= ((DWORDtrio & 0xFF)>>3)+1;
		retIndex+= ((DWORDtrio & 0xFF)>>3);
		}
	}        
	return retIndex;
}
*/

unsigned int KinutoraDecompress(char* ret, char* src, unsigned int srcSize){
	//unsigned int srcIndex=0; // Dummy me
	//unsigned int retIndex=0; // Dummy me
	// The muffinesque suggestion by Jim Dempsey enforced:
	char* retLOCAL = ret;
	char* srcLOCAL = src;
	char* srcEndLOCAL = src+srcSize;
	unsigned int DWORDtrio;
	unsigned int Flag;
	while(srcLOCAL < srcEndLOCAL){
		DWORDtrio = *(unsigned int*)srcLOCAL;
// |1stLSB     |2ndLSB  |3rdLSB   |
// --------------------------------
// |T|LL|O|xxxx|xxxxxxxx|xxxxxx|xx|
// --------------------------------
// [1bit           16bit]    24bit]
// T = 0 means Literal
// T = 1 means Match
// LL = 00b means Long MatchLength, 32>>LL or 32
// LL = 01b means Long MatchLength, 32>>LL or 16
// LL = 10b means Long MatchLength, 32>>LL or 8
// LL = 11b means Long MatchLength, 32>>LL or 4
// O = 0 means Long MatchOffset, 3 bytes long i.e. Sliding Window is 3*8-F-LL-O=3*8-4=20 or 1MB
// O = 1 means Short MatchOffset, 2 bytes long i.e. Sliding Window is 2*8-F-LL-O=2*8-4=12 or 4KB
		if ( (DWORDtrio & 0x01) == 0 ) { // That way is faster than 'if ( (DWORDtrio & 0x01) )'
				#ifndef _N_YMM
		memcpy(retLOCAL, (const char *)( (uint64_t)(srcLOCAL+1) ), 16);
				#endif
				#ifdef _N_YMM
		SlowCopy128bit( (const char *)( (uint64_t)(srcLOCAL+1) ), retLOCAL );
				#endif
		//srcLOCAL+= ((DWORDtrio & 0xFF)>>4)+1;
		retLOCAL+= ((DWORDtrio & 0xFF)>>4);
		srcLOCAL+= ((DWORDtrio & 0xFF)>>4)+1; // Swapping the order of above two lines led to 'LEA' vanishing!
		} else {
				#ifndef _N_YMM
		memcpy(retLOCAL, (const char *)( (uint64_t)(retLOCAL-((DWORDtrio&(0xFFFFFF>>((DWORDtrio & 0x08)<<0)))>>4)) ), 32);
				#endif
				#ifdef _N_YMM
		SlowCopy256bit( (const char *)( (uint64_t)(retLOCAL-((DWORDtrio&(0xFFFFFF>>((DWORDtrio & 0x08)<<0)))>>4)) ), retLOCAL );
				#endif
		srcLOCAL+= (uint64_t)(3-((DWORDtrio & 0x08)>>3));
		retLOCAL+= (uint64_t)( Min_Match_Length>>((DWORDtrio>>1)&0x03) );
		}
	}        
	return (unsigned int)(retLOCAL - ret);
}

/*
; 'Kinutora' decompression loop, 86-15+2=115 bytes long:
; mark_description "Intel(R) C++ Intel(R) 64 Compiler XE for applications running on Intel(R) 64, Version 12.1.1.258 Build 20111";
; mark_description "-O3 -QxSSE2 -D_N_YMM -FAcs";

.B8.3::                         
  00015 44 8b 12         mov r10d, DWORD PTR [rdx]              
  00018 41 f7 c2 01 00 
        00 00            test r10d, 1                           
  0001f 75 1c            jne .B8.5 
.B8.4::                         
  00021 45 0f b6 d2      movzx r10d, r10b                       
  00025 f3 0f 6f 42 01   movdqu xmm0, XMMWORD PTR [1+rdx]       
  0002a 41 c1 ea 04      shr r10d, 4                            
  0002e f3 0f 7f 00      movdqu XMMWORD PTR [rax], xmm0         
  00032 49 03 c2         add rax, r10                           
  00035 41 ff c2         inc r10d                               
  00038 49 03 d2         add rdx, r10                           
  0003b eb 46            jmp .B8.6 
.B8.5::                         
  0003d 44 89 d1         mov ecx, r10d                          
  00040 41 bb ff ff ff 
        00               mov r11d, 16777215                     
  00046 83 e1 08         and ecx, 8                             
  00049 41 d3 eb         shr r11d, cl                           
  0004c 45 23 da         and r11d, r10d                         
  0004f 41 c1 eb 04      shr r11d, 4                            
  00053 49 f7 db         neg r11                                
  00056 4c 03 d8         add r11, rax                           
  00059 c1 e9 03         shr ecx, 3                             
  0005c f7 d9            neg ecx                                
  0005e 41 d1 ea         shr r10d, 1                            
  00061 83 c1 03         add ecx, 3                             
  00064 c4 c1 7e 6f 03   vmovdqu ymm0, YMMWORD PTR [r11]        
  00069 41 83 e2 03      and r10d, 3                            
  0006d 48 03 d1         add rdx, rcx                           
  00070 44 89 d1         mov ecx, r10d                          
  00073 41 ba 20 00 00 
        00               mov r10d, 32                           
  00079 41 d3 ea         shr r10d, cl                           
  0007c c5 fe 7f 00      vmovdqu YMMWORD PTR [rax], ymm0        
  00080 49 03 c2         add rax, r10                           
.B8.6::                         
  00083 49 3b d0         cmp rdx, r8                            
  00086 72 8d            jb .B8.3 
*/

// With full use of the 64KB, Results_Core2_T7500.txt:
/*
Nakamichi 'Nekomata', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 433 MB/s.
Nakamichi 'Nekomata', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 435 MB/s.
Nakamichi 'Nekomata', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 433 MB/s.

Nakamichi 'Hitomi', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 435 MB/s.
Nakamichi 'Hitomi', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 433 MB/s.
Nakamichi 'Hitomi', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 52146282 bytes ...
RAM-to-RAM performance: 435 MB/s.

Nakamichi 'Kaiju', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63748036 bytes ...
RAM-to-RAM performance: 607 MB/s.
Nakamichi 'Kaiju', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63748036 bytes ...
RAM-to-RAM performance: 607 MB/s.
Nakamichi 'Kaiju', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63748036 bytes ...
RAM-to-RAM performance: 607 MB/s.

Nakamichi 'Kaidanji', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63430147 bytes ...
RAM-to-RAM performance: 676 MB/s.
Nakamichi 'Kaidanji', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63430147 bytes ...
RAM-to-RAM performance: 676 MB/s.
Nakamichi 'Kaidanji', written by Kaze, based on Nobuo Ito's LZSS source, babealicious suggestion by m^2 enforced.
Decompressing 63430147 bytes ...
RAM-to-RAM performance: 671 MB/s.

YAPPY: [b 1K] bytes 100000000 -> 73533773  73.5%  comp  41.3 MB/s  uncomp 658.4 MB/s 
YAPPY: [b 2K] bytes 100000000 -> 67516056  67.5%  comp  38.0 MB/s  uncomp 602.5 MB/s 
YAPPY: [b 4K] bytes 100000000 -> 61757720  61.8%  comp  34.4 MB/s  uncomp 547.0 MB/s 
YAPPY: [b 8K] bytes 100000000 -> 57701807  57.7%  comp  30.6 MB/s  uncomp 524.0 MB/s 
YAPPY: [b 64K] bytes 100000000 -> 54162908  54.2%  comp  28.7 MB/s  uncomp 509.1 MB/s 
YAPPY: [b 1024K] bytes 100000000 -> 53687370  53.7%  comp  28.3 MB/s  uncomp 509.5 MB/s 
*/

// Kaibutsu
// Decompression main loop:
/*
; mark_description "Intel(R) C++ Compiler XE for applications running on IA-32, Version 12.1.1.258 Build 20111011";
; mark_description "-O3 -QxSSE2 -D_N_XMM -FAcs";

.B7.3:                          
  0001e 0f b7 34 3a      movzx esi, WORD PTR [edx+edi]          
  00022 8b de            mov ebx, esi                           
  00024 81 e3 ff 00 00 
        00               and ebx, 255                           
  0002a 83 fb 10         cmp ebx, 16                            
  0002d 72 1d            jb .B7.5 
.B7.4:                          
  0002f 8b 4c 24 10      mov ecx, DWORD PTR [16+esp]            
  00033 f7 de            neg esi                                
  00035 83 c2 02         add edx, 2                             
  00038 8d 1c 01         lea ebx, DWORD PTR [ecx+eax]           
  0003b 83 c0 07         add eax, 7                             
  0003e 03 f3            add esi, ebx                           
  00040 8b 0e            mov ecx, DWORD PTR [esi]               
  00042 8b 76 04         mov esi, DWORD PTR [4+esi]             
  00045 89 0b            mov DWORD PTR [ebx], ecx               
  00047 89 73 04         mov DWORD PTR [4+ebx], esi             
  0004a eb 15            jmp .B7.6 
.B7.5:                          
  0004c f3 0f 6f 44 3a 
        01               movdqu xmm0, XMMWORD PTR [1+edx+edi]   
  00052 8b 4c 24 10      mov ecx, DWORD PTR [16+esp]            
  00056 8d 54 1a 01      lea edx, DWORD PTR [1+edx+ebx]         
  0005a f3 0f 7f 04 08   movdqu XMMWORD PTR [eax+ecx], xmm0     
  0005f 03 c3            add eax, ebx                           
.B7.6:                          
  00061 3b 54 24 18      cmp edx, DWORD PTR [24+esp]            
  00065 72 b7            jb .B7.3 
*/

// Decompression main loop:
/*
; mark_description "Intel(R) C++ Compiler XE for applications running on IA-32, Version 12.1.1.258 Build 20111011";
; mark_description "-O3 -D_N_GP -FAcs";

.B6.3:                          
  00026 8b 4c 24 14      mov ecx, DWORD PTR [20+esp]            
  0002a 0f b7 1c 2a      movzx ebx, WORD PTR [edx+ebp]          
  0002e f6 c3 07         test bl, 7                             
  00031 8d 0c 01         lea ecx, DWORD PTR [ecx+eax]           
  00034 75 45            jne .B6.5 
.B6.4:                          
  00036 8b 74 2a 01      mov esi, DWORD PTR [1+edx+ebp]         
  0003a 8b 7c 2a 05      mov edi, DWORD PTR [5+edx+ebp]         
  0003e 0f b6 db         movzx ebx, bl                          
  00041 89 31            mov DWORD PTR [ecx], esi               
  00043 89 79 04         mov DWORD PTR [4+ecx], edi             
  00046 8b 74 2a 09      mov esi, DWORD PTR [9+edx+ebp]         
  0004a 8b 7c 2a 0d      mov edi, DWORD PTR [13+edx+ebp]        
  0004e c1 eb 03         shr ebx, 3                             
  00051 89 71 08         mov DWORD PTR [8+ecx], esi             
  00054 03 c3            add eax, ebx                           
  00056 89 79 0c         mov DWORD PTR [12+ecx], edi            
  00059 8b 74 2a 11      mov esi, DWORD PTR [17+edx+ebp]        
  0005d 8b 7c 2a 15      mov edi, DWORD PTR [21+edx+ebp]        
  00061 89 71 10         mov DWORD PTR [16+ecx], esi            
  00064 89 79 14         mov DWORD PTR [20+ecx], edi            
  00067 8b 74 2a 19      mov esi, DWORD PTR [25+edx+ebp]        
  0006b 8b 7c 2a 1d      mov edi, DWORD PTR [29+edx+ebp]        
  0006f 8d 54 1a 01      lea edx, DWORD PTR [1+edx+ebx]         
  00073 89 71 18         mov DWORD PTR [24+ecx], esi            
  00076 89 79 1c         mov DWORD PTR [28+ecx], edi            
  00079 eb 14            jmp .B6.6 
.B6.5:                          
  0007b f7 db            neg ebx                                
  0007d 83 c2 02         add edx, 2                             
  00080 03 d9            add ebx, ecx                           
  00082 83 c0 08         add eax, 8                             
  00085 8b 33            mov esi, DWORD PTR [ebx]               
  00087 8b 5b 04         mov ebx, DWORD PTR [4+ebx]             
  0008a 89 31            mov DWORD PTR [ecx], esi               
  0008c 89 59 04         mov DWORD PTR [4+ecx], ebx             
.B6.6:                          
  0008f 3b 54 24 1c      cmp edx, DWORD PTR [28+esp]            
  00093 72 91            jb .B6.3 

; mark_description "Intel(R) C++ Compiler XE for applications running on IA-32, Version 12.1.1.258 Build 20111011";
; mark_description "-O3 -QxSSE2 -D_N_XMM -FAcs";

.B7.3:                          
  0001e 8b 4c 24 10      mov ecx, DWORD PTR [16+esp]            
  00022 0f b7 34 3a      movzx esi, WORD PTR [edx+edi]          
  00026 f7 c6 07 00 00 
        00               test esi, 7                            
  0002c 8d 1c 01         lea ebx, DWORD PTR [ecx+eax]           
  0002f 74 16            je .B7.5 
.B7.4:                          
  00031 f7 de            neg esi                                
  00033 83 c2 02         add edx, 2                             
  00036 03 f3            add esi, ebx                           
  00038 83 c0 08         add eax, 8                             
  0003b 8b 0e            mov ecx, DWORD PTR [esi]               
  0003d 8b 76 04         mov esi, DWORD PTR [4+esi]             
  00040 89 0b            mov DWORD PTR [ebx], ecx               
  00042 89 73 04         mov DWORD PTR [4+ebx], esi             
  00045 eb 24            jmp .B7.6 
.B7.5:                          
  00047 81 e6 ff 00 00 
        00               and esi, 255                           
  0004d c1 ee 03         shr esi, 3                             
  00050 f3 0f 6f 44 3a 
        01               movdqu xmm0, XMMWORD PTR [1+edx+edi]   
  00056 f3 0f 6f 4c 3a 
        11               movdqu xmm1, XMMWORD PTR [17+edx+edi]  
  0005c f3 0f 7f 03      movdqu XMMWORD PTR [ebx], xmm0         
  00060 f3 0f 7f 4b 10   movdqu XMMWORD PTR [16+ebx], xmm1      
  00065 03 c6            add eax, esi                           
  00067 8d 54 32 01      lea edx, DWORD PTR [1+edx+esi]         
.B7.6:                          
  0006b 3b 54 24 18      cmp edx, DWORD PTR [24+esp]            
  0006f 72 ad            jb .B7.3 
*/


// Railgun_Swampshine_BailOut, copyleft 2014-Apr-27, Kaze.
// 2014-Apr-27: The nasty SIGNED/UNSIGNED bug in 'Swampshines' which I illustrated several months ago in my fuzzy search article now is fixed here too:
/*
The bug is this (the variables 'i' and 'PRIMALposition' are uint32_t):
Next line assumes -19 >= 0 is true:
if ( (i-(PRIMALposition-1)) >= 0) printf ("THE NASTY BUG AGAIN: %d >= 0\n", i-(PRIMALposition-1));
Next line assumes -19 >= 0 is false:
if ( (signed int)(i-(PRIMALposition-1)) >= 0) printf ("THE NASTY BUG AGAIN: %d >= 0\n", i-(PRIMALposition-1));
And the actual fix:
...
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4
// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {
...
*/
// Railgun_Swampshine_BailOut, copyleft 2014-Jan-31, Kaze.
// Caution: For better speed the case 'if (cbPattern==1)' was removed, so Pattern must be longer than 1 char.
#define NeedleThreshold2vs4swampLITE 9+10 // Should be bigger than 9. BMH2 works up to this value (inclusive), if bigger then BMH4 takes over.
char * Railgun_Swampshine_BailOut (char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern)
{
	char * pbTargetMax = pbTarget + cbTarget;
	register uint32_t ulHashPattern;
	signed long count;

	unsigned char bm_Horspool_Order2[256*256]; // Bitwise soon...
	uint32_t i, Gulliver;

	uint32_t PRIMALposition, PRIMALpositionCANDIDATE;
	uint32_t PRIMALlength, PRIMALlengthCANDIDATE;
	uint32_t j, FoundAtPosition;

	if (cbPattern > cbTarget) return(NULL);

	if ( cbPattern<4 ) { 
		// SSE2 i.e. 128bit Assembly rules here:
		// ...
        	pbTarget = pbTarget+cbPattern;
		ulHashPattern = ( (*(char *)(pbPattern))<<8 ) + *(pbPattern+(cbPattern-1));
		if ( cbPattern==3 ) {
			for ( ;; ) {
				if ( ulHashPattern == ( (*(char *)(pbTarget-3))<<8 ) + *(pbTarget-1) ) {
					if ( *(char *)(pbPattern+1) == *(char *)(pbTarget-2) ) return((pbTarget-3));
				}
				if ( (char)(ulHashPattern>>8) != *(pbTarget-2) ) { 
					pbTarget++;
					if ( (char)(ulHashPattern>>8) != *(pbTarget-2) ) pbTarget++;
				}
				pbTarget++;
				if (pbTarget > pbTargetMax) return(NULL);
			}
		} else {
		}
		for ( ;; ) {
			if ( ulHashPattern == ( (*(char *)(pbTarget-2))<<8 ) + *(pbTarget-1) ) return((pbTarget-2));
			if ( (char)(ulHashPattern>>8) != *(pbTarget-1) ) pbTarget++;
			pbTarget++;
			if (pbTarget > pbTargetMax) return(NULL);
		}
	} else { //if ( cbPattern<4 )
		if ( cbPattern<=NeedleThreshold2vs4swampLITE ) { 
			// BMH order 2, needle should be >=4:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1; // 'Gulliver' is the skip
				if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]] != 0 ) {
					if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1-2]] == 0 ) Gulliver = cbPattern-(2-1)-2; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
							count = cbPattern-4+1; 
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
							if ( count <= 0 ) return(pbTarget+i);
						}
					}
				} else Gulliver = cbPattern-(2-1);
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);
		} else { // if ( cbPattern<=NeedleThreshold2vs4swampLITE )

// Swampwalker_BAILOUT heuristic order 4 (Needle should be bigger than 4) [
// Needle: 1234567890qwertyuiopasdfghjklzxcv            PRIMALposition=01 PRIMALlength=33  '1234567890qwertyuiopasdfghjklzxcv'
// Needle: vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv            PRIMALposition=29 PRIMALlength=04  'vvvv'
// Needle: vvvvvvvvvvBOOMSHAKALAKAvvvvvvvvvv            PRIMALposition=08 PRIMALlength=20  'vvvBOOMSHAKALAKAvvvv'
// Needle: Trollland                                    PRIMALposition=01 PRIMALlength=09  'Trollland'
// Needle: Swampwalker                                  PRIMALposition=01 PRIMALlength=11  'Swampwalker'
// Needle: licenselessness                              PRIMALposition=01 PRIMALlength=15  'licenselessness'
// Needle: alfalfa                                      PRIMALposition=02 PRIMALlength=06  'lfalfa'
// Needle: Sandokan                                     PRIMALposition=01 PRIMALlength=08  'Sandokan'
// Needle: shazamish                                    PRIMALposition=01 PRIMALlength=09  'shazamish'
// Needle: Simplicius Simplicissimus                    PRIMALposition=06 PRIMALlength=20  'icius Simplicissimus'
// Needle: domilliaquadringenquattuorquinquagintillion  PRIMALposition=01 PRIMALlength=32  'domilliaquadringenquattuorquinqu'
// Needle: boom-boom                                    PRIMALposition=02 PRIMALlength=08  'oom-boom'
// Needle: vvvvv                                        PRIMALposition=01 PRIMALlength=04  'vvvv'
// Needle: 12345                                        PRIMALposition=01 PRIMALlength=05  '12345'
// Needle: likey-likey                                  PRIMALposition=03 PRIMALlength=09  'key-likey'
// Needle: BOOOOOM                                      PRIMALposition=03 PRIMALlength=05  'OOOOM'
// Needle: aaaaaBOOOOOM                                 PRIMALposition=02 PRIMALlength=09  'aaaaBOOOO'
// Needle: BOOOOOMaaaaa                                 PRIMALposition=03 PRIMALlength=09  'OOOOMaaaa'
PRIMALlength=0;
for (i=0+(1); i < cbPattern-((4)-1)+(1)-(1); i++) { // -(1) because the last BB order 4 has no counterpart(s)
	FoundAtPosition = cbPattern - ((4)-1) + 1;
	PRIMALpositionCANDIDATE=i;
	while ( PRIMALpositionCANDIDATE <= (FoundAtPosition-1) ) {
		j = PRIMALpositionCANDIDATE + 1;
		while ( j <= (FoundAtPosition-1) ) {
			if ( *(uint32_t *)(pbPattern+PRIMALpositionCANDIDATE-(1)) == *(uint32_t *)(pbPattern+j-(1)) ) FoundAtPosition = j;
			j++;
		}
		PRIMALpositionCANDIDATE++;
	}
	PRIMALlengthCANDIDATE = (FoundAtPosition-1)-i+1 +((4)-1);
	if (PRIMALlengthCANDIDATE >= PRIMALlength) {PRIMALposition=i; PRIMALlength = PRIMALlengthCANDIDATE;}
	if (cbPattern-i+1 <= PRIMALlength) break;
	if (PRIMALlength > 128) break; // Bail Out for 129[+]
}
// Swampwalker_BAILOUT heuristic order 4 (Needle should be bigger than 4) ]

// Here we have 4 or bigger NewNeedle, apply order 2 for pbPattern[i+(PRIMALposition-1)] with length 'PRIMALlength' and compare the pbPattern[i] with length 'cbPattern':
PRIMALlengthCANDIDATE = cbPattern;
cbPattern = PRIMALlength;
pbPattern = pbPattern + (PRIMALposition-1);

// Revision 2 commented section [
/*
if (cbPattern-1 <= 255) {
// BMH Order 2 [
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]= cbPattern-1;} // cbPattern-(Order-1) for Horspool; 'memset' if not optimized
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=i; // Rightmost appearance/position is needed
			i=0;
			while (i <= cbTarget-cbPattern) { 
				Gulliver = bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]];
				if ( Gulliver != cbPattern-1 ) { // CASE #2: if equal means the pair (char order 2) is not found i.e. Gulliver remains intact, skip the whole pattern and fall back (Order-1) chars i.e. one char for Order 2
				if ( Gulliver == cbPattern-2 ) { // CASE #1: means the pair (char order 2) is found
					if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) {
						count = cbPattern-4+1; 
						while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
							count = count-4;
// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4

// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1; 
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));	
		}
	}
}
					}
					Gulliver = 1;
				} else
					Gulliver = cbPattern - Gulliver - 2; // CASE #3: the pair is found and not as suffix i.e. rightmost position
				}
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);
// BMH Order 2 ]
} else {
			// BMH order 2, needle should be >=4:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1; // 'Gulliver' is the skip
				if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]] != 0 ) {
					if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1-2]] == 0 ) Gulliver = cbPattern-(2-1)-2; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
							count = cbPattern-4+1; 
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4

// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1; 
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));	
		}
	}
}
						}
					}
				} else Gulliver = cbPattern-(2-1);
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);
}
*/
// Revision 2 commented section ]

		if ( cbPattern<=NeedleThreshold2vs4swampLITE ) { 

			// BMH order 2, needle should be >=4:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1; // 'Gulliver' is the skip
				if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]] != 0 ) {
					if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1-2]] == 0 ) Gulliver = cbPattern-(2-1)-2; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
							count = cbPattern-4+1; 
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4

// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1; 
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));	
		}
	}
}
						}
					}
				} else Gulliver = cbPattern-(2-1);
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);

		} else { // if ( cbPattern<=NeedleThreshold2vs4swampLITE )

			// BMH pseudo-order 4, needle should be >=8+2:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			// In line below we "hash" 4bytes to 2bytes i.e. 16bit table, how to compute TOTAL number of BBs, 'cbPattern - Order + 1' is the number of BBs for text 'cbPattern' bytes long, for example, for cbPattern=11 'fastest fox' and Order=4 we have BBs = 11-4+1=8:
			//"fast"
			//"aste"
			//"stes"
			//"test"
			//"est "
			//"st f"
			//"t fo"
			//" fox"
			//for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( *(unsigned short *)(pbPattern+i+0) + *(unsigned short *)(pbPattern+i+2) ) & ( (1<<16)-1 )]=1;
			//for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( (*(uint32_t *)(pbPattern+i+0)>>16)+(*(uint32_t *)(pbPattern+i+0)&0xFFFF) ) & ( (1<<16)-1 )]=1;
			// Above line is replaced by next one with better hashing:
			for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( (*(uint32_t *)(pbPattern+i+0)>>(16-1))+(*(uint32_t *)(pbPattern+i+0)&0xFFFF) ) & ( (1<<16)-1 )]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1;
				//if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]&0xFFFF) ) & ( (1<<16)-1 )] != 0 ) { // DWORD #1
				// Above line is replaced by next one with better hashing:
				if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]&0xFFFF) ) & ( (1<<16)-1 )] != 0 ) { // DWORD #1
					//if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] == 0 ) Gulliver = cbPattern-(2-1)-2-4; else {
					// Above line is replaced in order to strengthen the skip by checking the middle DWORD,if the two DWORDs are 'ab' and 'cd' i.e. [2x][2a][2b][2c][2d] then the middle DWORD is 'bc'.
					// The respective offsets (backwards) are: -10/-8/-6/-4 for 'xa'/'ab'/'bc'/'cd'.
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-4-2; else {
					// Above line is replaced by next one with better hashing:
					// When using (16-1) right shifting instead of 16 we will have two different pairs (if they are equal), the highest bit being lost do the job especialy for ASCII texts with no symbols in range 128-255.
					// Example for genomesque pair TT+TT being shifted by (16-1):
					// T            = 01010100
					// TT           = 01010100 01010100
					// TTTT         = 01010100 01010100 01010100 01010100
					// TTTT>>16     = 00000000 00000000 01010100 01010100
					// TTTT>>(16-1) = 00000000 00000000 10101000 10101000 <--- Due to the left shift by 1, the 8th bits of 1st and 2nd bytes are populated - usually they are 0 for English texts & 'ACGT' data.
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-4-2; else {
					// 'Maximus' uses branched 'if', again.
					if ( \
					( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6 +1]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6 +1]&0xFFFF) ) & ( (1<<16)-1 )] ) == 0 \
					|| ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4 +1]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4 +1]&0xFFFF) ) & ( (1<<16)-1 )] ) == 0 \
					) Gulliver = cbPattern-(2-1)-2-4-2 +1; else {
					// Above line is not optimized (several a SHR are used), we have 5 non-overlapping WORDs, or 3 overlapping WORDs, within 4 overlapping DWORDs so:
// [2x][2a][2b][2c][2d]
// DWORD #4
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>16) =     !SHR to be avoided! <--
// [2x] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) =                        |
//     DWORD #3                                                                       |
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16) =     !SHR to be avoided!   |<--
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) = ------------------------  |
//         DWORD #2                                                                      |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>16) =     !SHR to be avoided!      |<--
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) = ---------------------------  |
//             DWORD #1                                                                     |
// [2d] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]>>16) =                                 |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) = ------------------------------
//
// So in order to remove 3 SHR instructions the equal extractions are:
// DWORD #4
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) =  !SHR to be avoided! <--
// [2x] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) =                        |
//     DWORD #3                                                                       |
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) =  !SHR to be avoided!   |<--
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) = ------------------------  |
//         DWORD #2                                                                      |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) =  !SHR to be avoided!      |<--
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) = ---------------------------  |
//             DWORD #1                                                                     |
// [2d] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]>>16) =                                 |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) = ------------------------------
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-6; else {
// Since the above Decumanus mumbo-jumbo (3 overlapping lookups vs 2 non-overlapping lookups) is not fast enough we go DuoDecumanus or 3x4:
// [2y][2x][2a][2b][2c][2d]
// DWORD #3
//         DWORD #2
//                 DWORD #1
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-8]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-8]&0xFFFF) ) & ( (1<<16)-1 )] ) < 2 ) Gulliver = cbPattern-(2-1)-2-8; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) {
							// Order 4 [
						// Let's try something "outrageous" like comparing with[out] overlap BBs 4bytes long instead of 1 byte back-to-back:
						// Inhere we are using order 4, 'cbPattern - Order + 1' is the number of BBs for text 'cbPattern' bytes long, for example, for cbPattern=11 'fastest fox' and Order=4 we have BBs = 11-4+1=8:
						//0:"fast" if the comparison failed here, 'count' is 1; 'Gulliver' is cbPattern-(4-1)-7
						//1:"aste" if the comparison failed here, 'count' is 2; 'Gulliver' is cbPattern-(4-1)-6
						//2:"stes" if the comparison failed here, 'count' is 3; 'Gulliver' is cbPattern-(4-1)-5
						//3:"test" if the comparison failed here, 'count' is 4; 'Gulliver' is cbPattern-(4-1)-4
						//4:"est " if the comparison failed here, 'count' is 5; 'Gulliver' is cbPattern-(4-1)-3
						//5:"st f" if the comparison failed here, 'count' is 6; 'Gulliver' is cbPattern-(4-1)-2
						//6:"t fo" if the comparison failed here, 'count' is 7; 'Gulliver' is cbPattern-(4-1)-1
						//7:" fox" if the comparison failed here, 'count' is 8; 'Gulliver' is cbPattern-(4-1)
							count = cbPattern-4+1; 
							// Below comparison is UNIdirectional:
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
// count = cbPattern-4+1 = 23-4+1 = 20
// boomshakalakaZZZZZZ[ZZZZ] 20
// boomshakalakaZZ[ZZZZ]ZZZZ 20-4
// boomshakala[kaZZ]ZZZZZZZZ 20-8 = 12
// boomsha[kala]kaZZZZZZZZZZ 20-12 = 8
// boo[msha]kalakaZZZZZZZZZZ 20-16 = 4

// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4

// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1; 
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));	
		}
	}
}

							// In order to avoid only-left or only-right WCS the memcmp should be done as left-to-right and right-to-left AT THE SAME TIME.
							// Below comparison is BIdirectional. It pays off when needle is 8+++ long:
//							for (count = cbPattern-4+1; count > 0; count = count-4) {
//								if ( *(uint32_t *)(pbPattern+count-1) != *(uint32_t *)(&pbTarget[i]+(count-1)) ) {break;};
//								if ( *(uint32_t *)(pbPattern+(cbPattern-4+1)-count) != *(uint32_t *)(&pbTarget[i]+(cbPattern-4+1)-count) ) {count = (cbPattern-4+1)-count +(1); break;} // +(1) because two lookups are implemented as one, also no danger of 'count' being 0 because of the fast check outwith the 'while': if ( *(uint32_t *)&pbTarget[i] == ulHashPattern)
//							}
//							if ( count <= 0 ) return(pbTarget+i);
								// Checking the order 2 pairs in mismatched DWORD, all the 3:
								//if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1]] == 0 ) Gulliver = count; // 1 or bigger, as it should
								//if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1+1]] == 0 ) Gulliver = count+1; // 1 or bigger, as it should
								//if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1+1+1]] == 0 ) Gulliver = count+1+1; // 1 or bigger, as it should
							//	if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1]] + bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1+1]] + bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+count-1+1+1]] < 3 ) Gulliver = count; // 1 or bigger, as it should, THE MIN(count,count+1,count+1+1)
								// Above compound 'if' guarantees not that Gulliver > 1, an example:
								// Needle:    fastest tax
								// Window: ...fastast tax...
								// After matching ' tax' vs ' tax' and 'fast' vs 'fast' the mismathced DWORD is 'test' vs 'tast':
								// 'tast' when factorized down to order 2 yields: 'ta','as','st' - all the three when summed give 1+1+1=3 i.e. Gulliver remains 1.
								// Roughly speaking, this attempt maybe has its place in worst-case scenarios but not in English text and even not in ACGT data, that's why I commented it in original 'Shockeroo'.
								//if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+count-1]>>16)+(*(uint32_t *)&pbTarget[i+count-1]&0xFFFF) ) & ( (1<<16)-1 )] == 0 ) Gulliver = count; // 1 or bigger, as it should
								// Above line is replaced by next one with better hashing:
//								if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+count-1]>>(16-1))+(*(uint32_t *)&pbTarget[i+count-1]&0xFFFF) ) & ( (1<<16)-1 )] == 0 ) Gulliver = count; // 1 or bigger, as it should
							// Order 4 ]
						}
					}
				} else Gulliver = cbPattern-(2-1)-2; // -2 because we check the 4 rightmost bytes not 2.
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);

		} // if ( cbPattern<=NeedleThreshold2vs4swampLITE )
		} // if ( cbPattern<=NeedleThreshold2vs4swampLITE )
	} //if ( cbPattern<4 )
}

// Fixed version from 2012-Feb-27.
// Caution: For better speed the case 'if (cbPattern==1)' was removed, so Pattern must be longer than 1 char.
char * Railgun_Doublet (char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern)
{
	char * pbTargetMax = pbTarget + cbTarget;
	register uint32_t ulHashPattern;
	uint32_t ulHashTarget, count, countSTATIC;

	if (cbPattern > cbTarget) return(NULL);

	countSTATIC = cbPattern-2;

	pbTarget = pbTarget+cbPattern;
	ulHashPattern = (*(uint16_t *)(pbPattern));

	for ( ;; ) {
		if ( ulHashPattern == (*(uint16_t *)(pbTarget-cbPattern)) ) {
			count = countSTATIC;
			while ( count && *(char *)(pbPattern+2+(countSTATIC-count)) == *(char *)(pbTarget-cbPattern+2+(countSTATIC-count)) ) {
				count--;
			}
			if ( count == 0 ) return((pbTarget-cbPattern));
		}
		pbTarget++;
		if (pbTarget > pbTargetMax) return(NULL);
	}
}

// Last change: 2014-Sep-07
// If you want to help me to improve it, email me at: sanmayce@sanmayce.com
// Enfun!
