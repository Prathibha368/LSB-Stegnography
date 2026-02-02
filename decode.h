#ifndef DECODE_H
#define DECODE_H
#include "common.h"
typedef struct DecodeInfo
{
    /* Source Image info */
    /*char *src_image_fname;  // => Store the Src_Image_fname
    FILE *src_image_fptr;   // => File pointer for src_image

    /* Secret File Info */
    char output_fname[20];    // => Store the Secret_fname
    FILE *output_fptr;     // => File pointer for secret_file
    /*char secret_extn[7];
    int secret_extn_size;
    unsigned long int secret_file_size;*/

    /* Stego Image Info */
    char *encoded_image_fname;   // => Store the Output_img_fname
    FILE *encoded_image_fptr;    // => File pointer for output_Img

} DecodeInfo;
/* Read and validate Decode args from argv */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* Perform the decoding */
Status do_decoding(char *argv[],DecodeInfo *decInfo);   //main function for decoding

Status open_img_file(DecodeInfo *decInfo);
/* decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);
/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);
/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);
/* decode 1 byte from LSB of image data array */
char decode_1byte_from_lsb(char *buffer_8);
/* decode 4 bytes from LSB of image data array */
unsigned int  decode_4byte_from_lsb(char *buffer_8);

#endif