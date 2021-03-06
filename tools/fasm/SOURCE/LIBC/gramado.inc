
;;
;;  ## gramado extern ##
;;

;;#importante:
;;aqui chamaremos várias funções em C disponíveis na libc do gramado 
;;e na api, além das outras que podemos criar exclusivamente para isso.
;;

;extrn 
;extrn
;extrn
;extrn
;;...



;;#todo: Não tá funcionando bem.	
;;IN: eax = x ebx = y	
gramado_set_cursor:
 
    push eax 
	push ebx 
    push ecx 
    push edx 
	
    mov dword [.x], eax 
	mov dword [.y], ebx
	
	mov eax, 34 
	mov ebx, dword [.x]
	mov ecx, dword [.y]
	xor edx, edx 
	
	;int 200
	int 0x80
	
	pop edx
	pop ecx 
	pop ebx 
	pop ebx 
	
	ret
	
.x: dd 0	
.y: dd 0	

;;=====================
;;reset cursor.	
gramado_reset_cursor:
    push eax 
	push ebx
	
	xor eax, eax 
    xor ebx, ebx 	
    call gramado_set_cursor

    pop ebx  
    pop eax 	
    ret
	
	
	
;;===================================
;; getchar	
;; OUT: eax = char  'al'	
;; pega um char digitado pelo teclado.
gramado_getchar:
    ;;push eax
	push ebx
    push ecx	
	push edx
    
	;;mov byte [.saveChar], al

	xor edx, edx	
	xor ecx, ecx 
	xor ebx, ebx	
	xor eax, eax   

	mov dl, 0       ; byte [.saveChar]
	mov cl, 0       ; byte [.saveChar]
	mov bl, 0       ; byte [.saveChar] 
	mov al, 137     ;número do serviço
	
    ;int 200
	int 0x80

	pop edx
	pop ecx
    pop ebx
    ;;pop eax	
	
	;;#importante 
	;;o retorno é um char em 'AL'.
	
    ret	
	
;;.saveChar:
    ;;db 0	
	

;;===================================
;; putchar	
;; IN: eax = char e, 'al'	
;;
gramado_putchar2:
    push eax
	push ebx
    push ecx	
	push edx
    
	mov byte [.saveChar], al

	xor edx, edx	
	xor ecx, ecx 
	xor ebx, ebx	
	xor eax, eax   

	mov dl, byte [.saveChar]
	mov cl, byte [.saveChar]
	mov bl, byte [.saveChar] 
	mov al, 65     ;número do serviço
	
    ;int 200
	int 0x80

	;xor edx, edx	
	;xor ecx, ecx 
	;xor ebx, ebx	
	;xor eax, eax   

	pop edx
	pop ecx
    pop ebx
    pop eax	
	
    ret	
	
.saveChar:
    db 0
	
	
;adaptando pois gramado_putchar usa 'al' e 	display_character usa 'dl'
gramado_putchar:
	push edx
    mov dl, al
	call display_character
	pop edx
	ret
	
;;=====================================
;; DisplayMessage
;; IN: esi =  ponteiro para string	
;;

DisplayMessage:
    lodsb                                    ; Load next character.
    or  al, al                               ; Test for NUL character.
    jz  .DONE
    
	;al = char que queremos exibir.
	mov ah, 0
	xor ebx, ebx 
	call gramado_putchar
	
	;mov  ah, 0x0E                            ; BIOS teletype.                           
    ;mov  bx, 0x0007                          ; Página e atributo.  
	;int  0x10                                ; Invoke BIOS.
    
	jmp  DisplayMessage
.DONE:
    ret	
	
;------------------------------------------------------------------
; print_spaces:
;      Write a variable number of spaces to the console.
;
; Arguments:
;   AL = number to spaces.
;
gramado_print_spaces:
    ;nop
;screen32PrintSpaces:	
    pushad
    xor  ecx, ecx
    mov  cl, al                  ; use CX as the loop counter
    mov  al, ' '             ; character to print
.loop:
    jecxz .exit
	
    ;push ecx
	call    gramado_putchar
    ;pop ecx
	
	loop    .loop
.exit:
    popad
    ret	



