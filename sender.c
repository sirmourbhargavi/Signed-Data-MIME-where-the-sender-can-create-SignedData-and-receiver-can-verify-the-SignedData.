

#include <stdio.h>
#include "../../miracl.h"   /* include MIRACL system */
#include <stdlib.h>
#include <string.h>
#include <math.h>

char hash[32];
char buffer[1024];
char message[1024];
void printHex(char *stng);
void string2hexString(char* input, char* output);

miracl *mip;
char* hash_256() {
   
    int i;
    sha256 sh;
    shs256_init(&sh);
    for (i=0;buffer[i]!=0;i++) shs256_process(&sh,buffer[i]);
    shs256_hash(&sh,hash);    
    return hash;
}


int main()
{  
    
    /*  encode using public key, e = 3, N = 'taken from file'  */
    big N,e,m,c;

    FILE *ifile;
    FILE *ofile;

    static char buff[256];

    int i,mlen,klen;

    mip=mirsys(100,0);

    N=mirvar(0);
    e=mirvar(0);
    m=mirvar(0);
    c=mirvar(0);


    /*Read Public Parameters from file public.key*/
    ifile=fopen("public.key","rt");
    mip->IOBASE=16;
    cinnum(N,ifile);
    cinnum(e,ifile);
    fclose(ifile);
                                       /*    m^(e) mod       */ 
    
    /* find key length in characters (ASCII). Reset it as required!. Could be completely automated. */
    klen = logb2(N)/7 - 1;    
    printf("Key length = %d\n",klen);

    
    //cotnum(N,stdout);
    //cotnum(e,stdout);

    
    ofile = fopen("output.enc","wt");
    printf("Hii !!I am sender. \n Enter the text...: \n");
    fgets(buffer,1024,stdin);
    strcpy(message,buffer);

    string2hexString(hash_256(),buffer);
    strcat(buffer,";");
    strcat(buffer,message);
    printf("message to be send:%s\n",buffer);
    mlen = strlen(buffer);
    while(TRUE)
    {
        if(mlen < klen)
        {
            break;
        }
        for(int i=0; i<klen; i++)
            buff[i] = buffer[i];            
        buff[klen] = '\0';

        for(int i=klen; i<=mlen; i++)
            buffer[i-klen] = buffer[i];
        mlen = strlen(buffer);

        mip->IOBASE=128;
        cinstr(m,buff);
        /*cotnum(m,stdout);*/
        powmod(m,e,N,c);
        mip->IOBASE=16;
        /*cotnum(c,stdout);*/
        cotnum(c,ofile);
    }

    /*Unpadded*/
    if (mlen < klen && mlen != 0)
    {
        for (int i = 0; i < mlen; ++i)
            buff[i] = buffer[i];
        buff[mlen] = '\0';

        mip->IOBASE=128;
        cinstr(m,buff);
        powmod(m,e,N,c);
        mip->IOBASE=16;
        /*cotnum(c,stdout);*/
        cotnum(c,ofile);
    }
    fclose(ofile);
    printf("\nMessage has been send...\n");
    return 0;
}   



void string2hexString(char* input, char* output) {
    int loop;
    int i;
    

    for(i=0,loop=0; loop < 32 ;loop++,i+=2) {
        sprintf((char*)(output+i),"%02X", input[loop]);
    }
    output[i++] = '\0';
    
    
}
