dseg segment
input	label  byte
 maxlen    db  101;��󳤶�
 actuallen db  0;ʵ�ʳ���
 counter   db  101 dup(?);�ַ���
b_num 	db	4 dup(0);��д��ĸ����
s_num   db  4 dup(0);Сд��ĸ����
d_num   db  4 dup(0);���ָ���
dseg ends
;-----------------------------
cseg segment
	 assume cs:cseg,ds:dseg
main proc far
	mov 	ax,dseg
	mov 	ds,ax	;��ʼ��
	mov 	dx,offset maxlen
	mov 	ah,0ah
	int 	21h		;0ah�����ַ���
	mov		al,actuallen;ʵ�ʳ��ȣ�[0,ax-1]
	xor		ah,ah
	mov		si,0	;��ǰ�ַ��±�
	dec		ax		;�±����ֵ
start:
	cmp 	counter[si],'0'
	jge		digit	;>=0
if_loop:
	inc		si
	cmp		si,ax
	jg		output	;si>ax������ĩβ�ˣ������������
	jmp		start	;���򣬽����жϵ�ǰ�ַ���ʲô
digit:
	cmp		counter[si],'9'
	jle		isdigit	;0-9
	cmp		counter[si],'A'
	jge		big		;>=A
	jmp		if_loop	;ɶ������
isdigit:
	inc		d_num
	jmp		if_loop
big:
	cmp		counter[si],'Z'
	jle		isbig	;A-Z
	cmp		counter[si],'a'
	jge		smallone;>=a
	jmp		if_loop	;ɶ������
isbig:
	inc		b_num
	jmp		if_loop
smallone:
	cmp		counter[si],'z'
	jle		issmall	;a-z
	jmp 	if_loop	;ɶ������
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
	mov		al,b_num;�������д��ĸ����,��Ϊֻ��һ���ֽڵ��������Էŵ�al��ah��0���
	xor		ah,ah
	call	b_to_asc;ת��asc�룬����ַ���
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
	mov		si,0	;b_num[0],[1],[2],�����λ��,si��¼��ǰ�±�
	mov		di,0    ;di����Ƿ�����0
	cmp		al,0
	jz		thezero	;al==0��˵�������ַ�һ����û�У�0�Ӹ��������������
	mov		cx,100d	;�������λ��Ҳ����100
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
	div		cx		;dx������ax��
	add		ax,'0'
	mov		bx,dx	;�����ŵ�bx
	cmp		al,'0'
	jz		iffirst ;�����0��Ҫ����0�����
	mov		b_num[si],al 
	inc		si
	mov		di,1	;����õ�����0������di=1
	jmp		thenext
iffirst:
	cmp		di,1	;di��1˵��ǰ���Ѿ�������Ҫ��0
	jnz		thenext	;di=0����
	mov		b_num[si],'0'
	inc		si		;�Զ�������һ��thenext
thenext:
	mov		ax,bx
	ret
thediv_b endp
;------------------------------------
s_to_asc proc near
	mov		si,0	;s_num[0],[1],[2],�����λ��,si��¼��ǰ�±�
	mov		di,0    ;di����Ƿ�����0
	cmp		al,0
	jz		thezero_s	;al==0��˵�������ַ�һ����û�У�0�Ӹ��������������
	mov		cx,100d	;�������λ��Ҳ����100
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
	div		cx		;dx������ax��
	add		ax,'0'
	mov		bx,dx	;�����ŵ�bx
	cmp		al,'0'
	jz		iffirst_s ;�����0��Ҫ����0�����
	mov		s_num[si],al 
	inc		si
	mov		di,1	;����õ�����0������di=1
	jmp		thenext_s
iffirst_s:
	cmp		di,1	;di��1˵��ǰ���Ѿ�������Ҫ��0
	jnz		thenext_s	;di=0����
	mov		s_num[si],'0'
	inc		si		;�Զ�������һ��thenext
thenext_s:
	mov		ax,bx
	ret
thediv_s endp
;------------------------------------
d_to_asc proc near
	mov		si,0	;d_num[0],[1],[2],�����λ��,si��¼��ǰ�±�
	mov		di,0    ;di����Ƿ�����0
	cmp		al,0
	jz		thezero_d	;al==0��˵�������ַ�һ����û�У�0�Ӹ��������������
	mov		cx,100d	;�������λ��Ҳ����100
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
	div		cx		;dx������ax��
	add		ax,'0'
	mov		bx,dx	;�����ŵ�bx
	cmp		al,'0'
	jz		iffirst_d ;�����0��Ҫ����0�����
	mov		d_num[si],al 
	inc		si
	mov		di,1	;����õ�����0������di=1
	jmp		thenext_d
iffirst_d:
	cmp		di,1	;di��1˵��ǰ���Ѿ�������Ҫ��0
	jnz		thenext_d	;di=0����
	mov		d_num[si],'0'
	inc		si		;�Զ�������һ��thenext
thenext_d:
	mov		ax,bx
	ret
thediv_d endp
;------------------------------------
crlf proc near
	push	dx
	push	ax
	mov		dl,0ah	;�س�����
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