;--------------------------------------------------------------------------
; Write the byte value to the console in hex.
;
; Arguments:
;   AL = Value to be displayed in hex.
;   #bugbug: talvez seja eax e mostra um dword
;
gramado_print_hex:
    ;nop
;screen32PrintHex:	
    pushad
	XOR ECX, ECX 
    mov     cx, WORD 4
    bswap   eax
.looph:
    push    ax
    ror     al, 4
    
	call    gramado_print_nibble            ; display upper nibble
	pop     ax
    
	call    gramado_print_nibble            ; display lower nibble
	ror     eax, 8
    loop    .looph

    ;mov     al, 10                  ; carriage return
    ;call    print_char
    ;mov     al, 13
    ;call    print_char
    popad
    ret

gramado_print_nibble:
    and     al, 0x0f
    add     al, '0'
    cmp     al, '9'
    jna     .print_ascii
    add     al, 'A' - '9' - 1
.print_ascii:
    push eax 
	push ebx 
	push ecx 
	push edx
    call gramado_putchar
	pop edx 
	pop ecx 
	pop ebx 
	pop eax
	ret		
	
	
;--------------------------------------------------
; os_int_to_string:
;     Convert a binary integer into an string.
;
;  IN:	RAX = binary integer.
;	    RDI = location to store string.
;
; OUT:	RDI = points to end of string.
;
;	All other registers preserved.
;   Min return value is 0 and max return value is 
;   18446744073709551615 so the string needs to be 
;   able to store at least 21 characters 
;   (20 for the digits and 1 for the string terminator).
;   Adapted from http://www.cs.usfca.edu/~cruse/cs210s09/rax2uint.s
;
os_int_to_string:
	push edx
	push ecx
	push ebx
	push eax

	mov ebx, 10					; base of the decimal system
	xor ecx, ecx					; number of digits generated
os_int_to_string_next_divide:
	xor edx, edx					; RAX extended to (RDX,RAX)
	div ebx						; divide by the number-base
	push edx					; save remainder on the stack
	inc ecx						; and count this remainder
	cmp eax, 0					; was the quotient zero?
	jne os_int_to_string_next_divide		; no, do another division

os_int_to_string_next_digit:
	pop eax						; else pop recent remainder
	add al, '0'					; and convert to a numeral
	stosb						; store to memory-buffer
	loop os_int_to_string_next_digit		; again for other remainders
	xor al, al
	stosb						; Store the null terminator at the end of the string

	pop eax
	pop ebx
	pop ecx
	pop edx
	ret
;-----------------------------------------------------------------------

	
;--------------------------------------------------------------------
; os_string_to_int:
;     Convert a string into a binary integer
;  IN:	RSI = location of string
; OUT:	RAX = integer value
;	All other registers preserved
; Adapted from http://www.cs.usfca.edu/~cruse/cs210s09/uint2rax.s
os_string_to_int:
	push esi
	push edx
	push ecx
	push ebx

	xor eax, eax			; initialize accumulator
	mov ebx, 10			; decimal-system's radix
os_string_to_int_next_digit:
	mov cl, [esi]			; fetch next character
	cmp cl, '0'			; char preceeds '0'?
	jb os_string_to_int_invalid	; yes, not a numeral
	cmp cl, '9'			; char follows '9'?
	ja os_string_to_int_invalid	; yes, not a numeral
	mul ebx				; ten times prior sum
	and ecx, 0x0F			; convert char to int
	add eax, ecx			; add to prior total
	inc esi				; advance source index
	jmp os_string_to_int_next_digit	; and check another char
os_string_to_int_invalid:
	pop ebx
	pop ecx
	pop edx
	pop esi
	ret
;------------------------------------------------------------





;---------------------------------------------------------------------
; os_string_length:
;     Return length of a string.
;  IN:	RSI = string location
; OUT:	RCX = length (not including the NULL terminator)
;	All other registers preserved
os_string_length:
	push edi
	push eax
	
	xor ecx, ecx
	xor eax, eax
	mov edi, esi
	not ecx
	cld
	repne scasb	; compare byte at RDI to value in AL
	not ecx
	dec ecx

	pop eax
	pop edi
	ret
;--------------------------------------------------	
	
	
	
