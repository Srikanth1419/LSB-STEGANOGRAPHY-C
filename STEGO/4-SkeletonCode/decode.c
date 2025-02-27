#include <stdio.h>
#include "Decode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

struct _decode
{
    char password[11];
    uint password_len;

    char dec_file_extn[50];
    uint dec_file_extn_size;

    uint sec_data_len;

    uint dec_off_set;

    uint temp_len;

    char *str;
}d;

Result do_decoding(DecodeInfo *decInfo)
{
    printf("INFO : Opening Required Files\n");
    if((dec_open_files(decInfo)) ==d_success)
    {
      
       printf("INFO : DONE\n");
       printf("## Decoding  Procedure Started ##\n");
      
        if(skip_header(decInfo) == d_success)
        {
            printf("INFO : DONE\n");
            if(decode_magic_string_len(decInfo) == d_success)
            {
                printf("INFO : DONE\n"); 

                read_pass_word();               // Reading Magic String from User To confirm
                
               if(decode_magic_string(d.password_len,decInfo) ==d_success)
               {
                    printf("INFO : DONE\n");
                    free(d.str);
                    
                    if(decode_file_extn_size(decInfo) == d_success)
                    {
                        printf("INFO : DONE\n");
                        if(decode_file_extn_name(d.dec_file_extn_size, decInfo) == d_success)
                        {
                            strcat(decInfo->output_fname, d.dec_file_extn); // Merging decoded extn with provided file
                            free(d.str);
                            printf("INFO : DONE\n");
                            if(open_output_file(decInfo) == d_success)
                            {
                                printf("INFO : DONE\n");
                         
                                if(decode_secret_data_len(decInfo) == d_success)
                                {
                                    printf("INFO : DONE\n");
                                

                                    if(decode_secret_data(d.sec_data_len, decInfo) == d_success)
                                    {
                                        printf("INFO : DONE\n");
                                        free(d.str);
                                        return d_success;
                                    }
                                    else
                                    {
                                        printf("ERROR : Decoding of secret data not successfully Done\n");
                                        return d_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR : Decoding of secret data length not successfully Done\n");
                                    return d_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR :Opening of output file not successfully Done\n");
                                return d_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR : Decoding of file extension not successfully Done\n");
                            return d_failure;
                        }
               
                    }
                    else
                    {
                        printf("ERROR : Decoding of length of file extension not successfully Done!\n");
                        return d_failure;
                    }
               }
               else
               {
                    printf("ERROR : Decoding of Magic String not successfully Done\n");
                     return d_failure;
               }

            }
            else
            {
                printf("ERROR : Decoding of length of magic string not successfully Done\n");
                return d_failure;
               
            }
        }
        else
        {
            printf("ERROR : skipping of BMP image Header not successfully Done\n");
            return d_failure;
        }
    }
    else
    {
        printf("ERROR : Opening of required files not successfully Done\n");
        return d_failure;
    }
}
void read_pass_word()
{
    printf("Req : Enter the Magic string in between 10 Characters : ");
    __fpurge(stdin);
    scanf("%s", d.password);
   int len = strlen(d.password);

    if(len >= 10)
    read_pass_word();   
}
Result dec_open_files(DecodeInfo *decInfo)
{
    
    decInfo->fptr_stego_img = fopen(decInfo->stego_fname, "r");

    if(decInfo->fptr_stego_img == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_fname);
        return d_failure;
    }
    else
    {
        printf("INFO :Opened %s\n", decInfo -> stego_fname);
    }
    return d_success;
}

Result open_output_file(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->output_fname, "w"); // opening output file in write mode
                            
    if(decInfo->fptr_stego_img == NULL)                    // checking file is opened correctly or not
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        return d_failure;
    } 
    else
    {
        printf("INFO : Opened %s\n", decInfo -> output_fname);
        printf("INFO : DONE. Opened all required Files\n");
    }  
   
   
    return d_success;
}
Result skip_header(DecodeInfo *decInfo)
{
     printf("INFO : Skipping Image Header 54 Bytes\n");
    d.dec_off_set = ftell(decInfo -> fptr_stego_img);
    
    d.dec_off_set = fseek(decInfo->fptr_stego_img, 54,0);

    d.dec_off_set = ftell(decInfo -> fptr_stego_img);

    if(d.dec_off_set == 54)
        return d_success;
    else
        return d_failure;
    
}


