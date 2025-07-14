dseg segment
thename label byte
maxn   db    13
actn   db    ?
names  db    13 dup(?)    ;输入名字
;
thetel label byte
maxt   db    9
actt   db    ?
tels   db	 9 dup(?)			 ;输入电话
;
output_nam db   'Input name:','$'
output_tel db   'Input a telephone number:','$'
output_ask db   'Do you want a telephone number?(Y/N)','$'
output_ask_name  db  'name?','$'
output_title     db  'name                tel.',0ah,0dh,'$'
;
tel_tab db    20 dup(12 dup(?),8 dup(?),8 dup(?))	;电话簿，每一项20个字节，名字最多12个，空8个字节，最后8个字节电话号码
;
search_name		db    12 dup(?)						;查找的名字
;
step	  dw	29				;数组里28个字节为一项，所以以29为步长寻找数组里的每一项的第一位
;
temp      db    12 dup(?),8 dup(?),8 dup(?),'$'		;用于临时存储一项格式化的tel_tab，28个字节+'$'
si_temp	  dw    0									;用于保存交换的两项的下标位置地址
di_temp   dw    0									;dw就是因为地址是一个字
;
dseg ends
;--------------------------------------------
cseg segment
	 assume cs:cseg,ds:dseg
main proc far
	 mov ax,dseg
	 mov ds,ax
	 mov es,ax		;rep的时候会用到
	 call input_n	;bx一直存的就是n
	 mov cx,bx
	 mov di,0
start:
	 call init_space
	 lea  dx,output_nam
	 mov  ah,09h
	 int  21h		;输出名字提示
	 call input_name;输入名字
	 call crlf
	 lea  dx,output_tel
	 mov  ah,09h
	 int  21h		;输出电话提示
	 call input_tel	;输入电话
	 call crlf
	 call stor		;存储名字和电话到电话簿
	 inc  di
	 loop start
ask:
	 lea  dx,output_ask
	 mov  ah,09h
	 int  21h		;输出提示行询问是否查询
	 mov  ah,01h
	 int  21h		;输入一个字符Y/N
	 cmp  al,'Y'
	 jne  printline ;不是Y，跳转输出
	 call crlf
	 lea  dx,output_ask_name
	 mov  ah,09h
	 int  21h		;输出提示行，问名字
	 lea  dx,thename
	 mov  ah,0ah
	 int  21h		;输入名字
	 call crlf
	 call find_name	;查找
	 call crlf
	 jmp  ask		;继续询问，直到N
printline:
	 call crlf
	 call sort_name	;排序
	 mov  cx,bx		;开始输出排序后的所有项,cx=n
	 lea  dx,tel_tab
theloop:
	 mov  ah,09h
	 int  21h
	 call crlf
	 add  dx,29
	 loop theloop
	 ;
	 mov  ah,4ch
	 int  21h		;结束符
	 ret
main endp
;--------------------------------------
input_n proc near
	 mov bx,0
put_n:
	 mov ah,01h
	 int 21h		;输入一个字符
	 sub al,30h		;asc转int
	 cmp al,0
	 jl  exit1		;0-9之外的字符，说明输入结束
	 cmp al,9
	 jg  exit1
	 cbw			;有符号数扩展，按符号位填充ah
	 ;
	 xchg ax,bx		;xchg交换两个寄存器的值
	 mov cx,10
	 mul cx			;bx存的是已有的位数，每次bx*10+输入的ax，因为mul只能用于ax，所以交换bx与ax
	 xchg ax,bx
	 add bx,ax
	 jmp put_n		;继续输入
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
	 int  21h 		;输入字符串，名字
	 lea  si,names
	 lea  di,temp
	 mov  cl,actn
	 xor  ch,ch
	 cld			;cld,消除方向标志，使si,di每次都递增;std设置,si,di每次递减
	 rep  movsb		;高效复制字符串，DS:SI到ES:DI,cx是总共复制多少个字符
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
	 int  21h		;输入字符串，电话
	 lea  si,tels
	 lea  di,temp
	 add  di,20		;电话在temp+20开始存
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
	 mov  ax,di     ;当前存到第几个了
	 push di
	 lea  si,temp
	 lea  di,tel_tab
	 mov  cx,29
	 mul  cx		;tel_tab+di*29就是当前要存的位置
	 add  di,ax
	 mov  cl,29
	 xor  ch,ch
	 rep  movsb		;temp存到tel_tab对应的位置上
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
	 rep  stosb		;初始化search_name
	 mov  cl,actn
	 xor  ch,ch
	 lea  si,names
	 lea  di,search_name
	 rep  movsb		;将输入的名字格式化，方便比较
	 mov  cx,bx     ;总共n个名字
	 lea  si,search_name
	 lea  di,tel_tab
search:
	 push cx
	 push di
	 push si
	 xor  ch,ch
	 mov  cl,12
	 repz  cmpsb	;不断比较，直到有不相等的字符出现
	 je   ok
	 pop  si
	 pop  di
	 pop  cx
	 add  di,29		;di到tel_tab的下一组开头
	 loop search
ok:	 pop  si
	 pop  di
	 pop  cx
output_teltab:
	 lea  dx,output_title	;找到了，输出title行
	 mov  ah,09h
	 int  21h
	 mov  ax,bx
	 push bx
	 sub  ax,cx				;cx从n开始递减直到找到，那么n-cx就是找到的项的下标
	 mov  bx,29
	 mul  bx				;对应位置就是tel_tab+(n-cx)*29
	 lea  dx,tel_tab
	 add  dx,ax
	 mov  ah,09h
	 int  21h				;输出对应的项
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
	 dec  bx				;冒泡排序，从前往后冒，每次从后往前确定一个最大值，bx为当前确定的下标，直到<=0
	 cmp  bx,0
	 jle  exit2
	 mov  ax,0
loop2:
	 cmp  ax,bx				;内循环，ax从0开始往后冒泡，直到到bx的位置-1，因为它比较的是ax，ax+1两个下标
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
	 rep  cmpsb				;比较si和di
	 pop  si
	 pop  di
	 ja   swap				;如果[ax]>[ax+1],那么交换相邻的这两项
	 jmp  exit3
swap:
	 call init_space		;先格式化temp
	 push di
	 push si
	 mov  si,si_temp		;通过temp完成si和di的交换
	 lea  di,temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;si_temp→temp
	 mov  si,di_temp
	 mov  di,si_temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;di_temp→si_temp
	 lea  si,temp
	 mov  di,di_temp
	 mov  cl,29
	 xor  ch,ch
	 cld
	 rep  movsb				;temp→di_temp
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
	 mov  al,' '	;用空格初始化，那么之后没有填充的地方都会是空格
	 mov  cx,28
	 rep  stosb		;用al的值高效初始化ES：DI开始的cx个字节，先判断cx是否等于0，执行后面的指令，cx--，最后直到0，退出
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