DATA 	SEGMENT 
	PGSIZE 		DW 	?
	BUF_SIZE 	DB  80
	S_BUF 		DB	?
	BUF 		DB	200 DUP(?)						
  	CUR 		DW 	?
	HANDLE 		DW	?								
	MESS_GETNAME 	DB 		0DH, 0AH,"           PLEASE INPUT FILENAME : $"
	MESS_ERR1 		DB		0AH, 0DH,"           ILLEGAL FILENAME ! $"
	MESS_ERR2 		DB      0AH, 0DH,"			 FILE NOT FOUND ! $"
	MESS_ERR3 		DB 		0AH, 0DH,"			 FILE READ ERROR ! $"
	MESS_PSIZE		DB 	    0AH, 0DH,"			 PAGE SIZE : $ "
	CRLF			DB      0AH, 0DH,"$"
	MESS_STAR 		DB 		0AH, 0DH,"*****************************************"
					DB		0AH, 0DH,"$"
DATA	ENDS
CODE 	SEGMENT
		ASSUME DS :DATA ,CS :CODE
MAIN	PROC 	FAR
START:
		PUSH	DS
		SUB 	AX, AX
		PUSH	AX
		MOV		AX, DATA
		MOV		DS, AX
		MOV		PGSIZE, 12					;Each page 12 lines
		MOV		CUR, 200					;File data buffer is empty
		CALL 	GETLINE						;Get file name
		CALL 	OPENF						;open the file,(ax)=0 means no such file						
		OR		AX, AX						
		JNZ		DISPLAY									
		MOV 	DX, OFFSET MESS_ERR2
		MOV		AH, 09H
		INT		21H										;(ax)=0, no such file
		JMP		FILE_END
DISPLAY:
		MOV		CX, PGSIZE								
SHOW_PAGE:
		CALL	READ_BLOCK								
		OR		AX, AX						
		JNZ		NEXT2									

		MOV		DX, OFFSET MESS_ERR3					
		MOV		AH, 09H;(ax)=0;error in read
		INT		21H
		JMP		FILE_END
NEXT2:	
		CALL	SHOW_BLOCK								;display a line in buf, (bx) returned 0
														;means that the file reach its end
		OR		BX, BX									
		JZ		FILE_END								;(bx)=0:at the end of file
		OR		CX, CX
		JNZ		SHOW_PAGE								;(cx)<>0:not the last line of a page				
		MOV		DX, OFFSET MESS_STAR					
		MOV		AH, 09H
		INT		21H;at the end of a page,print a line of *
;the current page has been on creen,and followed by a line of stars
;the following part get the command from keyboard
WAIT_SPACE:												
		MOV		AH, 1
		INT		21H
		CMP		AL, " "
		JNZ		PSIZE									
		JMP		DISPLAY
PSIZE:
		CMP		AL, "P"									
		JNZ		WAIT_SPACE
		CALL	CHANGE_PSIZE							
HERE:
		MOV		AH, 1
		INT		21H
		CMP		AL, " "
		JNZ		HERE									;stick here to wait for space
		JMP		DISPLAY

FILE_END:
		RET
MAIN	ENDP


CHANGE_PSIZE	PROC	NEAR							
		PUSH	AX
		PUSH	BX
		PUSH	CX
		PUSH	DX
		MOV		DX, OFFSET MESS_PSIZE					
		MOV		AH, 09H
		INT		21H										;print the promt line
		MOV		AH, 01
		INT		21H										;get the new num of page size					
		CMP		AL, 0DH									
		JZ		ILLEG
		SUB		AL, "0"						
		MOV		CL, AL
GETP:
		MOV		AH, 1
		INT		21H
		CMP		AL, 0DH
		JZ		PGOT
		SUB		AL, "0"
		MOV		DL, AL
		MOV		AL, CL
		MOV		CL, DL								;exchange al and cl			
		MOV		BL, 10
		MUL		BL
		ADD		CL, AL
		JMP		GETP

PGOT:
		MOV		DL, 0AH
		MOV		AH, 2
		INT		21H									;output 0ah to complete the return
		CMP		CX, 0
		JLE		ILLEG
		CMP		CX, 24									
		JG		ILLEG
		MOV		PGSIZE, CX							;be sure the new page size in (0..24) region
ILLEG:
		MOV		DL, 0AH
		MOV		AH, 2
		INT		21H									;output 0ah to complete the return
		POP		DX
		POP 	CX
		POP		BX
		POP		AX
		RET
CHANGE_PSIZE	ENDP

OPENF	PROC	NEAR									
		PUSH	BX
		PUSH	CX
		PUSH	DX
		MOV		DX, OFFSET BUF
		MOV		AL, 0
		MOV		AH, 3DH					     			
		INT		21H
		MOV		HANDLE, AX				
		MOV		AX, 1
		JNC		OK
		MOV		AX, 0
OK:
		POP		DX
		POP		CX
		POP		BX
		RET
OPENF	ENDP
GETLINE	PROC	NEAR
		PUSH	AX
		PUSH	BX
		PUSH	CX
		PUSH	DX
		MOV		DX, OFFSET MESS_GETNAME
		MOV		AH, 09H
		INT		21H										;promt user to input file name
		MOV		DX, OFFSET BUF_SIZE						
		MOV		AH, 0AH									;function call of buffer input
		INT		21H										

		MOV		DX, OFFSET CRLF							
		MOV		AH, 09H
		INT		21H;return

		MOV		BL, S_BUF								
		MOV		BH, 0
		MOV		[BUF+BX], 0						       ;put 0 into the end of file name
		POP		DX
		POP		CX
		POP		BX
		POP		AX
		RET
GETLINE 	ENDP

READ_BLOCK	PROC	NEAR								
		PUSH	BX
		PUSH	CX
		PUSH	DX
		CMP		CUR, 200								
		JNZ		BACK
;if no more chars in buf can be displayed
;then read another 200 chars
		MOV		CX, 200
		MOV		BX, HANDLE								
		MOV		DX, OFFSET BUF
		MOV		AH, 3FH									
		INT		21H										
		MOV		CUR, 0											
		MOV		AX, 1
		JNC		BACK
		MOV		CUR, 200
		MOV		AX, 0
BACK:
		POP		DX
		POP		CX
		POP		BX
		RET
READ_BLOCK	ENDP

SHOW_BLOCK	PROC	NEAR								 
		PUSH	AX
		PUSH	DX
		MOV		BX, CUR
LOOP1:
		CMP		BX, 200									
		JL 		LP
		JMP		EXIT									;if buf is empty then return
LP:
		MOV		DL, BUF[BX]								;else show the current char
;		CMP		DL, 1AH									;search the file end
		CMP     DL, '$'
		JZ		EXIT_EOF
		INC		BX										
		INC		CUR
		MOV		AH, 02
		INT		21H
		CMP		DL, 0AH								   
		JZ		EXIT_IN									;if the char shown is return
														;then exit.a line has been on screen	   
		JMP		LOOP1
EXIT_EOF:
		MOV		BX, 0
EXIT_IN:
		DEC		CX						
EXIT:
		POP		DX
		POP		AX
		RET
SHOW_BLOCK	ENDP

CODE	ENDS
END 	START