; -----------------------------------------------------------------------------
; os_string_copy -- Copy the contents of one string into another
;  IN:	RSI = source
;	RDI = destination
; OUT:	All registers preserved
; Note:	It is up to the programmer to ensure that there is sufficient space in the destination
os_string_copy:
	push esi
	push edi
	push eax

os_string_copy_more:
	lodsb				; Load a character from the source string
	stosb
	cmp al, 0			; If source string is empty, quit out
	jne os_string_copy_more

	pop eax
	pop edi
	pop esi
	ret
; -----------------------------------------------------------------------------

	
; -----------------------------------------------------------------------------
; os_string_compare -- See if two strings match
;  IN:	RSI = string one
;	RDI = string two
; OUT:	Carry flag set if same
os_string_compare:
	push esi
	push edi
	push ebx
	push eax

os_string_compare_more:
	mov al, [esi]			; Store string contents
	mov bl, [edi]
	cmp al, 0			; End of first string?
	je os_string_compare_terminated
	cmp al, bl
	jne os_string_compare_not_same
	inc esi
	inc edi
	jmp os_string_compare_more

os_string_compare_not_same:
	pop eax
	pop ebx
	pop edi
	pop esi
	clc
	ret

os_string_compare_terminated:
	cmp bl, 0			; End of second string?
	jne os_string_compare_not_same

	pop eax
	pop ebx
	pop edi
	pop esi
	stc
	ret
; -----------------------------------------------------------------------------

	

; -----------------------------------------------------------------------------
; os_string_find_char -- Find first location of character in a string
;  IN:	RSI = string location
;	AL  = character to find
; OUT:	RAX = location in string, or 0 if char not present
;	All other registers preserved

os_string_find_char:
	push esi
	push ecx

	mov ecx, 1		; Counter -- start at first char
os_string_find_char_more:
	
	cmp byte [esi], al
	je os_string_find_char_done
	
	cmp byte [esi], 0
	je os_string_find_char_not_found
	
	inc esi
	inc ecx
	jmp os_string_find_char_more

os_string_find_char_done:
	mov eax, ecx

	pop ecx
	pop esi
	ret

os_string_find_char_not_found:
	pop ecx
	pop esi
	xor eax, eax	; not found, set RAX to 0
	ret
; -----------------------------------------------------------------------------


;=============================================================================
; os_string_change_char:
;     Change all instances of a character in a string.
;     Trocamos um caractere por outro dentro de uma string. 
;
; IN:	
;     ESI = Sstring location.
;	  AL  = Character to replace.
;	  BL  = Replacement character.
;
; OUT:	
;     All registers preserved
;
os_string_change_char:

	push esi  ;;Colocamos na pilha.
	
	push ecx
	push ebx
	push eax

	mov cl, al
os_string_change_char_loop:
	mov byte al, [esi]
	
	cmp al, 0
	je os_string_change_char_done
	
	cmp al, cl
	jne os_string_change_char_no_change
	
	mov byte [esi], bl

os_string_change_char_no_change:
	inc esi
	jmp os_string_change_char_loop

os_string_change_char_done:
	pop eax
	pop ebx
	pop ecx
	
	pop esi    ;;tiramos da pilha.
	ret
; -----------------------------------------------------------------------------
	
	
	


; -----------------------------------------------------------------------------
; os_string_chomp -- Strip leading and trailing spaces from a string
;  IN:	RSI = string location
; OUT:	All registers preserved
os_string_chomp:
	push esi
	push edi
	push ecx
	push eax

	call os_string_length		; Quick check to see if there are any characters in the string
	jecxz os_string_chomp_done	; No need to work on it if there is no data

	mov edi, esi			; RDI will point to the start of the string...
	push edi			; ...while RSI will point to the "actual" start (without the spaces)
	add edi, ecx			; os_string_length stored the length in RCX

os_string_chomp_findend:		; we start at the end of the string and move backwards until we don't find a space
	dec edi
	
	cmp esi, edi			; Check to make sure we are not reading backward past the string start
	jg os_string_chomp_fail		; If so then fail (string only contained spaces)
	
	cmp byte [edi], ' '
	je os_string_chomp_findend

	inc edi				; we found the real end of the string so null terminate it
	mov byte [edi], 0x00
	pop edi

	
