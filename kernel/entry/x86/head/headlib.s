;
; File: headlib.s 
;
; Descri��o:
;     Fun��es auxiliares de supporte a head.s do Kernel Base.
;
; Hist�rico:
;     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
;     Vers�o 1.0, 2016 - Revis�o.
;     ...
;

;
; Vari�veis globais..
;


;Se o kernel est� inicializado ou n�o.
global _KernelStatus
_KernelStatus: 
    dd 0	
	
;Vari�vel sera usada pelo scheduler.	
global	_task_switch_salva_esp
_task_switch_salva_esp: 
    dd 0		
	
	
;
; Vari�veis locais.
;	
	
current:       dd 0    ;current.
scr_loc:       dd 0    ;coisa da mem�ria de v�deo.
bl_video_mode: db 0 
bl_lfb:        db 0  
	

;
; @todo:
; Nota sobre a pilha em ring0 configurada na tss:
; O valor atual � 0x00200000. Mas n�o � o que queremos,
; o que queremos � o equivalente f�sico � 0xC03FFFF0.
; Que seria (0x00100000 + 3FFFF0).
; O problema � que temos um aplicativo come�ando no 
; endere�o f�sico 0x400000. Com isso a pilha sujaria
; o aplicativo.
; � necess�rio que o gerenciamento de mem�ria f�sica
; aloque mem�ria f�sica para os aplicativos em algum
; pool de mem�ria f�sica padronizado.
;
;
	
;
; TSS. 
;

;
;tss0
dd 0
dd 0	
tss0:
	dd 0x401000             ;back link
	dd 0x003FFFF0           ;esp0    (pilha do kernel) @todo: a pilha do kernel esta no fim do kernel
	dd 0x10                 ;ss0
	dd 0                    ;esp1
	dd 0                    ;ss1	
	dd 0                    ;esp2
	dd 0                    ;ss2	
	dd 0x9C000              ;cr3 
	dd 0x401000             ;eip   
	dd 0x00003200           ;eflags  (CPL = 3, interrup��es habilitadas)	
	dd 0                    ;eax
	dd 0                    ;ecx
	dd 0                    ;edx
	dd 0                    ;ebx	
	dd 0x43ffff             ;esp   (stack em user mode)
	dd 0                    ;ebp
	dd 0                    ;esi
	dd 0                    ;edi
	dd 0x23                 ;es 
	dd 0x1B                 ;cs 
	dd 0x23                 ;ss
	dd 0x23                 ;ds
	dd 0x23                 ;fs
	dd 0x23                 ;gs 
	dw LDT_TEST_SEL, 0	    ;LDT, reserved
	dw 0, tss0_iopb - tss0  ;debug, IO permission bitmap (none)	
tss0_iopb:
    times 8192 db 0FFh    ;@todo: Isso � realmente necess�rio.
tss0_end:
	
;
;tss1
dd 0
dd 0
tss1:		          
	dd _task0               ;back link  (todo: esse backlink esta errado,precisa ser logico)
	dd 0x003FFFF0           ;esp0   (pilha do kernel) @todo: a pilha do kernel esta no fim do kernel
	dd 0x10                 ;ss0
	dd 0                    ;esp1
	dd 0                    ;ss1	
	dd 0                    ;esp2
	dd 0                    ;ss2	
	dd 0x9C000              ;cr3 
	dd 0x401000             ;eip   
	dd 0x00003200           ;eflags  (CPL = 3, interrup��es habilitadas)	
	dd 0                    ;eax
	dd 0                    ;ecx
	dd 0                    ;edx
	dd 0                    ;ebx	
	dd 0x43ffff             ;esp (stack em user mode)
	dd 0                    ;ebp
	dd 0                    ;esi
	dd 0                    ;edi
	dd 0x23                 ;es 
	dd 0x23                 ;cs 
	dd 0x23                 ;ss
	dd 0x23                 ;ds
	dd 0x23                 ;fs
	dd 0x23                 ;gs 
	dw LDT_TEST_SEL, 0	    ;LDT, reserved
	dw 0, tss1_iopb - tss1  ;debug, IO permission bitmap (none)	
