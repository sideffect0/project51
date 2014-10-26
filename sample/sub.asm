
org {

mov 45h,#064h
mov a,#45h
cjne @a,#05h,endg
mov b,#055h
endg:

cpl p0.2
setb p0.7

setb c
mov a,#56h
mov r3,#67h
addc a,r3
;da
mov a,#32h
;swap a

mov r0,#45h
xcgd a,@r0

mov a,#05h
mov b,#02h
mul ab
mul ab
} end
