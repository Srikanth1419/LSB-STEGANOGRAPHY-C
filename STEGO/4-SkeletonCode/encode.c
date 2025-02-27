#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
struct temp
{
    char magic[20];
    uint magic_len;
    
    char file_extn[20];
    uint file_extn_len;

    char *sec_data;
    uint  secret_len;
    
    uint src_off_set;
    uint dest_off_set;
}encode;

uint get_image_size_for_bmp(FILE *fptr_image)
{
    char ch;
    int i=0;
    while( fread(&ch, 1, 1, fptr_image) >0)     // Finding Size of source Image
    {
        i++;
    }
    return i;                               // returning image size
}
 
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
      
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    else
    {
        printf("INFO : Opened %s\n", encInfo->src_image_fname);
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    else
    {
        printf("INFO : Opened %s\n", encInfo -> secret_fname);
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling

    if (encInfo->fptr_stego_image == NULL)
    {
      
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    else
    {
        printf("INFO : Opened %s\n", encInfo -> stego_image_fname);
    }

    // No failure return e_success
    return e_success;
}

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO : Opening Required Files\n");
    if(open_files(encInfo)==e_success)
    {
        printf("INFO : DONE\n");
       printf("## Encoding Procedure Started ##\n");
        if(check_capacity(encInfo)== e_success)
        {
            
            printf("INFO : Check Capacity function is completed\n");
            printf("INFO : DONE. Found  OK\n");
             rewind(encInfo -> fptr_src_image);
             rewind(encInfo -> fptr_stego_image);
    
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image )== e_success)
                {
                    printf("INFO : DONE\n");

                  if(encode_magic_string_len(encode.magic_len,encInfo)== e_success)
                        {
                            printf("INFO : DONE\n");
                          
                        if( encode_magic_string(encode.magic,encInfo) == e_success)
                                {
                                    printf("INFO : DONE\n");
                                    
                                     if(encode_secret_file_extn_size(encode.file_extn_len,encInfo) == e_success)
                                        {
                                                printf("INFO : DONE\n");  
                                                
                                                 if(encode_secret_file_extn(encode.file_extn,encInfo) == e_success)
                                                 {
                                                     printf("INFO : DONE\n");
                                                   
                                                     if(encode_secret_file_size(encode.secret_len,encInfo) == e_success)
                                                     {
                                                        printf("INFO : DONE\n");
                                                      
                                                        if(encode_secret_file_data(encInfo) == e_success)
                                                        {
                                                            printf("INFO : DONE\n");
                                                            
                                                            if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo ->fptr_stego_image) == e_success)
                                                            {   
                                                                printf("INFO : DONE\n");
                                                               
                                                                free(encode.sec_data);
                                                                return e_success;
                                                            }
                                                            else
                                                            {
                                                                printf("ERROR : Copying remaining data not successfully Done\n");
                                                                return e_failure;
                                                            } 
                                                        }
                                                        else
                                                        {   
                                                             printf("ERROR : Encoding of secret data not successfully Done\n");
                                                            return e_failure;;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        printf("ERROR : Encoding of length of secret data not successfully Done\n");
                                                        return e_failure;
                                                    }
                                                 }
                                                else
                                                {
                                                    printf("ERROR : Encoding of file extension not successfully Done\n");
                                                   
                                                    return e_failure;
                                                }
                                        }
                                        else
                                        {
                                            printf("ERROR : Encoding of length of file extension not successfully Done!\n");
                                            return e_failure;
                                        }
                                }
                                else
                                {
                                  printf("ERROR : Encoding of Magic String not successfully Done\n");
                                    
                                    return e_failure;
                                }  
                        }
                        else
                        {
                            printf("ERROR : Encoding of length of magic string not successfully Done\n");
                            return e_failure;
                        }     
                }
                else
               {
                    printf("ERROR : Copying of bmp Header not successfully Done\n");
                    //printf("INFO : Failed\n");
                    return e_failure;
                           
                }            
        }
        else
        {
            printf("ERROR : Check capacity function not successfully Done\n");
            return e_failure;
        }        
    }  
    else
    {
        printf("ERROR : Opening of required files not successfully Done\n");
        return e_failure;
    }

}

Status check_capacity(EncodeInfo *encInfo)
{

     printf("INFO : Checking for %s size\n", encInfo -> secret_fname);

     encode.secret_len = get_file_size(encInfo->fptr_secret);        //fun call for finding length of secret file size

     if((signed)encode.secret_len > 0)                     // comparing secret data length by converting as signed int becoz initially secret_len 
                                                            // is unsigned but storing signed int from get_file_size fun 
     {
        printf("INFO : Done. Not Empty\n");
     }
     else
     {
        printf("INFO : There is No secret Data to Ecode:\n");
        return e_failure;
        
     }

     printf("INFO : Checking for %s capacity to handle %s\n", encInfo -> src_image_fname, encInfo -> secret_fname);
    uint img_size = get_image_size_for_bmp(encInfo->fptr_src_image);
   

    uint size_of_all_data;
    int flag =1;
    while(flag)
    {
        printf("Req : Enter a Magic string in Between of 10 Characters : ");
        scanf("%s", encode.magic);
        encode.magic_len = strlen(encode.magic);
        if(encode.magic_len >=0 && encode.magic_len <=10)
        {
            flag =0;
        }
        else
        {
            flag =1;
        }
    }

    strcpy ( encode.file_extn, strstr(encInfo->secret_fname, "."));
   
    encode.file_extn_len = strlen(encode.file_extn);

    size_of_all_data = ( 54+  encode.magic_len *8 + encode.file_extn_len*8 + encode.secret_len*8 + (3*32));

    if(img_size > size_of_all_data)
        return e_success;
    else
        return e_failure;
}

