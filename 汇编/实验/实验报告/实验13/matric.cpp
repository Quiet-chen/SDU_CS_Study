#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string>
#include<iostream>
#include<fstream>
#include<iosfwd>
#include<bits/stdc++.h>
using namespace std;

string prepath = "DATA/1/";
string laterpath[16] = {"A1.txt","A2.txt","A3.txt","A4.txt","A5.txt","A6.txt","A7.txt","A8.txt",
							 "A9.txt","A10.txt","A11.txt","A12.txt","A13.txt","A14.txt","A15.txt","A16.txt"};
string Bpath = "B.txt";
string Answerpath = "answer.txt";

uint32_t MatricA[16][1024][1024];
uint32_t MatricB[1024][1024];
uint32_t MatricC[16][1024][1024];
uint64_t checksum = 0;
uint64_t givensum;
const int Arraysize = 1024;
string Atotpath[16];
string Btotpath;
string Ctotpath;



#define MatricAdd(A,B)\
do \
{ \
	for(int i = 0; i<16;i++){\
		for(int j = 0; j<Arraysize;j++){\
			for(int k = 0; k<Arraysize;k++){\
				B+=A[i][j][k];\
			}\
		}\
	}\
} \
while(0)


uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
} 
void init(){
	cout<<prepath<<endl;
	for(int i = 0; i < 16;i++){
		Atotpath[i]+=prepath;
		Atotpath[i]+=laterpath[i];
	}
	Btotpath+=prepath;
	Btotpath+=Bpath;
	Ctotpath+=prepath;
	Ctotpath+=Answerpath; 
	freopen(Ctotpath.c_str(),"r",stdin);
	cin>>givensum;
	cout<<"The given answer is "<<givensum<<endl;
	for(int i = 0; i < 16;i++){
		ifstream fin(Atotpath[i].c_str());
		for(int j = 0; j<1024;j++){
			for(int k = 0; k<1024;k++){
				fin>>MatricA[i][j][k];			
			}
		}
	}
		ifstream finp(Btotpath.c_str()); 
	for(int i =0;i<1024;i++){
		for(int j = 0;j<1024;j++){
			finp>>MatricB[i][j];
		}
	}
 	
}

void __attribute__((optimize("00")))
raw_calc_naive(void) {
	for(int i = 0; i<16;i++){
	
		for(int k = 0; k<Arraysize;k++){
			for(int j = 0; j<Arraysize;j++){
				for(int t = 0; t<Arraysize;t++){
				MatricC[i][k][j]+=MatricA[i][k][t]*MatricB[t][j];
				}
			}
		}
   }
}

void __attribute__((optimize("02")))
raw_calc_expert(void) {
	for(int i = 0; i<16;i++){
		
		for(int k = 0; k<Arraysize;k++){
			for(int j = 0; j<Arraysize;j++){
				for(int t = 0; t<Arraysize;t++){
				MatricC[i][k][j]+=MatricA[i][k][t]*MatricB[t][j];
				}
			}
		}
   }
}

void __attribute__((optimize("03")))
raw_calc_sse(void) {
for(int i = 0; i<16;i++){
	
		for(int k = 0; k<Arraysize;k++){
			for(int j = 0; j<Arraysize;j++){
				for(int t = 0; t<Arraysize;t++){
				MatricC[i][k][j]+=MatricA[i][k][t]*MatricB[t][j];
				}
			}
		}
   }
}

void __attribute__((optimize("03"), 
__target__("arch=core-avx2")))
raw_calc_avx_auto(void) {
	for(int i = 0; i<16;i++){
		for(int k = 0; k<Arraysize;k++){
			for(int j = 0; j<Arraysize;j++){
				for(int t = 0; t<Arraysize;t++){
				MatricC[i][k][j]+=MatricA[i][k][t]*MatricB[t][j];
				}
			}
		}
   }
}

