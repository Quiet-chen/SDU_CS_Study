TITLE SHOW_ERR--Display DOS function call error message
;Display a message based on an error code in AX
;All registers are preserved
;**********************************************************
DSEG     SEGMENT    PARA 'DATA'
  CR     EQU        13
  LF     EQU        10
  EOM    EQU        '$'
;
mess     DB          'error number?',CR,LF,EOM
monin     label      byte
  max     db         3
  act     db         ?
  ero     db         3 dup(?)
;
OUT_OF_RANGE DB     'Error code is not in valid range(1-83)'
             DB      CR,LF,EOM
RESERVED     DB      'Error code is reserved(36-79)',CR,LF,EOM
ER1      DB'Invalid function number',CR,LF,EOM
ER2      DB'File not found',CR,LF,EOM
ER3      DB'Path not found',CR,LF,EOM
ER4      DB'Too many open files',CR,LF,EOM
ER5      DB'Access denied',CR,LF,EOM
ER6      DB'Invalid handle',CR,LF,EOM
ER7      DB'Memory control block destroyed',CR,LF,EOM
ER8      DB'Insufficient memory',CR,LF,EOM
ER9      DB'Invalid memory block address',CR,LF,EOM
ER10     DB'Invalid environment',CR,LF,EOM
ER11     DB'Invalid format',CR,LF,EOM
ER12     DB'Invalid access code',CR,LF,EOM
ER13     DB'Invalid data',CR,LF,EOM
ER14     DB'No such message',CR,LF,EOM
ER15     DB'Invalid drive was specified',CR,LF,EOM
ER16     DB'Attempted to remove the current directory'
         DB CR,LF,EOM
ER17     DB'Not same device',CR,LF,EOM
ER18     DB'No more files',CR,LF,EOM
ER19     DB'Disk is write..protected',CR,LF,EOM
ER20     DB'Unknown unit',CR,LF,EOM
ER21     DB'Drive not ready',CR,LF,EOM
ER22     DB'Unknown command',CR,LF,EOM
ER23     DB'Data error (CRC)',CR,LF,EOM
ER24     DB'Bad request structure length',CR,LF,EOM
ER25     DB'Seek error',CR,LF,EOM
ER26     DB'Unknown media type',CR,LF,EOM
ER27     DB'Sector not found',CR,LF,EOM
ER28     DB'Printer out of paper',CR,LF,EOM
ER29     DB'Write fault',CR,LF,EOM
ER30     DB'Read fault',CR,LF,EOM
ER31     DB'General failure',CR,LF,EOM
ER32     DB'Sharing violation',CR,LF,EOM
ER33     DB'Lock violation',CR,LF,EOM
ER34     DB'Invalid disk change',CR,LF,EOM
ER35     DB'FCB unavailable',CR,LF,EOM
ER80     DB'File exists',CR,LF,EOM
ER81     DB'Reserved',CR,LF,EOM
ER82     DB'Cannot make',CR,LF,EOM
ER83     DB'Fail on INT 24',CR,LF,EOM
ERTAB1   DW ER1,ER2,ER3,ER4,ER5,ER6,ER7,ER8,ER9,ER10
         DW ER11,ER12,ER13,ER14,ER15,ER16,ER17,ER18
         DW ER19,ER20,ER21,ER22,ER23,ER24,ER25,ER26
         DW ER27,ER28,ER29,ER30,ER31,ER32,ER33,ER34
         DW ER35
ERTAB2   DW ER80,ER81,ER82,ER83
DSEG     ENDS
;****************************************************
CSEG     SEGMENT PARA 'CODE'
         ASSUME CS:CSEG,DS:DSEG,ES:DSEG
main      proc       far
          push       DS
          sub        AX,AX
          push       AX
;
          mov        AX,DSEG
          mov        DS,AX
          mov        ES,AX
;                    Input number;
;                    ------------
start:
         lea         dx,mess        ;显示提示语句
         mov         ah,09          ;09输出字符串
         int         21h
         lea         dx,monin
         mov         ah,0ah         ;获取输入字符串
         int         21h
         mov         dl,13          ;换行回车
         mov         ah,02          ;02输出字符
         int         21h
         mov         dl,10
         mov         ah,02
         int         21h
         cmp         act,0
         je          exit
;                    Convert ASCii to binary:
;                    -------------------------
         mov         ah,0
         cmp         act,2
         je          two
         mov         al,ero         ; 只有一个字符
         sub         al,30h         ; 转换为二进制
         jmp         SHOW_ERR
two:
         mov         al,ero   
         xor         ax,30h         ; 转换为二进制
         mov         bx,10
         mul         bx            ; 第一位乘以 10
         mov         bl,ero+1
         sub         bl,30h
         add         al,bl          ; 加上第二位
         jmp         SHOW_ERR
exit:    
         ret
;
SHOW_ERR PROC FAR
         PUSH     DS
         SUB      BX,BX
         PUSH     BX
;
         MOV      SI,DSEG        ;initialize DS
         MOV      DS,SI
;        
         PUSH     AX             ;save input error
         CMP      AX,83          ;check for error code in range
         JG       O_O_R
         CMP      AX,0
         JG       IN_RANGE
         jmp      start
O_O_R:   LEA      DX,OUT_OF_RANGE
         JMP      SHORT DISP_MSG
;Error code is valid,determine with table to use
IN_RANGE:
         CMP      AX,35          ;error code 1--35?
         JG       TRY79
         LEA      BX,ERTAB1      ;yes,POINTto ERTAB1
         DEC      AX
         JMP      FROM_ADDR
TRY79:
         CMP      AX,79          ;error code 36--79?
         JG       LAST_4
         LEA      DX,RESERVED    ;yes,display message
         JMP      DISP_MSG
LAST_4:
         LEA      BX,ERTAB2      ;error cpde 80--83
         AND      AX,3
FROM_ADDR:
         SHL      AX,1           ;point to correct offset
         ADD      BX,AX
         MOV      DX,[BX]        ;put message addr into DX
DISP_MSG:
         MOV      AH,9           ;display message string
         INT      21H
         POP      AX
         RET                     ;return to calling program
SHOW_ERR ENDP
;
CSEG     ENDS
;************************************************
         END      SHOW_ERR
         
         