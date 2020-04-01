.code

nativecall proc  p1:qword, p2:qword, p3:qword, p4:qword, callee:qword
	call callee
	ret
nativecall endp

end