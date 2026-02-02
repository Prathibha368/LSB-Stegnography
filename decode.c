#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"

Status do_decoding(char *argv[],DecodeInfo *decInfo){
    if((validate_decode_args(argv,decInfo))==failure){          //validation for argv ;calling the function for it.
        return failure;
    }
    printf("##  Decoding Procedure started  ##\n");  
    if (decode_magic_string(MAGIC_STRING, decInfo) == failure)
    {
        return failure;
    }
    if(decode_secret_file_extn(decInfo)==failure){
        return failure;
    }
    if(decode_secret_file_data(decInfo)==failure){
        return failure;
    }
    return success;

}
Status validate_decode_args(char *argv[], DecodeInfo *decInfo){
    //check if argv[2] is .bmp
    char * ptr1 =strstr(argv[2],".bmp");
    {
        if(ptr1==NULL){
        printf("Invalid encoded file name\n");
        return failure;
        }
        if(ptr1){
            if(strcmp( ptr1 ,".bmp")==0){
                decInfo->encoded_image_fname = argv[2];
               
            }
        }
        else{
            printf("Invalid encoded file name\n");
            return failure;
        } 
        printf("Its a bmp file\n");
    }
    
    if (argv[3]== NULL){
        printf("output file not mentioned. Creating output.(encoded extension) as default\n");
        strcpy(decInfo->output_fname,"data_file");
    }
    else{
        int i=0;
        while(argv[3][i] !='\0' && argv[3][i] !='.'){
           decInfo->output_fname[i]=argv[3][i];
           i++;
        }
        decInfo->output_fname[i] = '\0';
    }

    if(open_img_file(decInfo)==failure){
        return failure;
    }
    return success;
    
}
Status open_img_file(DecodeInfo *decInfo){
    decInfo->encoded_image_fptr = fopen(decInfo->encoded_image_fname, "r");
    printf("Opening required files\n");
    if (decInfo->encoded_image_fptr == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->encoded_image_fname);
        return failure;
    }
    printf("Opened %s\n",decInfo->encoded_image_fname);
    return success;
}
/* decode 1 byte from LSB of image data array */
char decode_1byte_from_lsb(char *buffer_8){
    char ch=0;
    for(int i=7;i>=0;i--){
        ch= ch |(buffer_8[7-i] &  1)<<i;
    }
    return ch;
}
/* decode 4 bytes from LSB of image data array */
unsigned int  decode_4byte_from_lsb(char *buffer_32){
    unsigned char ch=0;
    for(int i=31;i>=0;i--){
        ch= ch |(unsigned int )(buffer_32[31-i] &  1)<<i;
    }
    return ch;
}
/* decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo){
    fseek(decInfo->encoded_image_fptr, 54, SEEK_SET);
    char buffer_32[32];
    fread(buffer_32,sizeof(char),32,decInfo->encoded_image_fptr);
    //decode_4byte_from_lsb(strlen(MAGIC_STRING),ch);
   
    unsigned int magic_string_len=decode_4byte_from_lsb(buffer_32);
    char decoded_magic_string[magic_string_len+1];
    char buffer_8[8];
    for(int i=0;i<magic_string_len;i++){
        fread(buffer_8,sizeof(char),8,decInfo->encoded_image_fptr);
        decoded_magic_string[i]=decode_1byte_from_lsb(buffer_8);
    }
    decoded_magic_string[magic_string_len]='\0';

    if(strcmp(decoded_magic_string,MAGIC_STRING)==0){
        return success;
    }
    return failure;

}
/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo){
    printf("Decoding output file Extension\n");
    char buffer_32[32];
    fread(buffer_32,sizeof(char),32,decInfo->encoded_image_fptr);
   
   
    unsigned int extn_len=decode_4byte_from_lsb(buffer_32);
    char file_extn[extn_len+1];
    char buffer_8[8];
    for(int i=0;i<extn_len;i++){
        fread(buffer_8,sizeof(char),8,decInfo->encoded_image_fptr);
        file_extn[i]=decode_1byte_from_lsb(buffer_8);
    }
    file_extn[extn_len]='\0';
    strcat(decInfo->output_fname,file_extn);
    decInfo->output_fptr=fopen(decInfo->output_fname,"w");
    if((decInfo->output_fptr)==NULL){
        return failure;
    }
    printf("Opened %s\n",decInfo->output_fname);
    printf("Done.Opened all required files\n");
    return success;
}

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo){
    printf("Decoding File Size\n");
    char buffer_32[32];
    fread(buffer_32,sizeof(char),32,decInfo->encoded_image_fptr);

   
    unsigned int file_size=decode_4byte_from_lsb(buffer_32);
    printf("Done\n");
    printf("Decoding File Data\n");
   
    char ch;
    char buffer_8[8];
    for(int i=0;i<file_size;i++){
        fread(buffer_8,sizeof(char),8,decInfo->encoded_image_fptr);
        ch=decode_1byte_from_lsb(buffer_8);
        fwrite(&ch,sizeof(char),1,decInfo->output_fptr);
    }
    printf("Done\n");
    printf("##   Decoding done successfully  ##\n");
    return success;
}