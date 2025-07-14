dseg segment
thename label byte
maxn   db    13
actn   db    ?
names  db    13 dup(?)    ;��������
;
thetel label byte
maxt   db    9
actt   db    ?
tels   db	 9 dup(?)			 ;����绰
;
output_nam db   'Input name:','$'
output_tel db   'Input a telephone number:','$'
output_ask db   'Do you want a telephone number?(Y/N)','$'
output_ask_name  db  'name?','$'
output_title     db  'name                tel.',0ah,0dh,'$'
;
tel_tab db    20 dup(12 dup(?),8 dup(?),8 dup(?))	;�绰����ÿһ��20���ֽڣ��������12������8���ֽڣ����8���ֽڵ绰����
;
search_name		db    12 dup(?)						;���ҵ�����
;
step	  dw	29				;������28���ֽ�Ϊһ�������29Ϊ����Ѱ���������ÿһ��ĵ�һλ
;
temp      db    12 dup(?),8 dup(?),8 dup(?),'$'		;������ʱ�洢һ���ʽ����tel_tab��28���ֽ�+'$'
si_temp	  dw    0									;���ڱ��潻����������±�λ�õ�ַ
di_temp   dw    0									;dw������Ϊ��ַ��һ����
;
dseg ends
;--------------------------------------------
cseg segment
	 assume cs:cseg,ds:dseg
main proc far
	 mov ax,dseg
	 mov ds,ax
	 mov es,ax		;rep��ʱ����õ�
	 call input_n	;bxһֱ��ľ���n
	 mov cx,bx
	 mov di,0
start:
	 call init_space
	 lea  dx,output_nam
	 mov  ah,09h
	 int  21h		;���������ʾ
	 call input_name;��������
	 call crlf
	 lea  dx,output_tel
	 mov  ah,09h
	 int  21h		;����绰��ʾ
	 call input_tel	;����绰
	 call crlf
	 call stor		;�洢���ֺ͵绰���绰��
	 inc  di
	 loop start
ask:
	 lea  dx,output_ask
	 mov  ah,09h
	 int  21h		;�����ʾ��ѯ���Ƿ��ѯ
	 mov  ah,01h
	 int  21h		;����һ���ַ�Y/N
	 cmp  al,'Y'
	 jne  printline ;����Y����ת���
	 call crlf
	 lea  dx,output_ask_name
	 mov  ah,09h
	 int  21h		;�����ʾ�У�������
	 lea  dx,thename
	 mov  ah,0ah
	 int  21h		;��������
	 call crlf
	 call find_name	;����
	 call crlf
	 jmp  ask		;����ѯ�ʣ�ֱ��N
printline:
	 call crlf
	 call sort_name	;����
	 mov  cx,bx		;��ʼ���������������,cx=n
	 lea  dx,tel_tab
theloop:
	 mov  ah,09h
	 int  21h
	 call crlf
	 add  dx,29
	 loop theloop
	 ;
	 mov  ah,4ch
	 int  21h		;������
	 ret
main endp
;--------------------------------------
input_n proc near
	 mov bx,0
put_n:
	 mov ah,01h
	 int 21h		;����һ���ַ�
	 sub al,30h		;ascתint
	 cmp al,0
	 jl  exit1		;0-9֮����ַ���˵���������
	 cmp al,9
	 jg  exit1
	 cbw			;�з�������չ��������λ���ah
	 ;
	 xchg ax,bx		;xchg���������Ĵ�����ֵ
	 mov cx,10
	 mul cx			;bx��������е�λ����ÿ��bx*10+�����ax����Ϊmulֻ������ax�����Խ���bx��ax
	 xchg ax,bx
	 add bx,ax
	 jmp put_n		;��������
exit1:
	 ret
input_n endp
;--------------------------------------
input_name proc near
	 push di
	 push cx
	 push ax
	 lea  dx,thename
	 mov  ah,0ah
	 int  21h 		;�����ַ���������
	 lea  si,names
	 lea  di,temp
	 mov  cl,actn
	 xor  ch,ch
	 cld			;cld,���������־��ʹsi,diÿ�ζ�����;std����,si,diÿ�εݼ�
	 rep  movsb		;��Ч�����ַ�����DS:SI��ES:DI,cx���ܹ����ƶ��ٸ��ַ�
	 pop  ax
	 pop  cx
	 pop  di
	 ret
input_name endp
;--------------------------------------
input_tel proc near
	 push di
	 push cx
	 push ax
	 lea  dx,thetel
	 mov  ah,0ah
	 int  21h		;�����ַ������绰
	 lea  si,tels
	 lea  di,temp
	 add  di,20		;�绰��temp+20��ʼ��
	 mov  cl,actt
	 xor  ch,ch
	 cld
	 rep  movsb
	 pop  ax
	 pop  cx
	 pop  di
	 ret