tss1_iopb:
    times 8192 db 0FFh    ;@todo: Isso � realmente necess�rio.
tss1_end:
;
;fim
;

;
; LDTs.
;
 
;
;ldt 0.
dd 0
dd 0
ldt0:	
	dq 0x0000000000000000    ;null 
	dq 0x00c0fa01000003ff    ;0x0f, base = 0x10000	
	dq 0x00c0f201000003ff    ;0x17
	
;
;ldt 1.
dd 0
dd 0
ldt1:	
	dq 0x0000000000000000   ;null
	dq 0x00c0fa01000003ff   ;0x0f, base = 0x10000
    dq 0x00c0f201000003ff   ;0x17	
;
;fim.
;
	
	
;-----------------------------------------------------------------------------	
; _setup_system_interrupt: 
;    Configura um vetor da IDT para a interrup��o do sistema. ?? O endere�o do 
; ISR e o n�mero do vetor s�o passados ?? via argumento.
;
; IN:
;    eax = endere�o. 
;    ebx = n�mero do vetor (200).
;
global _setup_system_interrupt
_setup_system_interrupt:
    cli
	pushad

    mov dword [.address], eax    ;endere�o.
    mov dword  [.number], ebx    ;n�mero do vetor.
	
	;calcula o deslocamaneto
	mov eax, dword 8
	mov ebx, dword [.number]
	mul ebx
	;resuldado em eax
	
	;adiciona o deslocamento � base.
	mov edi, dword _idt               
	add edi, eax
	
	mov edx, dword [.address] 
	
	mov eax, dword 0x00080000    ;/* selector = 0x001B = user cs */	
	mov ax, dx		             ;uma parte do endere�o
	mov dx, word 0xEE00	         ;/* interrupt gate - dpl=3, present */
	
	;coloca o vetor na idt
	mov dword [edi+0], eax
	mov dword [edi+4], edx
	
 	;recarrega a nova idt
	;lidt [IDT_register]	
	
	popad
	sti      ;; @todo: #bugbug Cuidado !!!
	ret
.address: dd 0
.number: dd 0


