mov r1,#4
lsl r1,#16
ldr r0,=0x20200000
str r1,[r0,#4]
mov r2,#1
lsl r2,#16
mov r8,#0x1f
lsl r8,#20
Del4s:
sub r8,r8,#1
cmp r8,#0
bne Del4s
str r1,[r0,#4]
mov r7,#0x1f
lsl r7,#19
Del3s:
sub r7,r7,#1
cmp r7,#0
bne Del3s
str r2,[r0,#40]
mov r6,#0x1f
lsl r6,#20
Del2s:
sub r6,r6,#1
cmp r6,#0
bne Del2s
str r2,[r0,#28]
mov r5,#0x1f
lsl r5,#21
Del1s:
sub r5,r5,#1
cmp r5,#0
bne Del1s
str r2,[r0,#40]
mov r9,#0x1f
lsl r9,#22
delay:
sub r9,r9,#1
cmp r9,#0
bne delay
str r2,[r0,#28]
andeq r0,r0,r0
