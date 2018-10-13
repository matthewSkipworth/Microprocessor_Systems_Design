
	AREA program, CODE, READONLY
	EXPORT __main
	ENTRY
__main

	LDR r0, =src
	LDR r10, =dst
	mov r2, #1; counter
	mov r3, #41; number of chars in the string.
	LDRB r1, [r0]; load 1 byte of r0 into r1

loop1	
	cmp r2, r3
	beq loop2
	
	push {r1}
	add r4, r0, r2; find byte in rom
	add r2, r2, #1
	LDRB r1, [r4]; load 1 byte of r0 into r1
	cmp r1, #20
	bne loop1
	
	
loop2
	
	pop {r5}
	add r6, r10, r7 ;find address in ram
	strb r5, [r6];
	sub r2, r2, #1
	cmp r2, #1
	add r7, r7, #1
	bne loop2
	
	
stop B stop ;; end main

src DCB "Welcome to Microprocessor Systems Course."	

	ALIGN

    AREA    Strings, DATA, READWRITE
		

dst DCB "Second string - destination"

	ALIGN
	END