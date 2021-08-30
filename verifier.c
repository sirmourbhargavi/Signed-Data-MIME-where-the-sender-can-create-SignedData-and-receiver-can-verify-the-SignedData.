/*Program to implement decryption algorithm of RSA */

#include <stdio.h>
#include "../../miracl.h"   /* include MIRACL system */
#include <stdlib.h>
#include <string.h>
#include <math.h>

char hash[32];
char buffer[1024];

void strip(char *name)
{ 
    /* strip off filename extension */
    int i;
    for (i=0; name[i]!='\0'; i++)
    {
        if (name[i]!='.') continue;
        name[i]='\0';
        break;
    }
}

char* hash_256(char *message) {
   
    int i;
    sha256 sh;
    shs256_init(&sh);
    for (i=0;message[i]!=0;i++) shs256_process(&sh,message[i]);
    shs256_hash(&sh,hash);    
    return hash;
}

void string2hexString(char* input, char* output) {
    int loop;
    int i;
    

    for(i=0,loop=0; loop < 32 ;loop++,i+=2) {
        sprintf((char*)(output+i),"%02X", input[loop]);
    }
    output[i++] = '\0';
    
    
}

miracl *mip;

int main() {  
    big N,e,m,c,d,p,q,n;

    FILE *ifile;
    FILE *ofile;
    char buffer[1024] = {'\0'};
    char temp[1024];
    char message[1024];

    mip=mirsys(5000,0);

    N=mirvar(0);
    e=mirvar(0);
    m=mirvar(0);
    c=mirvar(0);
    d=mirvar(0);
    p=mirvar(0);
    q=mirvar(0);
    n=mirvar(0);

    /*Read Public and Private Parameters from files public.key and private.key*/
    ifile = fopen("public.key","rt");
    mip->IOBASE=16;
    cinnum(N,ifile);
    cinnum(e,ifile);
    fclose(ifile);
    ifile=fopen("private.key","rt");
    cinnum(p,ifile);
    cinnum(q,ifile);
    cinnum(d,ifile);
    cinnum(n,ifile);
    fclose(ifile);

    ifile=fopen("output.enc","rt");
    while(TRUE)
    {
        mip->IOBASE=16;
        cinnum(c,ifile);
        if (size(c) == 0)
        {
            break;
        }
        powmod(c,d,N,m);
        mip->IOBASE=128;
        //cotnum(m,stdout);
        cotstr(m,temp);
        strcat(buffer,temp);
    }
    strcat(buffer,"\0");
    printf("I am verifier\n");
    printf("\n msg i reiceved:%s \n",buffer);
    int i = 0,k=0;
    char recHash[1024] = {'\0'};
    while(buffer[i]!=';') {
        recHash[i]=buffer[i];
        i++;
    }
    i++;
    while(buffer[i]!='\0') message[k++] = buffer[i++];
    message[k]='\0';
    printf("Received Hash  : %s \n",recHash);
    printf("Received Message : %s \n",message);
    string2hexString(hash_256(message),message);
    printf("computed hash : %s \n",message);
    for(int j = 0; j<strlen(message); j++) {
        if(message[j]!=recHash[j]) {
            printf("Not varified!\n");
            return 0;
        }
    }
    printf("verified!\n");
    fclose(ifile);

    return 0;
}
