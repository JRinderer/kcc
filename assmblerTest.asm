global	_start

	section	.txt
_start:		nop										;main PROC
			push			rbp
			mov				rbp,rsp
			push			3						;emit numbers
			pop				qword [A]				;print assignments
			push			2						;emit numbers
			pop				qword [B]				;print assignments
			push			qword [A]				;print identifiers
			push			qword [B]				;print identifiers
			pop				rbx						;print addition ops
			pop				rax
			add				rax,rbx					;adds the contents of rax and rbx
			push			rax
			push			qword [A]				;print identifiers
			pop				rbx						;print multiplcation/division
			pop				rax
			mov				rdx,0					;move a 0 into rdx
			imul			rax,rbx					;multiply the two register
			push			rax
			pop				qword [E]				;print assignments
			push			qword [A]				;print identifiers
			pop				rsi
			mov				rax,0

;BSS Section
 
			section				.bss
 
A:			resq			1
B:			resq			1
E:			resq			1
 
 