;=============================================
; setup_faults:
;    Configura vetores da idt para faults.
;
setup_faults:
	push eax
	push ebx
	
	;#0  
	mov eax, dword _fault_N0
	mov ebx, dword 0
	call _setup_idt_vector	
	
	;#1  
	mov eax, dword _fault_N1
	mov ebx, dword 1
	call _setup_idt_vector	
	
	;#2  
	mov eax, dword _fault_N2
	mov ebx, dword 2
	call _setup_idt_vector	
	
	;#3  
	mov eax, dword _fault_N3
	mov ebx, dword 3
	call _setup_idt_vector	
	
	;#4  
	mov eax, dword _fault_N4
	mov ebx, dword 4
	call _setup_idt_vector	
	
	;#5  
	mov eax, dword _fault_N5
	mov ebx, dword 5
	call _setup_idt_vector	

	;#6 - Instru��o inv�lida.
	mov eax, dword _fault_INTRUCAO_INVALIDA
	mov ebx, dword 6
	call _setup_idt_vector
	
	;#7  
	mov eax, dword _fault_N7
	mov ebx, dword 7
	call _setup_idt_vector
	
	;#8 - double fault
	mov eax, dword _fault_DOUBLE
	mov ebx, dword 8
	call _setup_idt_vector	
	
	;#9  
	mov eax, dword _fault_N9
	mov ebx, dword 9
	call _setup_idt_vector

	;#10  
	mov eax, dword _fault_N10
	mov ebx, dword 10
	call _setup_idt_vector

	;#11  
	mov eax, dword _fault_N11
	mov ebx, dword 11
	call _setup_idt_vector
	
	;#12 - stack
	mov eax, dword _fault_STACK
	mov ebx, dword 12
	call _setup_idt_vector		
	
	;#13 - general protection
	mov eax, dword _fault_GP
	mov ebx, dword 13
	call _setup_idt_vector	
	
	;#14  
	mov eax, dword _fault_N14
	mov ebx, dword 14
	call _setup_idt_vector	

	;#15 
	mov eax, dword _fault_N15
	mov ebx, dword 15
	call _setup_idt_vector	

	;#16 
	mov eax, dword _fault_N16
	mov ebx, dword 16
	call _setup_idt_vector	

	;#17  
	mov eax, dword _fault_N17
	mov ebx, dword 17
	call _setup_idt_vector	

	;#18  
	mov eax, dword _fault_N18
	mov ebx, dword 18
	call _setup_idt_vector	

	;#19  
	mov eax, dword _fault_N19
	mov ebx, dword 19
	call _setup_idt_vector	

	;#20  
	mov eax, dword _fault_N20
	mov ebx, dword 20
	call _setup_idt_vector	

	;#21  
	mov eax, dword _fault_N21
	mov ebx, dword 21
	call _setup_idt_vector	

	;#22  
	mov eax, dword _fault_N22
	mov ebx, dword 22
	call _setup_idt_vector	

	;#23 
	mov eax, dword _fault_N23
	mov ebx, dword 23
	call _setup_idt_vector	

	;#24  
	mov eax, dword _fault_N24
	mov ebx, dword 24
	call _setup_idt_vector	

	;#25  
	mov eax, dword _fault_N25
	mov ebx, dword 25
	call _setup_idt_vector	

	;#26 
	mov eax, dword _fault_N26
	mov ebx, dword 26
	call _setup_idt_vector	

	;#27  
	mov eax, dword _fault_N27
	mov ebx, dword 27
	call _setup_idt_vector	

	;#28 
	mov eax, dword _fault_N28
	mov ebx, dword 28
	call _setup_idt_vector	

	;#29  
	mov eax, dword _fault_N29
	mov ebx, dword 29
	call _setup_idt_vector	

	;#30  
	mov eax, dword _fault_N30
	mov ebx, dword 30
	call _setup_idt_vector	

	;#31 
	mov eax, dword _fault_N31
	mov ebx, dword 31
	call _setup_idt_vector	
	
	pop ebx
	pop eax
	ret	
	

;=====================================
; setup_vectors:
;    Configura alguns vetores da idt.
;	
setup_vectors:
	push eax
	push ebx 
	

	;32
	;Iniciamos um timer provis�rio, 
	;depois o kMain() inicia o definitivo.
	;mov eax,  dword _irq0         ;Ser� inicializado em C. 
	mov eax, dword _timer_test     ;Provis�rio.
	mov ebx, dword 32
	call _setup_idt_vector	

	;33
	;Keyboard.
	mov eax, dword  _irq1
	mov ebx, dword 33
	call _setup_idt_vector
	
	;40
	;Clock, rtc.
	mov eax, dword  _irq8
	mov ebx, dword 40
	call _setup_idt_vector
	
	;44
	;Mouse.
	mov eax, dword  _irq12
	mov ebx, dword 44
	call _setup_idt_vector	
	
	;46
	;ide
	mov eax,  dword _irq14     
	mov ebx, dword 46
	call _setup_idt_vector	

	;47
	;ide
	mov eax,  dword _irq15     
	mov ebx, dword 47
	call _setup_idt_vector	
	


    ;128 - 0x80
	;A interrup��o de sistema.
	;#obs: Utilizamos uma chamada dirferente
	;para configurar essa interrup��o.
	mov eax, dword _int128
	mov ebx, dword 128
	call _setup_system_interrupt  
	;call _setup_idt_vector   
	
    ;200
	;A interrup��o de sistema.
	;#obs: Utilizamos uma chamada dirferente
	;para configurar essa interrup��o.
	;mov eax, dword _int200
	;mov ebx, dword 200
	;call _setup_system_interrupt  
	;call _setup_idt_vector       
   
    ;213
	;Executa nova tarefa.
	mov eax, dword _int213
	mov ebx, dword 213        
	call _setup_idt_vector	
	
	
    ;216
	;Fast Create Window.
	mov eax, dword _int216
	mov ebx, dword 216        
	call _setup_idt_vector	
	
	
	;;provis�rio.
	;mov eax, dword _irq9
	;mov ebx, dword 41       
	;call _setup_idt_vector		
	
	pop ebx
	pop eax
	ret
		

