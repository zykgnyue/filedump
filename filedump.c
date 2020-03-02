//This code is start at 2020/2/12 during corona-virus events
// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DF_LINEMAX (16)
#define DF_BINMODE_BYTENUM (4)
#define DF_HEXFMT "%2.2x   "
#define DF_DECFMT "%03d  "
#define DF_HEXDECSPC "%5s"
#define DF_BINSPC "%8s\t"
//define support file dump mode
typedef enum dumpxmode{
    USAGE,  //help message
    xMODE,  //up/down hex
    XMODE,  //left/right hex
    dMODE,  //up/down decimal
    DMODE,  //left/right decimal
    bMODE,  //up/down binary
    BMODE   //L/R binary
} FDUMPxmode;

// define enough buffer
char buff[DF_LINEMAX+DF_BINMODE_BYTENUM];
//prototype define
void printchar(char *buf,int maxlen,FDUMPxmode dpmode);
void printfmt(char *buf,int maxlen, char *fmt,FDUMPxmode dpmode);
void create_datfile(char * fname);
void printbin(char *buf,int maxlen,FDUMPxmode dpmode);
void processbuf(char*buf, int len, FDUMPxmode dpxmode);
int getLineLength(FDUMPxmode dpxmode);
FDUMPxmode getdpmode(char *args);
void printSpace(int cols,FDUMPxmode dpmode);

char *asciiabl[]={ \
    "NUL","SOH","STX","ETX","EOT",
    "ENQ","ACK","BEL","BS","TAB",
    "LF", "VT", "FF", "CR", "SO",
    "SI","DLE","DC1","DC2","DC3",
    "DC4","NAK","SYN","ETB","CAN",
    "EM","SUB","ESC","FS","GS","RS",
    "US","SP","!"};

int main(int argc,char * argv[])
{
    unsigned char data;
    FDUMPxmode dpxmode;
    int binlen;
    int len=0;
    int lenMAX = DF_BINMODE_BYTENUM;
     /*get arguments from command line*/
    if(argc<3)
    {
        //not input filename 
        dpxmode =USAGE;
    }else{ //>=3
        dpxmode =getdpmode(argv[1]);
    }

    if(dpxmode == USAGE)
    {
        printf("Usage: filedump option file.dat\n");
        printf("option: -x|-X|-d|-D|-b|-B \n");
        printf("file.dat: the file will be dumped.\n");
        printf("\n");
        return 1;
    }

    // get output bytes number
    lenMAX= getLineLength(dpxmode);

    //create_datfile(argv[1]);

    //open file for binary read
    FILE *fp;
    //file stream open for read binary xmode
    fp=fopen(argv[2],"rb");
    if(fp==NULL)
    {
        //open failed
        printf("open file %s failed!\n",argv[1]);
        return(2);
    }

    //read file contents and printf
    len=0;
    while(!feof(fp))
    {
        //get 1byte of char
        data=fgetc(fp);

        //read DF_LINEMAX byte of chars
        if(len < lenMAX && (!feof(fp)))
        {
            buff[len]=data;
            len++;
        }else{
            //16byte of chars output            
            processbuf(buff,len,dpxmode);

            len=0;
            //revise for lost 0x10 0x20 ...
            buff[len]=data;
            len++;
        }
    }
        

    fclose(fp);
    return 0;

}

//process read in data from FileStream
void processbuf(char*buf, int len, FDUMPxmode dpxmode)
{
    
    switch(dpxmode)
    {
        case xMODE: //up/down hex
            printchar(buf,len,dpxmode); 
            printf("\n");
            printfmt(buf,len,"%2.2x\t",dpxmode);
            printf("\n");
            break;
        case dMODE: //up/down dec
            printchar(buf,len,dpxmode); 
            printf("\n");
            printfmt(buf,len,"%03d\t",dpxmode);
            printf("\n");
            break;
        case bMODE: //up/down binary
            printchar(buf,len,dpxmode); 
            printf("\n");
            printbin(buf,len,dpxmode);
            printf("\n");
            break;
        case XMODE: // /L/R hex
            printfmt(buf,len,DF_HEXFMT,dpxmode);
            printf("  ");
            printchar(buf,len,dpxmode); 
            printf("\n");
        break;
        case DMODE: //L/R dec
            printfmt(buf,len,DF_DECFMT,dpxmode);
            printf("  ");
            printchar(buf,len,dpxmode); 
            printf("\n");
            break;
        case BMODE: //L/R bin
            printbin(buf,len,dpxmode);
            printf("  ");
            printchar(buf,len,dpxmode); 
            printf("\n");
            break;
        default:
            //never come here
            printf("Never come here!\n");
            break;
    }       
    printf("\n");
}

