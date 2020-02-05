#include <stdio.h>
#include <stdlib.h>
#define DF_LINEMAX (16)

unsigned char buf[DF_LINEMAX];
void printchar(char *bun,int maxlen);
void printfmt(char *bun,int maxlen, char* fmt);


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

    //open file for binary read
    FILE *fp;
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
    for(i=0;i<maxlen;i++)
    {
        if(buf[i]=='\n')
        {
            printf("\\n\t",buf[i]);
        }
        else if(buf[i]=='\r')
        {
            printf("\\r\t",buf[i]);
        }else{
            printf("%c\t",buf[i]);
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
