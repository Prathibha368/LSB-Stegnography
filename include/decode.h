#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include "common.h"
typedef struct DecodeInfo
{
    /* Stego Image Info */
    char *encoded_image_fname;   // => Store the encodded_img_fname
    FILE *encoded_image_fptr;    // => File pointer for encodded_Img

    /* output File Info */
    char output_fname[30];    // => Store the output_fname
    FILE *output_fptr;     // => File pointer for output_file
  //  char secret_extn[7];   // => store the extention while doing the validation
   // int secret_extn_size;
   // unsigned long int secret_file_size;

    

} DecodeInfo;
/*validating arguments*/
Status validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* Perform the decoding */
Status do_decoding(char *argv[],DecodeInfo *decInfo); //main function for decoding 
Status open_image_file(DecodeInfo *decInfo);

/* decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode 1 byte from LSB of image data array */
char decode_1byte_from_lsb( char *buffer_8);
/* decode 4 bytes from LSB of image data array */
int decode_4byte_from_lsb(char *buffer_32);


#endif
