dseg segment
fre dw 262,294,330,262,262,294,330,262      
    dw 330,349,392,330,349,392,392,440
    dw 392,349,330,262,392,440,392,349
    dw 330,262,294,196,262,294,196,262
time dw 10 dup(2500)
     dw 5000,2500,2500,5000,4 dup(1250),2500,2500
     dw 4 dup(1250),4 dup(2500),5000,2500,2500,5000
output1 db 'two tigers',0ah,0dh,'$'
output2 db 'end',0ah,0dh,'$'
dseg ends
;---------------------------------------
cseg segment
	 assume cs:cseg,ds:dseg
main proc far
	 org 100h
	 mov ax,dseg
	 mov ds,ax
	 lea dx,output1
	 mov ah,09h
	 int 21h
	 lea di,fre
	 lea bx,time
	 mov cx,32
new_shot:
	 push cx
	 call shoot
	 add di,2
	 add bx,2
	 mov cx,4000h
silent:
	 loop silent
	 pop cx
	 loop new_shot
	 mov al,48h
	 out 61h,al
	 lea dx,output2
	 mov ah,09h
	 int 21h
	 mov ah,4ch
	 int 21h
	 ret
main endp
;------------------------------------
shoot proc near
	 in al,61h
	 and al,11111100B
sound:
	 xor al,2
	 out 61h,al
	 push ax
	 push bx
	 mov ax,2801
	 mov bx,50
	 mul bx
	 div WORD ptr [di]
	 mov dx,ax
	 pop bx
	 pop ax
	 mov cx,dx
wait1:
	 loop wait1
	 dec WORD ptr [bx]
	 jnz sound
	 and al,11111100b
	 out 61h,al
	 ret
shoot endp
;---------------------------
cseg ends
    end main
	