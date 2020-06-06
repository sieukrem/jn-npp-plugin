.code

nativecall proc  ;  iReg:qword, fReg:qword, callee:qword, stack:qword, stackSize:qword
	push rbp
	push rcx
	push rdx
	push r8
	push r9

	movsd xmm0, qword ptr [rdx]
	movsd xmm1, qword ptr [rdx+8]
	movsd xmm2, qword ptr [rdx+16]
	movsd xmm3, qword ptr [rdx+24]
	
	mov rax, r8

	mov rdx, qword ptr [rcx+8]
	mov r8,  qword ptr [rcx+16]
	mov r9,  qword ptr [rcx+24]
	mov rcx, qword ptr [rcx]

	call rax

	add rsp, 5*8
	ret
nativecall endp

end