#include <stdio.h>
#include <stdlib.h>
#define DF_LINEMAX (16)

char buff[DF_LINEMAX];
void printchar(char *buf,int maxlen);
void printfmt(char *buf,int maxlen, char* fmt);
void create_datfile(char * fname);
void printbin(char *buf,int maxlen);
char *asciiabl[]={ \
    "NUL","SOH","STX","ETX","EOT",
    "ENQ","ACK","BEL","BS","TAB",
    "LF", "VT", "FF", "CR", "SO",
    "SI","DEL","DC1","DC2","DC3",
    "DC4","NAK","SYN","ETB","CAN",
    "EM","SUB","ESC","FS","GS","RS",
    "US","<SP>","!"};

int main(int argc,char * argv[])
{
    unsigned char data;
    int len=0;
     /*get arguments from command line*/
    if(argc<2)
    {
        //not input filename 
        printf("Usage: filedump filename\n");
        printf("filename is the file that you want to dump its contents!\n");
        return 1;
    }
    
    //create_datfile(argv[1]);

    //open file for binary read
    FILE *fp;
    //file stream open for read binary mode
    fp=fopen(argv[1],"rb");
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
        if(len < DF_LINEMAX && (!feof(fp)))
        {
            buff[len]=data;
            len++;
        }else{
            //16byte of chars output            
            printchar(buff,len);    
            printfmt(buff,len,"%2.2x\t\t");
            printfmt(buff,len,"%03d\t\t");
            printfmt(buff,len,"%03o\t\t");
            printbin(buff,len);

            printf("\n");
            len=0;
            //revise for lost 0x10 0x20 ...
            buff[len]=data;
            len++;
        }
    }

    fclose(fp);
    return 0;

}

// print char with visible ascii
void printchar(char *buf,int maxlen)
{
    int i;
    char tempdata;
    
    for(i=0;i<maxlen;i++)
    {
        tempdata=buf[i];
        if(tempdata <0)
        {
            printf("%c\t\t",'?');
        }else if(tempdata<=0x20){
            //index must unsigned 
            printf("%s\t\t",asciiabl[(unsigned char)tempdata]);
        }else{
            printf("%c\t\t",tempdata);
        }
    }
    printf("\n");

}

// print binary of the char 
// char with visible ascii
void printbin(char *buf,int maxlen)
{
    int i,j;
    for(i=0;i<maxlen;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%c",(buf[i]&(0x80>>j))!=0?'1':'0');
        }
        printf("\t");
    }

    printf("\n");
}

// printf hex oct dec format
void printfmt(char *buf,int maxlen, char* fmt)
{
    int i;
    for(i=0;i<maxlen;i++)
    {
        printf(fmt,(unsigned char)buf[i]);
    }
    printf("\n");
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