uint get_file_size(FILE *fptr)
{
    uint i =0;
    char ch;
    while(((ch =fgetc(fptr) )!= EOF ))      // fetching secret file data 
    {
        i++;
    }
    encode.sec_data = calloc(i, sizeof(char));          // allocating memory for secret data through calloc 
  
    rewind(fptr);
    i=0;
     while(((ch =fgetc(fptr) )!= EOF ))         // finding secret file data length
    {
       encode.sec_data[i] = ch;
       i++;
     
    }
    return i-1;                     // returning length of secret data
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
   
    printf("INFO : Copying Image Header\n");
    char str[54];
    int ret1 = fread(str, 54, 1, fptr_src_image);
    int ret2 = fwrite(str, 54, 1, fptr_dest_image);

    if(ret1 == ret2)
         return e_success;
     else
         return e_failure;
}

Status encode_magic_string_len( uint size, EncodeInfo *encInfo)
{
    printf("INFO : Encoding Magic String Length\n");
    if(encode_int_to_lsb(size, encInfo) == e_success)
         return e_success;
    else
    return e_failure;
   
}

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO : Encoding Magic String Signature\n");
 
    if(encode_string_to_lsb(magic_string, encInfo)==e_success)
        return e_success;
    else
        return e_failure;
}
/*encoding secret file extn Size*/
Status encode_secret_file_extn_size(uint file_size, EncodeInfo *encInfo)
{
    printf("INFO : Encoding %s Extension Length\n", encInfo -> secret_fname);
  
    if(encode_int_to_lsb(file_size, encInfo) == e_success)
        return e_success;
    else
        return e_failure;
}

/* Encoding secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
     printf("INFO : Encoding %s Extension\n", encInfo -> secret_fname);
  
    if(encode_string_to_lsb(file_extn, encInfo)==e_success)
        return e_success;
    else
        return e_failure;
}
/*Encoding secret file data size*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO : Encoding %s File Data Size\n", encInfo -> secret_fname);
    if(encode_int_to_lsb(file_size, encInfo)== e_success)
        return e_success;
    else
        return e_failure;
}

/*Encoding secret file Data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO : Encoding %s File Data\n", encInfo -> secret_fname);
    if(encode_string_to_lsb(encode.sec_data, encInfo) == e_success)
        return e_success;
    else
        return e_failure;

}
/*Copying remaining Data*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("INFO : Copying Left Over Data\n");

    unsigned char ch;
    int i=0;
   while( fread(&ch, 1,1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    
    encode.src_off_set = ftell(fptr_src);
    encode.dest_off_set = ftell(fptr_dest);
    if(encode.dest_off_set == encode.dest_off_set)
        return e_success;
    else 
        return e_failure;
}
/*encoding integer to data to source file data*/
Status encode_int_to_lsb(uint data, EncodeInfo *encInfo)
{
   
    char pass[32];

    
    
    fread(pass, 32, 1, encInfo->fptr_src_image);          // fetching 32 bytes from source file to encode Integer and storing in pass buffer

    encode.src_off_set = ftell(encInfo->fptr_src_image);        // finding source off_set position

     for(int i=0;i<32;i++)
    {
        pass[i] = (pass[i] & (~1)) |  ((data & (1<<i)) >> i);
    }
  
    
    fwrite(pass, 32, 1, encInfo->fptr_stego_image);        // writing integer Encoded 32 bytes to the stego_img file from buffer pass

    encode.dest_off_set = ftell(encInfo->fptr_stego_image);      // finding dest_off-set position


    if(encode.dest_off_set == encode.src_off_set)     // based on both off_set position sending return value
        return e_success;                    // 1. if both off_set at same pos then encoding of int done successfully
    else
        return e_failure;                    // 2. else both off sets at different pos means encoding of int not done properly
         
   
}
/*encoding string  data to source file data*/
Status encode_string_to_lsb(const char *data, EncodeInfo *encInfo)
{
    
    int size = strlen(data);     // finding length of collecting  string from other functions
    int i=0, j;
    char str[8];
    
    
    for(i=0;i<size;i++)
    {
        fread(str,8, 1, encInfo->fptr_src_image);           // reading 8 bytes from source file and storing in str buffer

        for(j=0;j<8;j++)
        {
            str[j] = (str[j] & (~1)) | ((data[i] & (1<<j)) >> j);
           
        }
            fwrite(str, 8, 1, encInfo->fptr_stego_image);      // writing 8 bytes of encoded data to destination file from str buffer

    }
     encode.src_off_set =  ftell(encInfo->fptr_src_image);   // finding source off_set position

    encode.dest_off_set = ftell(encInfo->fptr_stego_image);  // finding dest off_set position

    if(encode.dest_off_set == encode.src_off_set)
        return e_success;
    else
        return e_failure;
}