;;=================================================
;; ## Configurando nova entrada ##
;;	

;;#bugbug: isso est� em intel.c , mas precisa ser global para que todos 
;;possam usar.
;;talvez em kernel.h
extern _nic_idt_entry_new_number
extern _nic_idt_entry_new_address


global _asm_nic_create_new_idt_entry
_asm_nic_create_new_idt_entry:
	pushad
	
	;isso funcionou, tentar configurar outras interup��es com isso.
	
	mov eax, _nic_handler
	;mov eax, dword [_nic_idt_entry_new_address] ; 
	mov ebx, dword [_nic_idt_entry_new_number]	
	
	;deletar
	;mov eax, dword _irq9 ;[_idt_entry_new_address] ; 
	;mov ebx, dword 41; [_idt_entry_new_number] 

	call _setup_idt_vector	
    
	popad
	ret 
	
;;===============================================	
extern _xxxe1000handler

global _nic_handler	
_nic_handler:
    cli
	pushad
	
	call _xxxe1000handler
	
	mov al, 0x20
    out 0x20, al
    out 0xA0, al  
	
	popad
	sti
    iretd		

;====================================================
; _do_executa_new_task:
;
;  _contextSS      -  user data + RPL
;  _contextESP*	   -  *** user mode task esp
;  _contextEFLAGS  -  eflags
;  _contextCS      -  user code + RPL
;  _contextEIP*    -  *** entry point da tarefa.
;
; PS:. Essa rotina � chamada pelo kernel depois de 
; salvo o contexto da thread interrompida.
;
global _do_executa_new_task
_do_executa_new_task:	

    ;ajusta segmentos e RPL
	xor eax, eax	
    mov ax, 0x23        
    mov ds, ax                 
    mov es, ax
    mov fs, ax ;Mudar para 0. ??
    mov gs, ax ;Mudar para 0. ??
  
	;ss   USER_DATA_SEL+RPL
	xor eax, eax
	mov ax, word 0x23    ;user data com rpl = 3.
	push dword eax              
	
	;esp
	push dword [_contextESP]    
	
	;eflags  (IOPL = 3)
	push dword 0x00003200         	
	
	;cs      (CPL  = 3)  USER_CODE_SEL+RPL
	xor eax, eax
	mov ax, word 0x1B    ;user code com rpl = 3.
	push dword eax                     
	;push word ax        ;teste.
	
	;eip
	push dword [_contextEIP]    ;eip.
	
    mov al, 0x20
    out 0x20, al 
	sti	
	iretd

	
;--------------------------------------------
; set_base:	
;     ??
;
; in: 
;    eax - logic addr.
;    ebx = base addr. 
;    ecx - table addr. 
;    edi - descriptors offset.
;
set_base:
	add eax, ebx
	add edi, ecx
	mov [edi+2], word ax
	
	ror eax, 16
	
	mov [edi + 4], al
	mov [edi +7],  ah
	ror eax, 16
	ret
	

;------------------------------------------------------------
; _test_cpuid_support:
;     Testar via eflags se a cpu suporta a instru��o cpuid. 
; @todo: Essa rotina poderia se chamar _headlibTestCPUIDSupport.
;
global _test_cpuid_support	
_test_cpuid_support:	
	
	;cpuid supported?
    pushfd                  ;push the flags onto the stack.
    pop eax                 ;pop them back out, into EAX.
    
	mov ebx, eax            ;keep original.
    
	xor eax, 00200000h      ;turn bit 21 on.
    
	push eax                ;put altered EAX on stack.
    popfd                   ;pop stack into flags.
	
	pushfd                  ;push flags back onto stack.
    pop eax                 ;put them back into EAX.
    
	cmp eax, ebx
    jnz @CPUID_SUPPORTED    ;COOL.
                
	mov eax, dword 1
	;blah                   ;booo.
	stc
	RET
@CPUID_SUPPORTED:
    mov eax, dword 0
    clc 
	RET
	
	
