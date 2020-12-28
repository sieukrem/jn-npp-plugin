.code

nativecall proc  ;  fReg:qword rcx, callee:qword rdx, stack:qword r8, stackSize:qword r9
	push rbp
	push rcx
	push rdx
	push r8
	push r9
	push rdi
	push rsi
	push r13 ; 
	mov r13, rsp 

	mov r10, rcx

	sub rsp, r9

	cld
	mov rsi, r8 ; src
	mov rdi, rsp; dest
	mov rcx, r9 ; amount of bytes to copy
	rep movsb

	movsd xmm0, qword ptr [r10]
	movsd xmm1, qword ptr [r10+8]
	movsd xmm2, qword ptr [r10+16]
	movsd xmm3, qword ptr [r10+24]

    mov rax, rdx
	
	mov rcx, qword ptr [r10+32]
	mov rdx, qword ptr [r10+32+8]
	mov r8,  qword ptr [r10+32+16]
	mov r9,  qword ptr [r10+32+24]

	call rax

	mov rsp, r13
	pop r13
	pop rsi
	pop rdi
	add rsp, 5*8
	ret
nativecall endp

end