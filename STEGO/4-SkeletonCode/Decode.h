#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{   
    /*Encoded file info*/
    char *stego_fname;
    FILE *fptr_stego_img;
    
    /*Decoding of secret file Info*/
    char output_fname[100];
    FILE *fptr_output;
    
}DecodeInfo;

Result do_decoding(DecodeInfo *decInfo);

Result read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

Result dec_open_files(DecodeInfo *decInfo);

Result open_output_file(DecodeInfo *decInfo);

Result skip_header(DecodeInfo *decInfo);

Result decode_magic_string_len(DecodeInfo *decInfo);

Result decode_magic_string(uint size, DecodeInfo *decInfo);

Result decode_file_extn_size(DecodeInfo *decInfo);

Result decode_file_extn_name(uint size, DecodeInfo *decInfo);

Result decode_secret_data_len(DecodeInfo *decInfo);

Result decode_secret_data(uint size, DecodeInfo *decInfo);

Result decode_lsb_to_int(DecodeInfo *decInfo);

Result decode_lsb_to_string(uint size,DecodeInfo *decInfo);

void read_pass_word();

#endif

