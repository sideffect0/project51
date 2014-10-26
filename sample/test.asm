
org {

mov b,#04h
mov dptr,#1000h
mov r0,#40h
NEXT:
	movx a,@dptr	

	mov @r0,a
	inc dptr
	inc r0
	djnz b,NEXT

} end

start_mem {
1000:fh
1001:20h
1002:30h
1003:40h
} end_mem
