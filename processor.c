#include "processor.h"
#include "fileio.h"
struct file_info_st get_file_info(){
    struct file_info_st file_info;
    printf("Enter the input File Path.\n");
    fgets(file_info.filepath_in,256,stdin);
    file_info.filepath_in[strcspn(file_info.filepath_in,"\n")]='\0';
    printf("Enter the output File Path.\n");
    fgets(file_info.filepath_out,256,stdin);
    file_info.filepath_out[strcspn(file_info.filepath_out,"\n")]='\0';
    printf("Enter Password.\n");
    fgets(file_info.password,128,stdin);
    file_info.password[strcspn(file_info.password,"\n")]='\0';
    return file_info;
}

void encrypt(){


}

void decrypt(){

}