//2.46 2.51
#include <stdio.h>
#include <stdint.h>
#include <math.h>

void printfloat(float x1);
void printdouble(double x1);

void printbin(unsigned char *buf,size_t maxlen);
void printbyte(unsigned char *p, size_t len);
void printbyte_reverse(unsigned char *p, size_t len);
void  print_ulongbin(uint64_t data1,size_t ulen);
double printfpow2(double dat,  int Ecode1 );
void missile();
void missile_st();

//use struct to get float/double element
#define DF_FLOAT_FRAC_BITS  (23U)
#define DF_FLOAT_EXP_BITS   (8U)
#define DF_DOUBLE_FRAC_BITS (52U)
#define DF_DOUBLE_EXP_BITS   (11U)
#define DF_SIGN_BIT   (1U)
#define DF_BIAS_FLOAT   ((1UL<<(DF_FLOAT_EXP_BITS-1))-1UL)
#define DF_BIAS_DOUBLE   ((1UL<<(DF_DOUBLE_EXP_BITS-1))-1UL)

typedef struct{
    uint64_t frac:DF_DOUBLE_FRAC_BITS;
    uint64_t exp:DF_DOUBLE_EXP_BITS;
    uint64_t s:DF_SIGN_BIT;

} stdouble;

typedef union{
    double dat;
    stdouble u64;
} uidouble;

typedef struct{
    uint32_t frac:DF_FLOAT_FRAC_BITS;
    uint32_t exp:DF_FLOAT_EXP_BITS;
    uint32_t s:DF_SIGN_BIT;

} stfloat;

typedef union{
    float dat;
    stfloat u32;
} uifloat;



int main()
{
    printf("=====long type byte size=%u =====\n",sizeof(long));
    printf("=======patriot missile 0.1s======\n");
    printf("=========Using mask==============\n");
    missile();
    printf("\n=========Using struct===========\n");
    //this method is depend on compiler may have compatible problem
    printf("Using struct & union to access float &double elements.\n");
    missile_st();
    return 0;

}

void missile_st()
{
    uifloat uf01;//向偶取整
    uifloat uf00; //向零取整
    uidouble ud01;
    //uint64_t Bias=DF_BIAS_FLOAT;
    
    ud01.dat=(double)1.0/10.0; //accurate 
    uf01.dat=1.0f/10.0f;
    ///uf01.u32.frac=1;//-1.0f/10.0f;
    ///uf01.u32.exp=0;//-1.0f/10.0f;
    uf00.dat=uf01.dat;
    uf00.u32.frac &=(~0x01UL);//mask out bit0
    printf("float2double 0.1 toward even\n");
    printf("float 0.1 toward even: %.23f\n",uf01.dat);
    printbyte_reverse((unsigned char *)&uf01.u32,sizeof(uf01.u32));
    printf("\nsign=%c, exp=0x%.2x, frac=%.8x\n",uf01.u32.s!=0?'-':'+',uf01.u32.exp,uf01.u32.frac);
    //print bin
    printf("%c",uf01.u32.s!=0?'-':'+');
    if(uf01.u32.exp!=0)
    {
        if(uf01.u32.exp >= DF_BIAS_FLOAT)
        {
            printf("(2^%lu)*",uf01.u32.exp-DF_BIAS_FLOAT);
        }else{
            printf("(2^-%lu)*",DF_BIAS_FLOAT-uf01.u32.exp);
        }
        printf("1.");
    }else{
        printf("(2^-%lu)*",+DF_BIAS_FLOAT-1);
        printf("0.");
    }
    print_ulongbin(uf01.u32.frac,DF_FLOAT_FRAC_BITS);
    //("\n%c-%.2x-%.8x\n",uf01.u32.s!=0?'-':'+',uf01.u32.exp,uf01.u32.frac);

    //double
    printf("\n\ndouble 0.1 toward even\n");
    printf("double 0.1 toward even: %.23f\n",ud01.dat);
    printbyte_reverse((unsigned char *)&ud01.u64,sizeof(ud01.u64));
    printf("\nsign=%c, exp=0x%.2x, frac=%lx\n",ud01.u64.s!=0?'-':'+',ud01.u64.exp,(uint64_t)ud01.u64.frac);
    //print bin
    printf("%c",ud01.u64.s!=0?'-':'+');
    if(ud01.u64.exp!=0)
    {
        if(ud01.u64.exp >= DF_BIAS_DOUBLE)
        {
            printf("(2^%lu)*",ud01.u64.exp-DF_BIAS_DOUBLE);
        }else{
            printf("(2^-%lu)*",DF_BIAS_DOUBLE-ud01.u64.exp);
        }
        printf("1.");
    }else{
        printf("(2^-%lu)*",+DF_BIAS_DOUBLE-1);
        printf("0.");
    }
    print_ulongbin(ud01.u64.frac,DF_DOUBLE_FRAC_BITS);
    //("\n%c-%.2x-%.8x\n",uf01.u32.s!=0?'-':'+',uf01.u32.exp,uf01.u32.frac);






}