input_tel endp
;--------------------------------------
stor proc near
	 push ax
	 push bx
	 push cx
	 mov  ax,di     ;��ǰ�浽�ڼ�����
	 push di
	 lea  si,temp
	 lea  di,tel_tab
	 mov  cx,29
	 mul  cx		;tel_tab+di*29���ǵ�ǰҪ���λ��
	 add  di,ax
	 mov  cl,29
	 xor  ch,ch
	 rep  movsb		;temp�浽tel_tab��Ӧ��λ����
	 pop  di
	 pop  cx
	 pop  bx
	 pop  ax
	 ret
stor endp
;--------------------------------------
find_name proc near
	 push ax
	 push cx
	 push si
	 push di
	 lea  di,search_name
	 mov  al,' '
	 mov  cx,12
	 rep  stosb		;��ʼ��search_name
	 mov  cl,actn
	 xor  ch,ch
	 lea  si,names
	 lea  di,search_name
	 rep  movsb		;����������ָ�ʽ��������Ƚ�
	 mov  cx,bx     ;�ܹ�n������
	 lea  si,search_name
	 lea  di,tel_tab
search:
	 push cx
	 push di
	 push si
	 xor  ch,ch
	 mov  cl,12
	 repz  cmpsb	;���ϱȽϣ�ֱ���в���ȵ��ַ�����
	 je   ok
	 pop  si
	 pop  di
	 pop  cx
	 add  di,29		;di��tel_tab����һ�鿪ͷ
	 loop search
ok:	 pop  si
	 pop  di
	 pop  cx
output_teltab:
	 lea  dx,output_title	;�ҵ��ˣ����title��
	 mov  ah,09h
	 int  21h
	 mov  ax,bx
	 push bx
	 sub  ax,cx				;cx��n��ʼ�ݼ�ֱ���ҵ�����ôn-cx�����ҵ�������±�
	 mov  bx,29
	 mul  bx				;��Ӧλ�þ���tel_tab+(n-cx)*29
	 lea  dx,tel_tab
	 add  dx,ax
	 mov  ah,09h
	 int  21h				;�����Ӧ����
	 pop  bx
	 pop  di
	 pop  si
	 pop  cx
	 pop  ax
	 ret
find_name endp
;--------------------------------------
sort_name proc near
	 push bx
loop1:
	 dec  bx				;ð�����򣬴�ǰ����ð��ÿ�δӺ���ǰȷ��һ�����ֵ��bxΪ��ǰȷ�����±ֱ꣬��<=0
	 cmp  bx,0
	 jle  exit2
	 mov  ax,0
loop2:
	 cmp  ax,bx				;��ѭ����ax��0��ʼ����ð�ݣ�ֱ����bx��λ��-1����Ϊ���Ƚϵ���ax��ax+1�����±�
	 jge  loop1
	 call bubble
	 inc  ax
	 jmp  loop2
exit2:
	 pop  bx
	 ret
sort_name endp
;--------------------------------------
bubble proc near
	 lea  si,tel_tab
	 lea  di,tel_tab
	 push ax
	 mov  cx,29
	 mul  cx
	 add  si,ax				;si=tel_tab+ax*29
	 pop  ax
	 push ax
	 inc  ax
	 mul  cx
	 add  di,ax				;di=tel_tab+(ax+1)*29
	 pop  ax
	 ;
	 push di
	 push si
	 mov  si_temp,si
	 mov  di_temp,di
	 mov  cl,12
	 xor  ch,ch
	 rep  cmpsb				;�Ƚ�si��di
	 pop  si
	 pop  di
	 ja   swap				;���[ax]>[ax+1],��ô�������ڵ�������
	 jmp  exit3
swap:
	 call init_space		;�ȸ�ʽ��temp
	 push di
	 push si
	 mov  si,si_temp		;ͨ��temp���si��di�Ľ���
	 lea  di,temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;si_temp��temp
	 mov  si,di_temp
	 mov  di,si_temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;di_temp��si_temp
	 lea  si,temp
	 mov  di,di_temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;temp��di_temp
	 pop  si
	 pop  di
exit3:
	 ret
bubble endp
;--------------------------------------
init_space proc near
	 push di
	 push cx
	 push bx
	 push ax
	 lea  di,temp
	 mov  al,' '	;�ÿո��ʼ������ô֮��û�����ĵط������ǿո�
	 mov  cx,28
	 rep  stosb		;��al��ֵ��Ч��ʼ��ES��DI��ʼ��cx���ֽڣ����ж�cx�Ƿ����0��ִ�к����ָ�cx--�����ֱ��0���˳�
	 pop  ax
	 pop  bx
	 pop  cx
	 pop  di
	 ret
init_space endp
;--------------------------------------
crlf proc near
	 push ax
	 push bx
	 push cx
	 push dx
	 mov  dx,0dh
	 mov  ah,02h
	 int  21h
	 mov  dx,0ah
	 mov  ah,02h
	 int  21h
	 pop  dx
	 pop  cx
	 pop  bx
	 pop  ax
	 ret
crlf endp
;--------------------------------------
cseg ends
	 end  main