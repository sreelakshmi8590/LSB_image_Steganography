#ifndef DECODE_H
#define DECODE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char decode_string[100];
    char magic_string[100];
    char file_extn[10];

    
    char *out_image_fname;
    FILE *fptr_out_image;
    char image_data[MAX_IMAGE_BUF_SIZE];
    uint bits_per_pixel;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    long size_secret_extn;

    

    //char image_data[8];
    
    
}DecodeInfo;

/*functions calling for decode*/

OperationType check_operation_type(char *argv[]);

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_decode_files(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size,DecodeInfo *decInfo);

Status decode_lsb_to_byte(char *data,char *image_buffer);

Status decode_lsb_to_size(long int *size,DecodeInfo *decInfo);

Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo);

Status decode_secret_file_size(long *file_size,DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);
#endif