void missile()
{
    float xf01=0.1f;
    float xf00;
    unsigned int *fp00=(unsigned int *)&xf00;
    double xd01=(double)1.0/10.0;//=1.6/16
    double f2d01=xf01;
    printf("1. === float 0.1 toward even === \n float0.1 hex =");
    printf("==================================\n");
    printf("float2double 0.1 HEX\n ");
    printbyte_reverse((unsigned char *)&f2d01,sizeof(f2d01));
    printf("float 0.1 s/E/M\n ");
    printfloat(xf01);
    printf("float 0.1 HEX\n ");
    printbyte_reverse((unsigned char *)&xf01,sizeof(xf01));

    //mimic accurate 0.1
    printf("==================================\n");
    printf("accurate double 0.1 s/E/M\n");
    printdouble(xd01);
    printf("accurate double 0.1 hex\n");    
    printbyte_reverse((unsigned char *)&xd01,sizeof(xd01));
 
    //difference of float0.1 and double
    printf("==================================\n");
    printf("error double 0.1-float 0.1 toward even\n");
    f2d01 -= xd01;
    printf("error double 0.1-float 0.1 s/E/M\n");
    printdouble(f2d01);
    printf("error double 0.1-float 0.1 HEX\n");
    printbyte_reverse((unsigned char *)&f2d01,sizeof(f2d01));
    //error & meter error
    printf("==================================\n");
    printf("ErrorValue decimal:%E\n",f2d01);
    printf("==================================\n");
    printf("Metter(t_even)=%lf\n",f2d01*(100.0*3600.0*10.0*2000.0));
 
    //round down
    xf00=xf01;
    *fp00 -=1;
    printf("2. === float 0.1 round down ===\n");
    printf("==================================\n");
    printf("float 0.1 r/d s/E/M\n ");
    printfloat(xf00);
    printf("float 0.1 r/d Hex\n ");
    printbyte_reverse((unsigned char *)&xf00,sizeof(xf00));

    printf("==================================\n");
    f2d01 = xd01-xf00;
    printf("error double 0.1-float 0.1\n");
    printf("float 0.1 r/d s/E/M\n ");
    printdouble(f2d01);
    printf("float 0.1 r/d Hex\n ");
    printbyte_reverse((unsigned char *)&f2d01,sizeof(f2d01));
    printf("==================================\n");
    printf("ErrorValue decimal:%E\n",f2d01);
    printf("==================================\n");
    printf("Metter(t_0)=%lf\n",f2d01*(100.0*3600.0*10*2000.0));



}

#define DF_FSIGN(x) ((x)>>(sizeof(float)*8U-DF_SIGN_BIT)&0x01U)
#define DF_FEXP(x) ((x)>>(sizeof(float)*8U-DF_SIGN_BIT-DF_FLOAT_EXP_BITS)&0xffU)
#define DF_FFRAC(x) ((x)&(~(0x0F8ULL<<DF_FLOAT_FRAC_BITS)))
//#define DF_FMNOR(x)   ((x)|0x01UL<<24)

#define DF_DSIGN(x) ((x)>>(sizeof(double)*8U-DF_SIGN_BIT)&0x01U)
#define DF_DEXP(x) ((x)>>(sizeof(double)*8U-DF_SIGN_BIT-DF_DOUBLE_EXP_BITS)&0x7ffU)
#define DF_DFRAC(x) ((x)&(~(0xfffULL<<DF_DOUBLE_FRAC_BITS)))
//#define DF_DMNOR(x)   ((x)|(1UL<<53))

