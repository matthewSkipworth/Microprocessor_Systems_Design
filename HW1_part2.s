Matthew Skipworth
TCES430: Homework1, part2
13 October, 2018

	AREA program, CODE, READONLY
	EXPORT __main
	ENTRY
__main

	LDR r0, =src
	LDR r10, =dst

	mov r2, #1; counter
	mov r9, #0; stack tracker
	mov r3, #41; number of chars in the string.
	LDRB r1, [r0]; load 1 byte of r0 into r1

loop1
	;cmp r2, r3
	;beq stop

	push {r1}
	add r9, r9, #1; stack tracker...
	add r4, r0, r2; find byte in rom
	add r2, r2, #1;
	LDRB r1, [r4]; load 1 byte of r0 into r1
	cmp r1, #46
	beq period
	cmp r1, #32
	bne loop1; branch if the char is a space.
period
	add r8, r2, #0
	beq loop2;


loop2

	pop {r5}
	sub r9, r9, #1
	add r6, r10, r7 ;find address in ram
	strb r5, [r6];
	sub r8, r8, #1
	add r7, r7, #1

	cmp r9, #0
	bne loop2

	cmp r1, #46
	beq add_period

	cmp r9, #0
	add r6, r10, r7
	strb r1, [r6]

	add r4, r0, r2; find byte in rom
	add r2, r2, #1
	LDRB r1, [r4]; load 1 byte of r0 into r1
	add r7, r7, #1

	beq loop1

add_period

	add r6, r10, r7
	strb r1, [r6]

stop B stop ;; end main

src DCB "Welcome to Microprocessor Systems Course. "

	ALIGN

    AREA    Strings, DATA, READWRITE


dst DCB "Second string - destination"

	ALIGN
	END
