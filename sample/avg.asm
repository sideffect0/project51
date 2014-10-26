org{
	mov dptr,#4501h
	mov r1,#05h
	mov b,#00h
	mov r0,#41h
loop:
	movx a,@dptr
	clr c
	add a,b
	mov b,a
	jnc cry
cry:
	inc dptr
	djnz r1,loop
	mov a,b
	mov b,#05h
	div ab
	mov dptr,#4600h
	movx @dptr,a
}end

start_mem{
	4501:01h
	4502:02h
	4503:03h
	4504:04h
	4505:05h
}end_mem