os_string_chomp_start_count:		; read through string until we find a non-space character
	cmp byte [esi], ' '
	jne os_string_chomp_copy
	inc esi
	jmp os_string_chomp_start_count

os_string_chomp_fail:			; In this situataion the string is all spaces
	pop edi				; We are about to bail out so make sure the stack is sane
	mov al, 0x00
	stosb
	jmp os_string_chomp_done

; At this point RSI points to the actual start of the string (minus the leading spaces, if any)
; And RDI point to the start of the string

os_string_chomp_copy:		; Copy a byte from RSI to RDI one byte at a time until we find a NULL
	lodsb
	stosb
	cmp al, 0x00
	jne os_string_chomp_copy

os_string_chomp_done:
	pop eax
	pop ecx
	pop edi
	pop esi
	ret
; -----------------------------------------------------------------------------



; -----------------------------------------------------------------------------
; os_string_parse -- Parse a string into individual words
;  IN:	RSI = Address of string
; OUT:	RCX = word count
; Note:	This function will remove "extra" whitespace in the source string
;	"This is  a test. " will update to "This is a test."
os_string_parse:
	push esi
	push edi
	push eax

	xor ecx, ecx			; RCX is our word counter
	mov edi, esi

	call os_string_chomp		; Remove leading and trailing spaces
	
	cmp byte [esi], 0x00		; Check the first byte
	je os_string_parse_done		; If it is a null then bail out
	inc ecx				; At this point we know we have at least one word

os_string_parse_next_char:
	lodsb
	stosb
	cmp al, 0x00			; Check if we are at the end
	je os_string_parse_done		; If so then bail out
	cmp al, ' '			; Is it a space?
	je os_string_parse_found_a_space
	jmp os_string_parse_next_char	; If not then grab the next char

os_string_parse_found_a_space:
	lodsb				; We found a space.. grab the next char
	cmp al, ' '			; Is it a space as well?
	jne os_string_parse_no_more_spaces
	jmp os_string_parse_found_a_space

os_string_parse_no_more_spaces:
	dec esi				; Decrement so the next lodsb will read in the non-space
	inc ecx
	jmp os_string_parse_next_char
os_string_parse_done:
	pop eax
	pop edi
	pop esi
    ret
; -----------------------------------------------------------------------------
	
	


; -----------------------------------------------------------------------------
; os_string_append -- Append a string to an existing string
;  IN:	RSI = String to be appended
;	RDI = Destination string
; OUT:	All registers preserved
; Note:	It is up to the programmer to ensure that there is sufficient space in the destination
os_string_append:
	push esi
	push edi
	push ecx

	xchg esi, edi
	call os_string_length
	xchg esi, edi
	add edi, ecx
	call os_string_copy

	pop ecx
	pop edi
	pop esi
	ret
; -----------------------------------------------------------------------------



; -----------------------------------------------------------------------------
; os_hex_string_to_int -- Convert up to 8 hexascii to bin
;  IN:	RSI = Location of hex asciiz string
; OUT:	RAX = binary value of hex string
;	All other registers preserved
os_hex_string_to_int:
	push esi
	push ecx
	push ebx

	cld
	xor ebx, ebx
os_hex_string_to_int_loop:
	lodsb
	mov cl, 4
	cmp al, 'a'
	jb os_hex_string_to_int_ok
	sub al, 0x20				; convert to upper case if alpha
os_hex_string_to_int_ok:
	sub al, '0'				; check if legal
	jc os_hex_string_to_int_exit		; jmp if out of range
	cmp al, 9
	jle os_hex_string_to_int_got		; jmp if number is 0-9
	sub al, 7				; convert to number from A-F or 10-15
	cmp al, 15				; check if legal
	ja os_hex_string_to_int_exit		; jmp if illegal hex char
os_hex_string_to_int_got:
	shl ebx, cl
	or bl, al
	jmp os_hex_string_to_int_loop
os_hex_string_to_int_exit:
	mov eax, ebx				; int value stored in RBX, move to RAX
	pop ebx
	pop ecx
	pop esi
	ret
;-----------------------------------------------------------------

	
	