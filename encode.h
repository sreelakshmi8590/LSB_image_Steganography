#include<stdio.h>
#include<string.h>
#include"types.h"
#include"encode.h"

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        printf(".bmp is present\n");
        encInfo->src_image_fname=argv[2];//jst storing the files in the structure.
    }
    else
    {
        printf(".bmp is not present\n");
        return e_failure;
    }
    if(strstr(argv[3],".txt")!=NULL)
    {
        printf(".txt is present\n");
        encInfo->secret_fname=argv[3];
    }
    else
    {
        printf(".txt is not present\n");
        return e_failure;
    }
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL)
        {
            printf(".bmp is present\n");
            encInfo->stego_image_fname=argv[4];
        }
        else
        {
            printf("stego.bmp is not present\n");
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname="stego.bmp";//create default output filename "stego.bmp".
    }
    return e_success;   
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("All files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened");
        return e_failure;
    }
    if(check_capacity(encInfo)==e_success)
    {
        printf("Check capacity is successfull\n");
    }
    else 
    {
        printf("Check capacity is un-successfull\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("Header copied successfull\n");
    }
    else
    {
        printf("Header not copied successfull\n");
        return e_failure;
    }

    if(encode_magic_string(encInfo)==e_success)
    {
        printf("Magic string encoded successful\n");
    }
    else
    {
        printf("Magic string not encoded successful\n");
        return e_failure;
    }
    
    char *ext = strchr(encInfo->secret_fname, '.');
    int size = strlen(ext);
    if(encode_size_to_lsb(size,encInfo)==e_success)
    {
        printf("Size extension encoded successfull\n");
    }
    else
    {
        printf("Size extension not encoded successfull\n");
        return e_failure;
    }
     if(encode_secret_file_extn(strchr(encInfo->secret_fname,'.'),encInfo)==e_success)
    {
        printf("Secrect file encoded successfully\n");
    }
    else
    {
        printf("Secrect file not encoded successfully\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
    {
        printf("Size of secret file is successfull\n");
    }
    else
    {
        printf("Size of secret file is not successfull\n");
        return e_failure;
    }
    if(encode_secret_file_data(encInfo)==e_success)
    {
        printf("Secret file data is encode successfull\n");
    }
    else
    {
        printf("Secret file data is not encode successfull\n");
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("Remained data copied\n");
    }
    else
    {
        printf("Remained data not copied\n");
        return e_failure;
    }
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
   encInfo-> fptr_src_image=fopen(encInfo->src_image_fname,"r");
   if(encInfo-> fptr_src_image==NULL)
   {
        printf("Source file is not present \n");
        return e_failure;

   }

   encInfo-> fptr_secret=fopen(encInfo->secret_fname,"r");
   if(encInfo-> fptr_secret==NULL)
   {
        printf("Secret file is not present \n");
        return e_failure;

   }

   encInfo-> fptr_stego_image=fopen(encInfo->stego_image_fname,"w");//write mode
   if(encInfo-> fptr_stego_image==NULL)
   {
        printf("Secret file is not present \n");
        return e_failure;

   }
   return e_success;
}
Status check_capacity(EncodeInfo *encInfo)//check whether the capacity for srce imge to hide secret file.
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo-> fptr_src_image);//total bytes of pixel data
    encInfo->size_secret_file=get_file_size(encInfo-> fptr_secret);//to know the size of secret file

    printf("Enter the magic string:");
    scanf(" %[^\n]",encInfo->magic);
    if(encInfo->image_capacity > ((strlen(encInfo->magic)*8)+32+32+32+(encInfo->size_secret_file*8)))
        return e_success;
    else
    {
        printf("Image capacity failed\n");
        return e_failure;
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
uint get_image_size_for_bmp(FILE *fptr_image)//to find how mny bytes of pixel data available in bmp
{
    uint width,height;
    fseek(fptr_image,18,SEEK_SET);//y 18?18->21=>width(4bytes)  
    fread(&width,4,1,fptr_image);//fread-> read the exact binary byte fle to buffer.y size->4=>Number of bytes to read
    //printf("width-> %d\n",width);
    fread(&height,4,1,fptr_image);
    //printf("height-> %d\n",height);

    return width*height*3;//3=>R(1 byte)+G(1 byte)+B(1 byte)
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)//to create an output image(stego)
{
    rewind(fptr_src_image);//read the first 54 bytes from the original image
    char buffer[54];//BMP image always has a 54-byte header.
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);//Write the same 54 bytes into the new stego image.binry data frm buffer to file
    return e_success;
}
//this is a key part of LSB steganography.
Status encode_magic_string(EncodeInfo *encInfo)
{
    if(encode_data_to_image(encInfo->magic,strlen(encInfo->magic),encInfo)==e_success)
    {
        printf("Magic string is encoded\n");
    }
    else
    {
        printf("Magic string is not encoded\n");
        return e_failure;
    }
    return e_success;
}

Status encode_data_to_image(char *data, int size,EncodeInfo *encInfo)//data=>It points to the first byte of the data we want to hide in the image.
{
    for(int i=0;i<size;i++)
    {
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i]&0xFE) | ((data &(1<<i))>>i);
    }
    return e_success;
}

//hide the size of the secret file inside the image.
Status encode_size_to_lsb(long size,EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    for(int i=0;i<32;i++)
    {
        buffer[i]=((buffer[i] & 0xFE) |((size &(1<<i))>>i));
    }
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo)==e_success)
    {
        printf("Successful\n");
        return e_success;
    }
    else
    {
        printf("un-Successful\n");
        return e_failure;
    }
    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_size_to_lsb(file_size,encInfo)==e_success)
        return e_success;
    else
        return e_failure;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(buffer,encInfo->size_secret_file,encInfo);
    return e_success;
    
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src))
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;    
}