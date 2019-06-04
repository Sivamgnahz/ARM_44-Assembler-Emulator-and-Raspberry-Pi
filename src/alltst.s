mov r2, r1
add r3,r1,r2
mul r3,r1,r2
add r2,r1,#2
str r1,[r2],r4
mla r3,r1,r2,r4
sub r5,r4, r3, lsr r2
str r1,[r2]
str r1,[r0,#8]
ldr r3,[r1,#-0x4]
str r1,[r2,r4]
ldr r5,[r1,r2,lsr #2]
str r1,[r2],r4
add r3,r1,r2
mov r1,#0x20200000
mov r1,#4
mov r1,#14
mov r1,#-0x04
mov r1,#0x0F
mov r1,#0x20200000
ldr r0,=0x20200004
ldr r3,=0xE
foo:
wait:
wait1:
b wait
b foo
beq wait1
andeq r0,r0,r0
