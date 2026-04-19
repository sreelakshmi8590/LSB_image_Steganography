#include<stdio.h>
#include<string.h>
#include"types.h"
#include"decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        printf(".bmp is present\n");
        decInfo->stego_image_fname=argv[2];
    }
    else
    {
        printf(".bmp is not present\n");
        return e_failure;
    }
    if(strstr(argv[3],".txt")!=NULL)
    {
        printf(".txt is present\n");
        decInfo->out_image_fname=argv[3];
    }
    else
    {
        printf(".txt is not present\n");
        return e_failure;
    }
    
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo)==e_success)
    {
        printf("All files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened");
        return e_failure;
    }
    
    if(decode_magic_string(decInfo)==e_success)
    {
        printf("Magic string decoded successful\n");
    }
    else
    {
        printf("Magic string not decoded successful\n");
        return e_failure;
    }
    if(decode_lsb_to_size(&decInfo->size_secret_extn, decInfo)==e_success)
    {
        printf("Size extension successfully\n");
    }
    else
    {
        printf("Size extension not successfully\n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo->file_extn,decInfo)==e_success)
    {
        printf("secret file extension decoding successfully\n");
    }
    else
    {
        printf("secret file extension decoding un-successfully\n");
        return e_failure;
    }
    if(decode_secret_file_size(&decInfo->size_secret_file,decInfo)==e_success)
    {
        printf("Size of secret file is decoded successfull\n");
    }
    else
    {
        printf("Size of secret file is not decoded successfull\n");
        return e_failure;
    }
    if (decode_secret_file_data(decInfo) == e_success)
        printf("Secret file data decoded successfully\n");
    else
    {
        printf("Secret file data decoding failed\n");
        return e_failure;
    }
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
   decInfo-> fptr_stego_image=fopen(decInfo->stego_image_fname,"r");
   if(decInfo-> fptr_stego_image==NULL)
   {
        printf("Stego image file is not present \n");
        return e_failure;

   }
   decInfo-> fptr_out_image=fopen(decInfo->out_image_fname,"w");
   if(decInfo-> fptr_out_image==NULL)
   {
        printf("Output file is not present \n");
        return e_failure;
   }
   return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
    
    printf("Enter the magic string:");
    scanf(" %[^\n]",decInfo->magic_string);
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    if(decode_data_from_image(decInfo->decode_string,strlen(decInfo->magic_string),decInfo)==e_success)
    {
        if(strcmp(decInfo->decode_string,decInfo->magic_string)==0)
            printf("Magic string is decoded\n");
        else
        {
            printf("Magic string is not decoded\n");
            return e_failure;
        }
    }
    
    return e_success;
}
Status decode_data_from_image(char *data, int size,DecodeInfo *decInfo)
{
    char data1[size+1];
    for(int i=0;i<size;i++)
    {
        fread(decInfo->image_data,8,1,decInfo->fptr_stego_image);
        decode_lsb_to_byte(&data1[i],decInfo->image_data);
    }
    data1[size] = '\0';
    strcpy(data, data1);
    return e_success;
}

Status decode_lsb_to_byte(char *data,char *image_buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
        ch=ch|((image_buffer[i]&1)<<i);
    }
    *data=ch;
    return e_success;
}
Status decode_lsb_to_size(long int *size,DecodeInfo *decInfo)
{
    char buffer[32];
    unsigned long int data=0;
    fread(buffer,32,1,decInfo->fptr_stego_image);
    for(int i=0;i<32;i++)
    {
        data=data|((buffer[i]&1)<<i);
    }
    *size=data;
    
    return e_success;
}
Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo)
{
    if(decode_data_from_image(decInfo->extn_secret_file,decInfo->size_secret_extn,decInfo)==e_success)
    {
        
        printf("Successful\n");
        return e_success;
    }
    else
    {
        printf("un-Successful\n");
        return e_failure;
    }
}
Status decode_secret_file_size(long *file_size,DecodeInfo *decInfo)
{
    char buffer[32];
    char size=0;
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    for(int i=0;i<32;i++)
    {
        size =size| (buffer[i] & 1) << i;
    }
    *file_size=size;
    printf("%d",*file_size);
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[decInfo->size_secret_file];
    if(decode_data_from_image(buffer, decInfo->size_secret_file, decInfo)==e_success)
    {
        fwrite(buffer, decInfo->size_secret_file, 1, decInfo->fptr_out_image);
        return e_success;
    }
    else
        return e_failure;
    return e_success;
}