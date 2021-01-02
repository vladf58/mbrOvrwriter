;----------------------------------------------;
; We will be using nasm with x86 instructions- available on most machines.
; nasm -f bin asmCode.asm -o data.bin
;
; Repository: https://github.com/d12/Assembly-8086-NASM-Snake/blob/master/snake.asm
;
;----------------------------------------------;
 
;Snake!
[BITS 16]

TOTAL_SEGMENTS equ 0x09

section .bss
  x_coord   RESW TOTAL_SEGMENTS ; [x_coord] is the head, [x_coord+2] is the next cell, etc.
  y_coord   RESW TOTAL_SEGMENTS ; Same here
  t1        RESB 2
  t2        RESB 2
  enabled   RESB 2
  x_apple   RESB 2
  y_apple   RESB 2
  
section  .text
  global_start 

_start:
  CALL SetVideoMode
  CALL SetInitialCoords
  CALL ClearScreen
  CALL Debug
  CALL ListenForInput

SetVideoMode:
  MOV AH, 0x00
  MOV AL, 0x13
  INT 0x10
  RET

ClearScreen:
  MOV CX, 0x00
  MOV DX, 0x00
  MOV AL, 0x00
  MOV BH, 0x00
  MOV AH, 0x0C
  .x_loop_begin
   MOV CX, 0x00
   .y_loop_begin
    INT 0x10
    INC CX
    CMP CX, 0x140
    JNAE .y_loop_begin
   .y_loop_end
   INC DX
   CMP DX, 0xFF
   JNAE .x_loop_begin
  .x_loop_end
  RET 

SetInitialCoords:
  MOV AX, 0x0F ; Initial x/y coord
  MOV BX, 0x00
  MOV DX, TOTAL_SEGMENTS
  ADD DX, DX

  .initialize_loop_begin
   MOV [x_coord+BX], AX
   MOV [y_coord+BX], AX
   ADD BX, 0x02
   CMP BX, DX
   JNE .initialize_loop_begin
  
  MOV AX, 0x00
  MOV [t1]       , AX
  MOV [t2]       , AX
  MOV AX, 2
  MOV [enabled]  , AX

  CALL RandomNumber
  MOV [x_apple], AX
  CALL RandomNumber
  MOV [y_apple], AX
  RET

ListenForInput:  ;Repeatedly check for keyboard input
  MOV AH, 0x00 ; Set AH to 0 to lock when listening for key
  MOV AL, 0x00 ; Set last key to 0
  INT 0x16   ; Listen for a keypress, save to register AL
  
  CALL InterpretKeypress

  CALL ListenForInput
  RET

InterpretKeypress:
  CMP AL, 0x77
  JE .w_pressed

  CMP AL, 0x61
  JE .a_pressed

  CMP AL, 0x73
  JE .s_pressed

  CMP AL, 0x64
  JE .d_pressed
  CALL Debug

  RET ; Invalid keypress, start listening again

  .w_pressed
  MOV AX, [x_coord]
  MOV BX, [y_coord]
  DEC BX
  JMP .after_control_handle

  .a_pressed
  MOV AX, [x_coord]
  MOV BX, [y_coord]
  DEC AX
  JMP .after_control_handle

  .s_pressed
  MOV AX, [x_coord]
  MOV BX, [y_coord]
  INC BX
  JMP .after_control_handle
  
  .d_pressed
  MOV AX, [x_coord]
  MOV BX, [y_coord]
  INC AX
 
  .after_control_handle
  MOV [t1], AX
  MOV [t2], BX
  CALL CheckAppleCollision
  CALL ShiftArray
  CALL DrawSnake
  CALL DrawApple
  RET

CheckAppleCollision:
  CMP AX, [x_apple]
  JNE .no_collision

  CMP BX, [y_apple]
  JNE .no_collision
  
  MOV AX, [enabled]
  INC AX
  MOV [enabled], AX
  
  CALL RandomNumber
  MOV [x_apple], AX
  CALL RandomNumber
  MOV [y_apple], AX

  .no_collision
  RET

DrawApple:
  MOV CX, [x_apple]
  MOV DX, [y_apple]
  MOV AL, 0x0C
  CALL DrawPixel
  RET

DrawSnake:
  CALL ClearScreen
  MOV BX, 0x00
  MOV AL, 0x0A
  MOV [t1], BX
  .draw_snake_loop_begin
   CMP [enabled], BX
   JBE .skip
   MOV [t1], BX
   ADD BX, BX
   MOV CX, [x_coord+BX]
   MOV DX, [y_coord+BX]
   CALL DrawPixel
   MOV BX, [t1]
   INC BX
   JMP .draw_snake_loop_begin
  
  .skip
  RET

ShiftArray:
  MOV BX, TOTAL_SEGMENTS
  DEC BX
  ADD BX, BX
  .loop_begin
   ADD BX, -2
   MOV DX, [x_coord+BX]
   MOV CX, [y_coord+BX]
   ADD BX, 2
   MOV [x_coord+BX], DX
   MOV [y_coord+BX], CX
   ADD BX, -2
   CMP BX, 0x00
   JNE .loop_begin
  MOV DX, [t1]
  MOV [x_coord], DX
  MOV DX, [t2]
  MOV [y_coord], DX
  RET

DrawPixel:
  MOV AH, 0x0C     ; Draw mode
  MOV BH, 0x00     ; Pg 0
  INT 0x10         ; Draw
  RET

RandomNumber:
  RDTSC
  AND EAX, 0xF
  RET

Debug:
  MOV AL, 0x0A
  MOV CX, 0x00
  MOV DX, 0x00
  CALL DrawPixel
  RET

TIMES 510 - ($ - $$) db 0  ;$ Fill the rest of the sector with 0
DW 0xAA55      ;Add boot signature at the end of bootloader