#include <stdio.h>
#include <stdlib.h>
#define DF_LINEMAX (16)

unsigned char buf[DF_LINEMAX];
void printchar(char *bun,int maxlen);
void printfmt(char *bun,int maxlen, char* fmt);
void create_datfile(char * fname);

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
            buf[len]=data;
            len++;
        }else{
            //16byte of chars output
            printchar(buf,len);    
            printfmt(buf,len,"%2.2x\t");
            printfmt(buf,len,"%03d\t");
            printf("\n");
            len=0;
        }
    }

    fclose(fp);
    return 0;

}

void printchar(char *bun,int maxlen)
{
    int i;
    char tempdata;
    
    for(i=0;i<maxlen;i++)
    {
        tempdata=buf[i];
        switch(tempdata)
        {
        case '\n':/* constant-expression */
            /* code */
            printf("\\n\t");
            break;
        case '\r':/* constant-expression */
            /* code */
            printf("\\r\t");
            break;
        case '\a':/* constant-expression */
            /* code */
            printf("\\a\t");
            break;
        case '\t':/* constant-expression */
            /* code */
            printf("\\t\t");
            break;
        case '\f':/* constant-expression */
            /* code */
            printf("\\f\t");
            break;
        default:
            printf("%c\t",tempdata);
            break;
        }
    }
    printf("\n");

}


void printfmt(char *bun,int maxlen, char* fmt)
{
    int i;
    for(i=0;i<maxlen;i++)
    {
        printf(fmt,buf[i]);
    }
    printf("\n");
}

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
    
    fclose(fp);
}