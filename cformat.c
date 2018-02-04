#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_format.h"

struct file_hdr *  
get_header_file(FILE * file, int tot_size ) {

    if (sizeof(struct file_hdr) > tot_size)
        return NULL;
    struct file_hdr * fhdr = (char*)malloc(sizeof(struct file_hdr));
    if (!fhdr) {
        printf("Can't allocate memory for input file\n");
        return NULL;
    }    
   
    return fhdr;
}

int 
exec(char * filename) {
    FILE * file;
    file = fopen(filename, "rb");
    if (!file) {
        printf("Can't open input file\n");
        return ERR_FILE_OPEN;
    }

    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);
    fseek(file, 0, SEEK_SET);



    struct file_hdr * fhdr = get_header_file(file, filesize);
    if (fhdr == NULL)
        return ERR_INVALID_HEADER_FILE;


    free (fhdr);
    return 0;
}