void __attribute__((optimize("03"), 
__target__("arch=core-avx2")))
raw_calc_avx_manual(void) {
	const uint32_t arraysize = 0x400000;
	const uint32_t innerlp = 0x1000;
	const uint64_t outlp = 16;
	__asm__ __volatile__ (
		"lea 			%[MatricA], 				%%rbx\n"
		"lea 			%[MatricB], 				%%r11\n"
		"lea 			%[MatricC], 				%%rdi\n"
		
		"xor 			%%r12, 				%%r12\n"
		"1:\n"
		"xor 			%%r14, 				%%r14\n"
		
		"2:\n"
		"xor 			%%r13, 				%%r13\n"
		"mov 			%%r11,				%%rsi\n"
		"xor			%%r10,				%%r10\n"
		"3:\n"	
		"mov 			$0x20, 				%%eax\n"
		"mov 			$0x40, 				%%edx\n"
		"mov 			$0x60,				%%ebp\n"
		"xor 			%%ecx, 				%%ecx\n"
		"add   			$0x1000,			%%r10\n" 
		"vbroadcastss 	(%%rbx),			%%ymm12\n"
	
		"4:\n"
		"vmovdqu 		(%%rsi,%%rcx,1), 	%%ymm0\n"
		"vpmulld 		%%ymm12,			%%ymm0,				%%ymm4\n"
		"vmovdqu 		(%%rsi,%%rax,1),	%%ymm1\n"
		"vpmulld 		%%ymm12,			%%ymm1,				%%ymm5\n"
		"vmovdqu 		(%%rsi,%%rdx,1), 	%%ymm2\n"
		"vpmulld 		%%ymm12,			%%ymm2, 			%%ymm6\n"
		"vmovdqu 		(%%rsi,%%rbp,1),	%%ymm3\n"
		"vpmulld 		%%ymm12,			%%ymm3, 			%%ymm7\n"					
		"vmovdqu 		(%%rdi,%%rcx,1), 	%%ymm8\n"
		"vpaddd 		%%ymm8, 			%%ymm4, 			%%ymm8\n"
		"vmovdqu 		(%%rdi,%%rax,1), 	%%ymm9\n"
		"vpaddd 		%%ymm9, 			%%ymm5, 			%%ymm9\n"
		"vmovdqu 		(%%rdi,%%rdx,1), 	%%ymm10\n"
		"vpaddd 		%%ymm10, 			%%ymm6, 			%%ymm10\n"
		"vmovdqu 		(%%rdi,%%rbp,1), 	%%ymm11\n"
		"vpaddd 		%%ymm11, 			%%ymm7, 			%%ymm11\n"
		"vmovdqu 		%%ymm8,				(%%rdi,%%rcx,1)\n"
		"vmovdqu 		%%ymm9,				(%%rdi,%%rax,1)\n"
		"vmovdqu 		%%ymm10,			(%%rdi,%%rdx,1)\n"
		"vmovdqu 		%%ymm11,			(%%rdi,%%rbp,1)\n"			
		"add 			$0x80, 				%%eax\n"
		"add 			$0x80, 				%%ebp\n"
		"add 			$0x80, 				%%ecx\n"
		"add 			$0x80, 				%%edx\n"
		"cmp 			%%ecx, 				%[innerlp]\n"
		"jnz 			4b\n"		
		"add 			$0x1000, 			%%r13d\n"
		"add 			$0x1000,			%%esi\n"
		"inc			%%ebx\n"
		"inc			%%ebx\n"
		"inc			%%ebx\n"
		"inc			%%ebx\n"
		"add			$0x1000,			%%r10\n"
		"cmp 			%%r13d, 			%[arraysize]\n"
		"jnz 			3b\n"		
		"add 			$0x1000,			%%edi\n"
		"add  			$0x1000, 			%%r14d\n"
		"cmp 			%%r14d, 			%[arraysize]\n"
		"jnz 			2b\n"		
		"inc			%%r12\n"
		"cmp 			%%r12, 				%[outlp]\n"
		"jnz 			1b\n"
		:[MatricC]"=m"(MatricC)
		:[MatricA]"m"(MatricA),[MatricB]"m"(MatricB),[arraysize]"r"(arraysize),
		[innerlp]"r"(innerlp),[outlp]"r"(outlp)
		:"rax","rbx","rcx","rdx","rbp","rsi","rdi","r10","r11","r12","r13","r14","memory"
	);
}
void test(char* opt, void (*func)(void)) {
	
	uint64_t start;
	uint64_t end;
	checksum = 0;
	memset(MatricC,0,sizeof(MatricC));

	start = rdtsc();
	func();
	end = rdtsc();

	MatricAdd(MatricC,checksum);
	
	printf("%10s %20llu - %llu \tcycles\n",opt,checksum,end-start);
}
int main(){
	init();
	test("naive", raw_calc_naive);
	test("expert", raw_calc_expert);
	test("sse", raw_calc_sse);
	test("avx-auto", raw_calc_avx_auto);
	test("avx-manual", raw_calc_avx_manual);
	return 0;
}
