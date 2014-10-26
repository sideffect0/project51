       
start_mem{
;00:05h
}end_mem
org{

setb rs0

setb p1.0
setb p1.1
setb p1.2
setb p1.3
setb p1.4
setb p1.5
setb p1.6
setb p1.7

setb a
mov a,#05h

add a,#ffh
;xcg a,@b
}end