;==========================================================
; _get_page_dir:
;     Pega o valor de cr3.
;     page dir. 
; @todo: Essa rotina poderia se chamar _headlibGetPageDirectory.
;	
global _get_page_dir	
_get_page_dir:
	mov eax, cr3
	ret
	
	
;==================================================================
; _set_page_dir:
;     configura o cr3
;     page dir. 
; ; @todo: Essa rotina poderia se chamar _headlibSetPageDirectory.
;	
global _set_page_dir	
_set_page_dir:
	mov cr3, eax
	ret
	
	
;=============================================
; _get_page_fault_adr: 
;     Pega o cr2.
;     endere�o quando da pagefault.
;	
global _get_page_fault_adr	
_get_page_fault_adr:
	mov eax, cr2
	ret	
	
;---------------------------
; _halt:
;    Executa a instru��o hlt.
;	
global _halt	
_halt:
	hlt
	ret
	
;================================================
; setup_gdt:
;     Carrega gdtr.
;
setup_gdt:
	lgdt [_GDT_register]
	ret
	
	
;=================================================
; setup_idt:
;     Configura a IDT.
;     Dado o endere�o da IDT, 
;     preenche todos os vetores, 
;     apontando para um s� endere�o. 'unhandled_int'.
;
;++
setup_idt:
    pushad
	mov edx, unhandled_int  
	
	mov eax, dword 0x00080000    ;/* selector = 0x0008 = cs */	
	mov ax, dx		             ;uma parte do endere�o
	mov dx, word 0x8E00	         ;/* interrupt gate - dpl=0, present */
	
	mov edi, dword _idt                
	
	mov ecx, dword 256
rp_sidt:	
	mov dword [edi+0], eax
	mov dword [edi+4], edx
	add  edi, dword 8	
	dec ecx
	jne rp_sidt	
	;lidt [IDT_register]	
		
	popad
	ret
;;--	


;=====================================
; _setup_idt_vector:
;     Configura um vetor da IDT.
; IN:
; eax = endere�o
; ebx = n�mero do vetor
;
global _setup_idt_vector
_setup_idt_vector:
    ;cli
	pushad

    mov dword [.address], eax    ;endere�o.
    mov dword [.number],  ebx    ;numero do vetor.

	;calcula o deslocamaneto
	mov eax, dword 8
	mov ebx, dword [.number]
	mul ebx
	;resuldado em eax
	
	;adiciona o deslocamento � base.
	mov edi, dword _idt               
	add edi, eax
		
	mov edx, dword [.address]    ;unhandled_int ;ignore_int       ;lea edx, ignore_int
	
	mov eax, dword 0x00080000    ;/* selector = 0x0008 = cs */	
	mov ax, dx		             ;uma parte do endere�o
	mov dx, word 0x8E00	         ;/* interrupt gate - dpl=0, present */
	
	;coloca o vetor na idt
	mov dword [edi+0], eax
	mov dword [edi+4], edx
	
 	;recarrega a nova idt
	;lidt [IDT_register]	
	
	popad
	;sti
	ret
.address: dd 0
.number: dd 0


;=====================================
; _enable_pse:
;     Enable PSE para p�ginas de 4MB.
;     N�o usaremos isso.
;
global _enable_pse
_enable_pse:
    push eax	
	mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax		
	pop eax
	ret
	
;=====================================
; _asm_shut_down:
; * shut down
; @todo: ainda n�o implementada.
;
global _asm_shut_down
_asm_shut_down:
	jmp _asm_reboot  ;;Errado.      
    jmp $
	
	
;=====================================
; _asm_reboot:
;     Reboot via teclado.
; @todo: Essa rotina poderia se chamar _headlibReboot. 
;
global _asm_reboot	
_asm_reboot:
    ; Wait for an empty Input Buffer.
	in al, 0x64
	test al, 00000010b		
	jne _asm_reboot
	; Send the reboot call to the keyboard controller.
	mov al, 0xFE
	out 0x64, al			
	hlt
    jmp _asm_reboot	
	
;
; Fim.
;
