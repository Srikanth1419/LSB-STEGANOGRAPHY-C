#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */


typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
  
    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
  
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);


/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* encode magic string length*/
Status encode_magic_string_len(uint size, EncodeInfo *encInfo);


/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_extn_size(uint file_size, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/*Encode secret file size*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*encoding integer to data to source file data*/
Status encode_int_to_lsb(uint data, EncodeInfo *encInfo);

/*encoding string  data to source file data*/
Status encode_string_to_lsb(const char *data, EncodeInfo *encInfo);


/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);


/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo);

#endif

