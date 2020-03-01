#include <stdio.h>
#include <inttypes.h>
int main(){
	unsigned uBig=0x00123456;
	int varBig;
	int64_t  varL=(int64_t)0x8765432101234567ULL>>63;
        printf("long type size=%d bytes\n",sizeof(long)); 
        
        printf("(int64_t)0x8765432101234567ULL>>63: %lld,0x%llx\n",varL,varL);
        
	printf("direct 0x8765432101234567LL>>4: %lld,0x%llx\n",0x8765432101234567LL>>4,0x8765432101234567LL>>4);
	printf("direct 0x8765432101234567ULL>>4: %lld,0x%llx\n",0x8765432101234567ULL>>4,0x8765432101234567ULL>>4);

	printf("direct 0x8765432101234567>>4: %lld,0x%llx\n",0x8765432101234567>>4,0x8765432101234567>>4);
	//compiler shfit 0
	//signed shift constant  by compiler
	varBig=(0x87123456 >>32); 
	//code4output varBig result
	printf("0x87123456 >>32 %d,0x%8.8x\n",varBig,varBig);

	varBig=(0x87123456 >>4); 
	//code4output varBig result
	printf("0x87123456 >>4 %d,0x%8.8x\n",varBig,varBig);

	varBig=(0x87123456L >>4); 
	//code4output varBig result
	printf("0x87123456L >>4 %d,0x%8.8x\n",varBig,varBig);


	varBig=((signed int)0x87123456 >>4); 
	//code4output varBig result
	printf("(signed int)0x87123456 >>4 %d,0x%8.8x\n",varBig,varBig);

	varBig=(-0x00123456 >>8); 
	//code4output varBig result
	printf("-0x123456 >>8 %d,0x%8.8x\n",varBig,varBig);




        varBig=0x87123456;
        varBig >>=4;
        printf("signed >>4 %d,0x%8.8x\n",varBig,varBig);

        varBig >>=40; //sarl	%cl, -4(%rbp)
        printf("signed >>40 %d,0x%8.8x\n",varBig,varBig);

        uBig=0x87123456U;
        uBig >>=4;    //shrl	$4, -8(%rbp)
        printf("unsigned >>4 %u,0x%8.8x\n",uBig,uBig);


}
