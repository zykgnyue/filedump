// Report
//
//

#include <stdio.h>
//functions prototype declearation
void checkShift();
void inplaceReverseArray();
void reverse_array(int a[], size_t cnt);
void inpswap(int*x,int*y);

int gVar;

int main()
{
        int x1=23,y1=0xAA;

        //pinter test
        int B=-12345;
        int *p;
        int i;
        p=&B; //B的存储地址给p
        *p= 56789;
        char *pc;
        pc =(char*)&B;

        //check pointer addressed value
        printf("%d = %8.8x\n",p[0],p[0]);
        printf("%d = %x\n",(char)pc[0],pc[0]&0xFF);
        
        //check local address & endian
        for(i=0;i<sizeof(B);i++)
        {
               printf("localVar @ %p:%2.2x\n",pc+i,(unsigned char)*(pc+i));
        }
	
        //check global address & endian
        pc=(char *)&gVar;
        gVar=0x12345678;
        for(i=0;i<sizeof(B);i++)
        {
               printf("gVar @ %p:%2.2x\n",pc+i,(unsigned char)*(pc+i));
        }
		
	//check code address
        printf("main @ %p\n",main);

	//check lib code address
        printf("printf @ %p\n",printf);
      	
      	
      	//check inplace swap
        x1=0x23;
        y1=0xAA;
        printf("before swap: %x,%x\n",x1,y1);
        inpswap(&x1,&y1);
        printf("after swap: %x,%x\n",x1,y1);
        printf("%d,%d\n",x1,y1);

        //check Logical/arithmetic shift
      	checkShift();

     	//use inplace swap reverse array element
     	inplaceReverseArray();

}


//check shfit bits>=variable size
//learn shift 
void checkShift()
{
	unsigned uBig=0x00123456;
	int varBig=(0x00123456 >>32);
	
        //compiler shfit 0
	//signed shift constant  by compiler
	varBig=(0x87123456 >>32);
	//code4output varBig result
	printf("signed shfitRight32 %d,0x%8.8x\n",varBig,varBig);
	
	varBig=(0x87123456 >>40);
	//code4output varBig result
	printf("signed shfitRight40 %d,0x%8.8x\n",varBig,varBig);
	

        //compiler shfit 0; 32 Logic mod32=0
	varBig=(0x87123456 <<32);
	//code4output varBig result
	printf("signed shfitLeft32 %d, 0x%8.8x\n",varBig,varBig);

        //logical 40 mod 32 =8
	varBig=(0x87123456 <<40);
	//cod4output varBig result
	printf("signed shfitLeft40 %d, 0x%8.8x\n",varBig,varBig);


        //CPU shift 0
	varBig=(0x87123456);
	varBig <<=32;
	//code4output varBig result
        printf("CPU:signed shfitLeft32 %d, 0x%8.8x\n",varBig,varBig);

        //varable shfitRight40mod32=8 by CPU
	varBig=(0x87123456);
	varBig <<=40;
	//code4output varBig result
        printf("CPU:signed shfitLeft40 %d, x%8.8x\n",varBig,varBig);


	varBig=(0x87123456);
	varBig >>=32;
	//code4output varBig result
        printf("CPU:signed shfitRight32 %d, 0x%8.8x\n",varBig,varBig);


	varBig=(0x87123456);
	varBig >>=40;
	//code4output varBig result
        printf("CPU:signed shfitRight40 %d, 0x%8.8x\n",varBig,varBig);

	
	//unsigned shift by variable
        uBig=0x87654321;
	uBig <<=40;
	//code4output uBig result
        printf("CPU:uint shfitLeft40 %u, 0x%8.8x\n",uBig,uBig);

        uBig=0x87654321;
	uBig <<=32;
	//code output uBig result
        printf("CPU:uint shfitLeft32 %u, 0x%8.8x\n",uBig,uBig);

	//unsigned shift by variable
        uBig=0x87654321;
	uBig >>=40;
	//code4output uBig result
        printf("CPU:uint shfitRight40 %u, 0x%8.8x\n",uBig,uBig);

        uBig=0x87654321;
	uBig >>=32;
	//code output uBig result
        printf("CPU:uint shfitRight32 %u, 0x%8.8x\n",uBig,uBig);

        uBig=0x87654321U>>32;
	//code output uBig result
        printf("Const:uint shfitRight32 %u, 0x%8.8x\n",uBig,uBig);

        uBig=0x87654321U>>40;
	//code output uBig result
        printf("Const:uint shfitRight40 %u, 0x%8.8x\n",uBig,uBig);

}

//you can not use other array as intermidiate buffer
//参考 中文ebook  p38练习题2.11，完善代码，找出BUG
//ref  Englisg Book p91 Practice Problem 2.11 complete the code & find bugs
void inplaceReverseArray()
{
        int table1[]={1,2,3,4,5};
        int table2[]={1,2,3,4};

        //output table1 contents

        reverse_array(table1,sizeof(table1)/sizeof(int));

        //output reversed table1 contents



        //output table2 contents

        reverse_array(table2,sizeof(table2)/sizeof(int));

        //output reversed table2 contents

}


//reverseArray without buffer
//
void reverse_array(int a[], size_t cnt)
{

        size_t first, last;

        printf("Before swap:a[%lu]=[",cnt);
        for(first =0;first<cnt ;first++)
        {
                printf("%d, ",a[first]);
        }
        printf("]\n");
#ifdef BUG_REVERSE
        for (first = 0, last = cnt-1; first <= last; first++,last--)
#else
        for (first = 0, last = cnt-1; first < last; first++,last--)
#endif        
        {
                inpswap(&a[first], &a[last]);
        }

        printf("After swap:a[%lu]=[",cnt);

        for(first =0;first<cnt ;first++)
        {
                printf("%d, ",a[first]);
        }
        printf("]\n");
}


//swap without buffer
void inpswap(int*x,int*y)
{
        *x=*x^*y;
        *y=*x^*y;
        *x=*x^*y;
}

