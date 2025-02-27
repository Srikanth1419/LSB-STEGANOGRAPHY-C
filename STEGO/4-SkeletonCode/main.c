#include <stdio.h>
#include "encode.h"
#include "Decode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;

    DecodeInfo decInfo;

     if (argc == 1)
    {
        if (!(strcmp(argv[0], "./a.out")))
        {
            printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
            printf("Decoding : ./a.out -d <.bmp file>  [output file]\n");
        }
        return 0;
    }
    Status option = check_operation_type(argv);

    if (option == e_encode) // user wants to perform encode operation
    {

        if (argc == 2)
        {
            printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
            return 0;
        }

        printf("SUCCESS : check_operation function is completed\n");

        printf("-------User Selected ENCODE Operation-------\n");
        Status ret = read_and_validate_encode_args(argc, argv, &encInfo);
        if (ret == e_success)
        {
            printf("SUCCESS : read_and_validate fun is successfully done\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("SUCCESS : Encoding Done Successfully\n");
                
            }
            else
            {
                 printf("FAILURE : Encode Function is Failed\n");
            }
        }
        else
        {
            printf("ERROR : read_and_validate function is Failed\n");
        }
    }
    else if (option == e_decode) // user wants to perform Decode Operation
    {
        
        if (argc == 2)
        {
            printf("Decoding : ./a.out -d <.bmp file>  [output file]\n");
            return 0;
        }
        
        printf("SUCCESS : check_operation function is completed\n");
        printf("-------User Selected Decode Operation-------\n");

        if(read_and_validate_decode_args( argc, argv, &decInfo) == d_success)
        {
            printf("SUCCESS : read_and_validate fun is successfully done\n");
            
            if(do_decoding(&decInfo) == d_success)
                printf("SUCCESS : Decoding Done Successfully\n");
            else
                printf("FAILURE : Decode Function is Failed\n");
                
        }
        else
        {
            printf("ERROR : read_and_validate Function is Failed\n");
        }
    }
    else
    {
        printf("ERROR : Your operation should be -e or -d\n");
        printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
        printf("Decoding : ./a.out -d <.bmp file>  [output file]\n");
        return 0;
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(argv[2]  != NULL)
    printf("INFO : Checking Operation Type\n");
    if (argv[1] != NULL)
    {
        if (!(strcmp(argv[1], "-e")))
        {

            return e_encode;
        }
        else if (!(strcmp(argv[1], "-d")))
        {

            return e_decode;
        }
        else
        {

            return e_unsupported;
        }
    }
}
/*validating CLA for Encoding*/
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    char *ptr;
    if (argv[2])
    {
        ptr = strstr(argv[2], ".bmp");
        if (ptr != NULL)
        {
            if (!(strcmp(ptr, ".bmp")))
            {
                encInfo->src_image_fname = argv[2];
            }
            else
            {
                printf("ERROR : User Not provided Right source File to Encode\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR : User Not provided Right Source File to Encode\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR : User Not provided Right source File to Encode\n");
        return e_failure;
    }
   

#if 1
   
    if (argv[3] != NULL)
    {
      
        ptr = strstr(argv[3], ".");
        if (ptr == NULL)
        {
            printf("ERROR : User Not provided Right Secret File to Encode\n");
            return e_failure;
        }
        else if(!(strcmp(ptr, ".")))
        {
            printf("ERROR : User Not provided Right Secret File to Encode\n");
            return e_failure;
           
        }
        else
        {
             encInfo->secret_fname = argv[3];            //Updating extension also
           
        }
    }
    else
    {
        printf("ERROR : User Not Provided Secret Secret file to Encode\n");
        return e_failure;
    }
#endif

    if (argv[4] != NULL)
    {
        ptr = strstr(argv[4], ".bmp");

        if (ptr != NULL)
        {
            if (!(strcmp(ptr, ".bmp")))
            {
                encInfo->stego_image_fname = argv[4];
            }
            else
            {
                printf("ERROR: Output File Mentioned with different Extension instead of .bmp\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR : Output File Mentioned without .bmp Extension\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO : Output File is not Mentioned. ");
        printf("Creating stego_img.bmp as Default File\n");
        encInfo->stego_image_fname = "stego_img.bmp";
    }
    return e_success;
}

/*Validating CLAs for Decoding  Opreation*/

Result read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo) 
{
    char *ptr;

    if(argv[2] != NULL)
    {
        ptr = strstr(argv[2], ".bmp");
       
        if(ptr != NULL)
        {
          
            if(strcmp(ptr, ".bmp"))
            {
                printf("ERROR : User Not Provided Right Source File to Decode\n");
                return d_failure;
            }
            else
            {
                decInfo -> stego_fname = argv[2];
            }
        }
        else
        {
            printf("ERROR : User Not Provided Right Source File to Decode\n");
            return d_failure;
        }
    }
    else
    {
        printf("ERROR : User Not Provided Right Source File to Decode\n");
        return d_failure;
    }

    if(argv[3] != NULL)
    {
        strcpy(decInfo -> output_fname,argv[3]);
    }
    else
    {
        printf("INFO : Creating Default Output File\n");
        strcpy(decInfo -> output_fname,"default");
    }
    return d_success;
   
}