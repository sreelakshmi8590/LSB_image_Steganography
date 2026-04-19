#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main(int argc,char *argv[])
{
    if(check_operation_type(argv)==e_encode)//to know wheather user want encoding or decoding
    {
        printf("You have choosed encoding\n");

        EncodeInfo encInfo;//to store files like source image, secret file, output stego image.
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)//Validates whether those files exist and are correct.
        {
            printf("read and validate successfully\n");
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding is successfull\n");
            }
            else
            {
                printf("Encoding is un-successfull\n");
                return e_failure;
            }
        }
        else
        {
            printf("Encoding is un-successfull\n");
            return e_failure;
        }    
    }
    else if(check_operation_type(argv)==e_decode)
    {
        printf("You have choosed decoding\n");

        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo)==e_success)//Validates whether those files exist and are correct.
        {
            printf("read and validate successfully\n");
            if(do_decoding(&decInfo)==e_success)
            {
                printf("Decoding is successfull\n");
            }
            else
            {
                printf("Decoding is un-successfull\n");
                return e_failure;
            }

        }
        else
        {
            printf("Pass correct arguments\n");
            printf("./a.out -e beutiful.bmp secret.txt-->for encoding\n");
            printf("./a.out -d stego.bmp-->for decoding\n");
        }
    }
    
    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else
        return e_unsupported;
}