Result decode_magic_string_len(DecodeInfo *decInfo)
{
    printf("INFO : Decoding Magic String Length\n");
    #if 1
    if(decode_lsb_to_int(decInfo) == d_success)
    {
        if(d.temp_len >=0 && d.temp_len <=10)
        {
            d.password_len = d.temp_len;
            return d_success;
        }
        else
        {
            printf("INFO : Magic String Length is not in Range\n");
            return d_failure;
        }
    }
    else
        return d_failure;
    #endif
}

Result decode_magic_string(uint size, DecodeInfo *decInfo)
{
    
    printf("INFO : Decoding Magic String\n");
    d.str = calloc(size+1, sizeof(char));       // Allocating dynamically Memory Based on size of magic string 
    Result ret = decode_lsb_to_string(size, decInfo);

    if( !(strcmp(d.str, d.password)))       // comparing decoded password and user given password
    {
        printf("INFO : User Entered Magic String and Decoded Magic String same\n");
        return d_success;
    }
    else
    {
        printf("INFO : User Entered Magic String and Decoded Magic String is not Matched\n");
        return d_failure;
    }
    
}

Result decode_file_extn_size(DecodeInfo *decInfo)
{
    printf("INFO : Decoding file extension size\n");
    if(decode_lsb_to_int (decInfo) == d_success)
    {
        d.dec_file_extn_size = d.temp_len;
        return d_success;
    }
    else
    {
        return d_failure;
    }
}

Result decode_file_extn_name(uint size, DecodeInfo *decInfo)
{
    printf("INFO : Decoding file extension\n");
    d.str = calloc(size+1, sizeof(char));        // Allocating Memory dynamically Based on size of file extn

    Result ret = decode_lsb_to_string(size, decInfo);

    strcpy(d.dec_file_extn, d.str);

    if((strstr(d.dec_file_extn, ".")))
    
        return d_success;
    else
        return d_failure;

}

Result decode_secret_data_len(DecodeInfo *decInfo)
{
    printf("INFO : Decoding file size\n");
    if(decode_lsb_to_int(decInfo) == d_success)
    {
        d.sec_data_len =d.temp_len;
        return d_success;
    }
    else
    {
        return d_failure;
    }
}

Result decode_secret_data(uint size, DecodeInfo *decInfo)
{
    printf("INFO : Decoding secret file data\n");
    d.str = calloc(size+1, sizeof(char));       // Allocating Memory dynamically Based on size of secret Data and +1 to store a null character

    if(decode_lsb_to_string(size, decInfo) == d_success)
    {
        fwrite(d.str, size, 1,decInfo -> fptr_output);
        return d_success;
    }
    else
    {
        return d_failure;
    }
}

Result decode_lsb_to_int(DecodeInfo *decInfo)
{
   d.temp_len =0;
    
    char buff[32];
    fread(buff, 32, 1, decInfo -> fptr_stego_img);

    d.dec_off_set = ftell(decInfo -> fptr_stego_img);
    for(int i=0;i<32;i++)
    {
       d.temp_len = (((buff[i]&1)<<i)|(d.temp_len));
    }
    if(d.temp_len > 0)
        return d_success;
    else
        return d_failure;

}


Result decode_lsb_to_string(uint size,DecodeInfo *decInfo)
{
    char buff[8];
    char ch =0;
    int i;
    for( i=0;i<size;i++)
    {
        ch=0;
        fread(buff, 8,1,decInfo -> fptr_stego_img);
        for(int j=0;j<8;j++)
        {
            ch = (((buff[j]&1)<<j)|(ch));  
        }
        d.str[i] = ch;
    }
    d.str[i] ='\0';

    if(i == size)
    {
        return d_success;
    }
    else
    {
        return d_failure;
    }
}