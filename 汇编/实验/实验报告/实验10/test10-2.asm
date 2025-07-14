dseg segment
dseg ends
cseg segment
     assume cs:cseg,ds:dseg
main proc far
		mov ax,dseg
		mov ds,ax
		mov cl,0
		xor ch,ch
		mov dl,21H
		xor dh,dh
row:	
		call show
		call crlf
		inc cx
		cmp cx,11
		jl row
		mov ah,4ch
		int 21h
		ret
main endp
;--------------------------
show proc near
		mov al,0
		xor ah,ah
col:	push ax
		mov ah,02h	;输出dl
		int 21h
		pop ax
		push dx
		push ax
		mov dl,0	;输出空格
		mov ah,02h
		int 21h
		pop ax
		pop dx
		inc ax
		inc dx
		cmp ax,20
		jl col
		ret
show endp
;-------------------------
crlf proc near
	push	dx
	push	ax
	mov		dl,0ah	;回车换行
	mov		ah,02h
	int 	21h
	mov		dl,0dh
	mov		ah,02h
	int		21h
	pop		ax
	pop		dx
	ret
crlf endp
;-----------------------------
cseg ends
	 end main