dseg segment
input	label  byte
 maxlen    db  101;最大长度
 actuallen db  0;实际长度
 counter   db  101 dup(?);字符串
b_num 	db	4 dup(0);大写字母个数
s_num   db  4 dup(0);小写字母个数
d_num   db  4 dup(0);数字个数
dseg ends
;-----------------------------
cseg segment
	 assume cs:cseg,ds:dseg
main proc far
	mov 	ax,dseg
	mov 	ds,ax	;初始化
	mov 	dx,offset maxlen
	mov 	ah,0ah
	int 	21h		;0ah输入字符串
	mov		al,actuallen;实际长度，[0,ax-1]
	xor		ah,ah
	mov		si,0	;当前字符下标
	dec		ax		;下标最大值
start:
	cmp 	counter[si],'0'
	jge		digit	;>=0
if_loop:
	inc		si
	cmp		si,ax
	jg		output	;si>ax，读到末尾了，输出各个计数
	jmp		start	;否则，接着判断当前字符是什么
digit:
	cmp		counter[si],'9'
	jle		isdigit	;0-9
	cmp		counter[si],'A'
	jge		big		;>=A
	jmp		if_loop	;啥都不是
isdigit:
	inc		d_num
	jmp		if_loop
big:
	cmp		counter[si],'Z'
	jle		isbig	;A-Z
	cmp		counter[si],'a'
	jge		smallone;>=a
	jmp		if_loop	;啥都不是
isbig:
	inc		b_num
	jmp		if_loop
smallone:
	cmp		counter[si],'z'
	jle		issmall	;a-z
	jmp 	if_loop	;啥都不是
issmall:
	inc		s_num
	jmp		if_loop
output:
	call	crlf
	mov		al,s_num;
	xor		ah,ah
	call	s_to_asc
	lea		dx,s_num
	mov		ah,09h
	int 	21h
	call 	crlf
	mov		al,b_num;先输出大写字母个数,因为只有一个字节的数，所以放到al，ah用0填充
	xor		ah,ah
	call	b_to_asc;转成asc码，输出字符串
	lea		dx,b_num
	mov 	ah,09h
	int		21h
	call	crlf
	mov		al,d_num
	xor		ah,ah
	call	d_to_asc
	lea		dx,d_num
	mov		ah,09h
	int		21h
	mov		ah,4ch
	int		21h
	ret
main endp
;------------------------------------
b_to_asc proc near
	mov		si,0	;b_num[0],[1],[2],最多三位数,si记录当前下标
	mov		di,0    ;di标记是否是首0
	cmp		al,0
	jz		thezero	;al==0，说明该类字符一个都没有，0加个结束符就能输出
	mov		cx,100d	;最多是三位数也就是100
	call	thediv_b
	mov		cx,10d
	call	thediv_b
	mov		cx,1d
	call	thediv_b
thend:
	mov		b_num[si],'$'
	ret
thezero:
	mov		b_num[0],'0'
	mov		b_num[1],'$'
	ret
b_to_asc endp
;-----------------------------------
thediv_b proc near
	mov 	dx,0
	div		cx		;dx余数，ax商
	add		ax,'0'
	mov		bx,dx	;余数放到bx
	cmp		al,'0'
	jz		iffirst ;如果是0，要看首0不输出
	mov		b_num[si],al 
	inc		si
	mov		di,1	;如果得到不是0的数，di=1
	jmp		thenext
iffirst:
	cmp		di,1	;di是1说明前面已经有数了要填0
	jnz		thenext	;di=0跳过
	mov		b_num[si],'0'
	inc		si		;自动进入下一段thenext
thenext:
	mov		ax,bx
	ret
thediv_b endp
;------------------------------------
s_to_asc proc near
	mov		si,0	;s_num[0],[1],[2],最多三位数,si记录当前下标
	mov		di,0    ;di标记是否是首0
	cmp		al,0
	jz		thezero_s	;al==0，说明该类字符一个都没有，0加个结束符就能输出
	mov		cx,100d	;最多是三位数也就是100
	call	thediv_s
	mov		cx,10d
	call	thediv_s
	mov		cx,1d
	call	thediv_s
thend_s:
	mov		s_num[si],'$'
	ret
thezero_s:
	mov		s_num[0],'0'
	mov		s_num[1],'$'
	ret
s_to_asc endp
;-----------------------------------
thediv_s proc near
	mov 	dx,0
	div		cx		;dx余数，ax商
	add		ax,'0'
	mov		bx,dx	;余数放到bx
	cmp		al,'0'
	jz		iffirst_s ;如果是0，要看首0不输出
	mov		s_num[si],al 
	inc		si
	mov		di,1	;如果得到不是0的数，di=1
	jmp		thenext_s
iffirst_s:
	cmp		di,1	;di是1说明前面已经有数了要填0
	jnz		thenext_s	;di=0跳过
	mov		s_num[si],'0'
	inc		si		;自动进入下一段thenext
thenext_s:
	mov		ax,bx
	ret
thediv_s endp
;------------------------------------
d_to_asc proc near
	mov		si,0	;d_num[0],[1],[2],最多三位数,si记录当前下标
	mov		di,0    ;di标记是否是首0
	cmp		al,0
	jz		thezero_d	;al==0，说明该类字符一个都没有，0加个结束符就能输出
	mov		cx,100d	;最多是三位数也就是100
	call	thediv_d
	mov		cx,10d
	call	thediv_d
	mov		cx,1d
	call	thediv_d
thend_d:
	mov		d_num[si],'$'
	ret
thezero_d:
	mov		d_num[0],'0'
	mov		d_num[1],'$'
	ret
d_to_asc endp
;-----------------------------------
thediv_d proc near
	mov 	dx,0
	div		cx		;dx余数，ax商
	add		ax,'0'
	mov		bx,dx	;余数放到bx
	cmp		al,'0'
	jz		iffirst_d ;如果是0，要看首0不输出
	mov		d_num[si],al 
	inc		si
	mov		di,1	;如果得到不是0的数，di=1
	jmp		thenext_d
iffirst_d:
	cmp		di,1	;di是1说明前面已经有数了要填0
	jnz		thenext_d	;di=0跳过
	mov		d_num[si],'0'
	inc		si		;自动进入下一段thenext
thenext_d:
	mov		ax,bx
	ret
thediv_d endp
;------------------------------------
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
;--------------------------------------
cseg ends
	 end main