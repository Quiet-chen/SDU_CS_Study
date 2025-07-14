;PROGRAM TITLE GOES HERE--HANOI
;Solves tower of HANOI puzzle.Printout sequence of moves 
;of N discs from initial spindle X to final spindle Z. 
;Using spindle Y for temporery storage. 
;****************************************************
datarea segment					;define data segment 
flag		dw		0
constant	dw		10000,1000,100,10,1
pgsize		dw		?
buf_size	db		80
s_buf		db		?
buf			dw		200 dup(?)
cur			dw		?
handle		dw		?
mess_getname	db		0dh,0ah,"           PLEASE INPUT FILENAME : $"
mess_err1		db		0ah,0dh,"           ILLEGAL FILENAME ! $"
mess_err2		db		0ah,0dh,"			 FILE NOT FOUND ! $"
mess_err3		db		0ah,0dh,"			 FILE READ ERROR ! $"
FILE			db		"OUTPUT.TXT",0
mess_err4		db		0ah,0dh,"			 FILE CREATE ERROR ! $"
tmpdw			dw		?
tmpdb			db		?
datarea ends
;*********************************************
prognam segment					;define code segment 
;----------------------------------------------
main 	proc far 
		assume cs:prognam,ds:datarea
start: 
;set up stack for return 
		push 	ds 
		sub 	ax,ax 
		push 	ax 
;set DS register to current data segment 
		mov 	ax,datarea 
		mov 	ds,ax 
		mov		cur,10
		call 	getline
		call 	openf
		or		ax,ax
		jnz		display
		mov		dx,offset mess_err2
		mov		ah,09h
		int		21h
		jmp		FILE_END
display:
		call	read_block
		or		ax,ax
		jnz		next2
		mov		dx,offset mess_err3
		mov		ah,09h
		int 	21h
		jmp		FILE_END
next2:
		mov		dx,offset FILE
		mov		cx,0
		mov		ah,3ch
		int 	21h
		jc		error4
		mov		handle,ax
		mov		bx,buf[4]
		mov		bh,0h
		mov		si,bx
		mov		bx,buf[6]
		mov		bh,0h
		mov		di,bx
		mov		bx,buf[0]
		sub		bx,3030h
		mov		bh,0h
		mov		cx,buf[2]
		mov		ch,0h
		call	hanoi
		jmp		FILE_END
error4:
		mov		dx,offset mess_err4
		mov		ah,09
		int 	21h
FILE_END:
		mov		bx,handle
		mov		ah,3eh
		int		21h
		ret
main endp 
;
hanoi proc near					;define subprocedure
;Solves tower of HANOI puzzle. 
;Argement:(BX)=N,(CX)=X,(SI)=Y,(DI)=Z. 
		cmp 	bx,1 			;if N=1,execute basis 
		je 		basis 
		call 	save 			;save N,X,Y,Z 
		dec 	bx 
		xchg 	si,di 
		call 	hanoi 			;call HANOI(N-1,X,Z,Y) 
		call 	restor 			;restore N,X,Y,Z 
		call 	print			;print XNZ 
		dec 	bx 
		xchg 	cx,si 
		call 	hanoi			;call HANOI(N-1,Y,X,Z) 
		jmp 	return 
basis: 	call 	print 			;print X1Z 
return: ret 					;return 
hanoi endp 						;end subprocedure
;-----------------------------------------------
print proc near 
;print XNZ 
		push	bx
		mov		bx,handle
		push	cx
		mov		tmpdw,cx
		mov		cx,1
		mov		dx,offset tmpdw
		mov		ah,40h
		int		21h
		pop		cx
		pop		bx
		call	binidec
		push 	bx
		push	cx
		push	dx
		mov		cx,1
		mov		bx,handle
		mov		tmpdw,di
		mov		dx,offset tmpdw
		mov		ah,40h
		int		21h
		mov		cx,1
		mov		bx,handle
		mov		tmpdb,0ah
		mov		dx,offset tmpdb
		mov		ah,40h
		int 	21h
		mov		cx,1
		mov		bx,handle
		mov		tmpdb,0dh
		mov		dx,offset tmpdb
		mov		ah,40h
		int 	21h
		pop 	dx
		pop		cx
		pop		bx
		ret