void printfloat(float x1)
{
    unsigned int *fp=(unsigned int *) &x1;
    unsigned int flt=*fp;
    uint64_t signe=DF_FSIGN(flt);
    uint64_t exp=DF_FEXP(flt);
    uint64_t frac=DF_FFRAC(flt);
    uint64_t Ecode=0;
    uint64_t Mcode=0;
    uint64_t Bias=(1U<<(8-1))-1;
    signed short Ecode1;
    if(exp==0U)
    {
        //denormalized
        Ecode1=-Bias+1;
        Mcode=frac;
        // printf("sign=%c\n",signe!=0? '-' : '+');
        // printf("E=%d\n",Ecode1);
        // printf("frac=0.");//0.
        printf("Value=%c",signe!=0? '-' : '+');
        printf("(2^(%d))*",Ecode1);
        printf("0.");//0.
        

    }else if(exp==0xffUL && frac == 0UL){
        printf("Value=INF \n");
        return;

    }else if(exp==0xffUL && frac != 0UL){
        //special INF
        printf("Value=NAN \n");
        return;

    }else{
        //normalized 
        Ecode1=-Bias+exp;
        Mcode =frac;//1 is not add
        printf("Value=%c",signe!=0? '-' : '+');
        printf("(2^(%d))*",Ecode1);
        printf("1.");  //1bit more
    }
    //printf fraction
    print_ulongbin(flt,(sizeof(x1)*8U-DF_FLOAT_EXP_BITS-DF_SIGN_BIT)); 
    //printf("\nVaule(2^n)=%g*2^%d\n",x1*pow(2,abs(Ecode1)),Ecode1);
    printfpow2(x1,Ecode1);

}

void printdouble(double x1)
{
    uint64_t *fp=(uint64_t *) &x1;
    uint64_t flt=*fp;
    uint64_t signe=DF_DSIGN(flt);
    uint64_t exp=DF_DEXP(flt);
    uint64_t frac=DF_DFRAC(flt);
    uint64_t Ecode=0;
    uint64_t Mcode=0;
    //w=11
    uint64_t Bias=(1U<<(11-1))-1;
    signed short Ecode1;
    if(exp==0U)
    {
        //denormalized
        Ecode1=-Bias+1;
        Mcode=frac;
        printf("Value=%c",signe!=0? '-' : '+');
        printf("(2^(%d))*",Ecode1);
        printf("0.");//0.
    }else if(exp==0x7ffUL && frac == 0UL){
        printf("INF \n");
        return;

    }else if(exp==0x7ffUL && frac != 0UL){
        //special INF
        printf("NAN \n");
        return;

    }else{
        //normalized 
        Ecode1=-Bias+exp;
        Mcode =frac;//1 is not add
        printf("sign=%c",signe!=0? '-' : '+');
        printf("(2^(%d))*",Ecode1);
        printf("1.");  //1bit more
    }
    //printf fraction
    print_ulongbin(flt,(sizeof(x1)*8U-DF_DOUBLE_EXP_BITS-DF_SIGN_BIT)); 
    //printf("\nVaule(2^n)=%g*2^%d\n",x1*pow(2,abs(Ecode1)),Ecode1);
    printfpow2(x1,Ecode1);
}

double printfpow2(double dat,  int Ecode1 )
{
    if(Ecode1<0)
    {
        printf("Vaule(2^n)=%.20g*2^%d\n",dat*pow(2,-Ecode1),Ecode1);
    }else{
        printf("Vaule(2^n)=%.20g*2^%d\n",dat/pow(2,Ecode1),Ecode1);        
    }

}

void  print_ulongbin(uint64_t data1,size_t ulen)
{
    unsigned int ui1;
    uint64_t mask;
    unsigned char x=1;
    for(ui1=0;ui1<ulen;ui1++)
    {
        if((ui1%4)==0)
        {
            printf(" ");
        }
        mask = ((uint64_t)0x01ULL)<<(ulen-ui1-1);
        x=((data1&mask)!=0);
        printf("%u",x);

    }
    printf("\n");

}


void printbyte(unsigned char *p, size_t len)
{

    size_t ui1;
    for(ui1=0;ui1<len;ui1++)
    {
        printf("%2.2x ",(unsigned char)p[ui1]);

    }

    printf("\n");

}

void printbyte_reverse(unsigned char *p, size_t len)
{

    size_t ui1;
    for(ui1=0;ui1<len;ui1++)
    {
        printf("%2.2x ",(unsigned char)p[len-ui1-1]);

    }

    printf("\n");

}

// print binary of the char 
// char with visible ascii
void printbin(unsigned char *buf,size_t maxlen)
{
    int i,j;
    for(i=0;i<maxlen;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%c",(buf[i]&(0x80>>j))!=0?'1':'0');
            if(j!=0 && (j%4)==0)
            {
                printf("%c",'-');

            }
        }
        printf(" ");
    }
    printf("\n");

}