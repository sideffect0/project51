org {

mov a,#01h
mov b,#04h
inc b
in:
mul ab
djnz b,in

} end