//get output byte number 
int getLineLength(FDUMPxmode dpxmode)
{
    //get read byte number 
    int len;
    if(dpxmode ==bMODE || dpxmode ==BMODE)
    {
        len=DF_BINMODE_BYTENUM;
    }else{
        len= DF_LINEMAX;
    }
    return  len;
}

//get selected mode 
FDUMPxmode getdpmode(char *args)
{
    FDUMPxmode dpxmode;
    if(strncmp(args,"-x",2)==0)
    {   
        dpxmode =xMODE;
    }else if(strncmp(args,"-X",2)==0)
    {
        dpxmode =XMODE;
    }else if(strncmp(args,"-d",2)==0)
    {
        dpxmode =dMODE;

    }else if(strncmp(args,"-D",2)==0)
    {
        dpxmode =DMODE;

    }else if(strncmp(args,"-b",2)==0)
    {
        dpxmode =bMODE;

    }else if(strncmp(args,"-B",2)==0)
    {
        dpxmode =BMODE;

    }else{
        //usage
        dpxmode = USAGE;
    }
    return dpxmode;
}

// print char with visible ascii
void printchar(char *buf,int maxlen, FDUMPxmode dpmode)
{
    int i;
    char tempdata;
    
    for(i=0;i<maxlen;i++)
    {
        tempdata=buf[i];
        if(tempdata <0)
        {
            if(dpmode==XMODE || dpmode==DMODE||dpmode==BMODE)
            {
                printf("%c",'?');
            }else if(dpmode==bMODE){

                printf("%c\t\t",'?');
            }else{
                printf("%c\t",'?');

            }
        }else if(tempdata<=0x20){
            //index must unsigned 
            if(dpmode==XMODE || dpmode==DMODE||dpmode==BMODE)
            {
                //L/R mode replace with DOT|?
                printf("%c",'?');
            }else if(dpmode==bMODE){
                //u/d mode replace with special named chars
                printf("%s\t\t",asciiabl[(unsigned char)tempdata]);

            }else{
                printf("%s\t",asciiabl[(unsigned char)tempdata]);
            }

        }else{
            //0x21==0x7F
            if(dpmode==XMODE || dpmode==DMODE||dpmode==BMODE)
            {
                //L/R
                //0x7F DELETE has no display font
                printf("%c",(tempdata!=0x7f)?tempdata:'?');
            }else{
                //u/d 
                if(tempdata!=0x7F)
                {
                    if(dpmode==bMODE)
                    {
                        printf("%c\t\t",tempdata);
                    }
                    else
                    {
                        /* code */
                        printf("%c\t",tempdata);
                    }
                    
                }else
                {
                    //0x7F DELETE has no display font
                    /* code */
                    if(dpmode==bMODE)
                    {
                        printf("%s\t\t","DEL");
                    }else
                    {
                        /* code */
                        printf("%s\t","DEL");
                    }
                    
                }
            }
        }
    }


}

//print last space for L/R mode
void printSpace(int cols,FDUMPxmode dpmode)
{
    int i;
    if(dpmode ==BMODE)
    {
        for(i=0;i<cols;i++)
        {
            printf(DF_BINSPC," ");
        }

    }else{
        
        for(i=0;i<cols;i++)
        {
            printf(DF_HEXDECSPC," ");
        }


    }
}

// print binary of the char 
// char with visible ascii
void printbin(char *buf,int maxlen,FDUMPxmode dpmode)
{
    int i,j;
    int templen=getLineLength(dpmode);
    for(i=0;i<maxlen;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%c",(buf[i]&(0x80>>j))!=0?'1':'0');
        }
        printf("\t");
    }
    //actural num < define max number

    if(maxlen<templen && maxlen!=0)
    {
        printSpace((templen-maxlen),dpmode);
    }
    // printf("\n");
}

// printf hex oct dec format
void printfmt(char *buf,int maxlen,char *fmt,FDUMPxmode dpmode)
{
    int i;
    int templen=getLineLength(dpmode);

    for(i=0;i<maxlen;i++)
    {
        
        printf(fmt,(unsigned char)buf[i]);

    }
    if(dpmode == DMODE || dpmode==XMODE)
    {
        if(maxlen<templen && maxlen!=0)
        {
            printSpace((templen-maxlen),dpmode);

        }
    }
    /// printf("\n");

}

//create test file with 0x00-0xff
void create_datfile(char * fname)
{
    unsigned char i=0;
    FILE *fp;
    fp=fopen(fname,"wb+");
    if(fp==NULL)
    {
        printf("File creation error!\n");
        exit(4);
    }

    for(i=0;i<UCHAR_MAX;i++)
    {
        fputc(i,fp);
    }
    fputc(i,fp);
    //fputc(i-2,fp);
    
    fclose(fp);
}