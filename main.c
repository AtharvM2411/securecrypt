#include <stdio.h>
#include <string.h>
#include "processor.h"
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