; A boot sector that prints a string using a BIOS routine
mov ah, 0x0e          ; TTY mode / scrolling teletype function
mov al, 'H'
int 0x10              ; Call BIOS video interrupt
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

infinite_loop:
    jmp infinite_loop   ; Hang here so the screen doesn't clear

; Pad to 512 bytes with zeros and add the standard BIOS boot signature
times 510-($-$$) db 0
dw 0xaa55
