#include <stdio.h>
#include <string.h>
#include "fileio.h"
struct file_info_st get_file_info();
int main(){
    int option;
    struct file_info_st file_info;
    do{
        printf("Enter your choice.\n1.Encrypt the File.\n2.Decrypt the file.\n3.Exit.\n");
        while(scanf("%d",&option)!=1){printf("Invalid Choice.\nEnter 1,2 or 3 only\n1.Encrypt the File.\n2.Decrypt the file.\n3.Exit.\n");while(getchar()!='\n');}
        while(getchar()!='\n');
        switch (option)
        {
        case 1:
            file_info = get_file_info();
            break;

        case 2:
            file_info = get_file_info();
            break;
        case 3:
            printf("Program will Exit. Press any Key to Continue.");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }while(option!=3);
getchar();
return 0;
}
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