print endp 
;--------------------------------------------
save proc near 
;push N,X,Y,2 onto stack 
		pop 	bp 
		push	bx 
		push 	cx 
		push 	si 
		push 	di 
		push 	bp 
		ret 
save endp 
;--------------------------------------------
restor proc near
;pop Z,Y,X,N from stack 
		pop 	bp 
		pop 	di 
		pop 	si 
		pop 	cx 
		pop 	bx 
		push 	bp 
		ret 
restor endp
;------------------------------------------------
decibin proc near
;procedure to convert decimal on keybd to binary.
;result is left in BX register. 
		mov 	bx,0				;clear BX for number 
;get digit from keyboard,convert to birary 
newchar: 
		mov 	ah,1 				;keyboard input 
		int 	21h					;call DOS 
		sub 	al,30h				;ASCII to binary 
		jl 		exit1 				;jump if<0 
		cmp 	al,9d				;isit>9d£¿ 
		jg 		exit1				;yes,not dec digit 
		cbw 						;byte in AL to word in AX
;(digit is now in AX) 
;multiply number in BX by 10 decimal. 
		xchg 	ax,bx				;trade digit &number 
		mov 	cx,10d 				;put 10 dec in CX 
		mul 	cx					;number times 10 
		xchg 	ax,bx				;trade number &digit
;add digit in AX to number in BX 
		add 	bx,ax 				;add digit to number 
		jmp 	newchar 			;get next digit 
exit1: 	ret							;return from decibin 
decibin endp						;end of decibin proc 
;-------------------------------------------------
openf proc near
		push	bx
		push	cx
		push	dx
		mov		dx,offset buf
		mov		al,0
		mov		ah,3dh
		int		21h
		mov		handle,ax
		mov		ax,1
		jnc		ok
		mov		ax,0
ok:
		pop 	dx
		pop		cx
		pop		bx
		ret
openf	endp
getline proc near
		push	ax
		push 	bx
		push	cx
		push	dx
		mov		dx,offset mess_getname
		mov		ah,09h
		int		21h
		mov		dx,offset buf_size
		mov		ah,0ah
		int     21h
		call	crlf
		mov		bl,s_buf
		mov		bh,0
		mov		[buf+bx],0
		pop		dx
		pop		cx
		pop		bx
		pop		ax
		ret
getline	endp
read_block	proc near
		push	bx
		push	cx
		push	dx
		cmp		cur,10
		jnz		back
		mov		cx,10
		mov		bx,handle
		mov		dx,offset buf
		mov		ah,3fh
		int		21h
		mov		cur,0
		mov		ax,1
		jnc		back
		mov		cur,10
		mov		ax,0
back:
		pop		dx
		pop		cx
		pop		bx
		ret
read_block endp
;----------------------------------------------------
binidec proc near 
;procedure to convert binary number in BX to decimal 
;on console screen
		push 	bx 
		push 	cx 
		push 	si 
		push 	di 
		mov 	flag,0 
		mov 	cx,5 
		lea 	si,constant 
;
dec_div: 
		mov 	ax,bx				;number high half 
		mov 	dx,0				;zero out low half 
		div 	word ptr[si]		;divide by contant 
		mov 	bx,dx				;remainder into BX 
		mov 	dl,al				;quotient into DL 
;
		cmp 	flag,0 				;have not leading zero 
		jnz 	print1 
		cmp 	dl,0 
		je 		skip 
		mov 	flag,1 
;print the contents of DL on screen 
print1: 
		push	bx
		push	cx
		mov		bx,handle
		mov		cx,1
		add 	dl,30h				;convert to ASCII 
		mov		tmpdb,dl
		mov		dx,offset tmpdb
		mov 	ah,40h				;display function 
		int 	21h					;call DOS
		pop		cx
		pop		bx
skip: 
		add 	si,2 
		loop 	dec_div 
		pop 	di 
		pop 	si 
		pop 	cx 
		pop 	bx 
		ret							;return from dec_div 
binidec endp 
;----------------------------------------------
crlf proc near 
;print carriage return and linefeed 
		mov 	dl,0ah				;linefeed 
		mov 	ah,02h				;display function 
		int 	21h 
;
		mov 	dl,0dh 				;carriage return 
		mov 	ah,02h				;display function 
		int 	21h 
;
		ret 
crlf endp 
;--------------------------------------------
prognam ends						;end of code segment 
;****************************************
end start							;end assembly