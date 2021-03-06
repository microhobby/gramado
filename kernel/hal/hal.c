/*
 * Gramado Hal - The main file for the Hal module in the kernel base. 
 * (c) Copyright 2015-2016 Fred Nora.
 *
 *  File: k\hal\hal.c 
 *  Hardware Abstraction Layer
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 *     //...
 */
 
 
#include <kernel.h>
//#include "hidata.h"  //Hal Internal Data.

//unsigned char  HalName[] = "HAL LAYER";

//
// inport.
//

extern unsigned long gdt;
extern unsigned long idt;

extern void gui_buffer_putpixel();

extern void swlib_backbuffer_putpixel();
extern void swlib_lfb_putpixel();


//
//@todo: criar rotina hal_get_page_dir();
//extern unsigned long _get_page_dir();
//

//
// Prot�tipo de fun��es internas.
//

unsigned long getGdt();
unsigned long getIdt();



 /* 
    Exemplo:
     checa alguns chipes especificos
 
    //...

		        // Procura por Am79C970, vendor 0x1022, device 0x2000.
                // 82371AB		
		        if( Vendor == 0x1022 &&    //AMD - (Advanced Micro Devices)   
		        Device == 0x2000 )         //Am79C970/1/2/3/5/6.
		        {
			        //System->SouthBridge = (unsigned long) 0;
		            printf("hal_hardware_detect: PCnet LANCE PCI Ethernet Controller Found.\n");
		        };
	
		        // Procura por PIIX4, vendor 8086, device 7113.
                // 82371AB		
		        if( Vendor == 0x8086 &&    //Intel.   
		        Device == 0x7113 )     //82371AB.
		        {
			        //System->SouthBridge = (unsigned long) 0x7113;
		            printf("hal_hardware_detect: Intel 82371AB PIIX4 Found.\n");
		        };

		        // Procura por vendor 8086, device 27B8. 
		        // Intel 82801GB ICH7.
		        // Name: 82801GB/GR (ICH7 Family) LPC Interface Bridge
		        // Name: I/O Controller Hub LPC
		        if( Vendor == 0x8086 &&    //Intel. 
		        Device == 0x27B8 )     //82801GB.
		        {
			        //System->SouthBridge = (unsigned long) 0x27B8;
		            printf("hal_hardware_detect: Intel 82801GB ICH7 Found.\n");
		        };
		
		        // Procura por vendor 8086, device 2448.
		        // Intel 82801 Mobile PCI Bridge.
		        if( Vendor == 0x8086 &&    //Intel.
                 Device == 0x2448 )     //82801.
		         {
			      //System->xxx = (unsigned long) 0x2448;
		           printf("hal_hardware_detect: Intel 82801 Mobile PCI Bridge Found.\n");
		         };		
		
		         // Procura por vendor 8086, device 27B9.
		         // Intel 82801GBM (ICH7-M) LPC Interface Bridge.
		        if( Vendor == 0x8086 &&    //Intel. 
		         Device == 0x27B9 )     //82801GBM.
		        {
			       //System->xxx = (unsigned long) 0x27B9;
		             printf("hal_hardware_detect: Intel 82801GBM (ICH7-M) LPC Interface Bridge Found.\n");
		          };

		        // Procura por vendor 8086, device 2829.
		        // Intel(R) ICH8M SATA AHCI Controller.
		        if( Vendor == 0x8086 &&    //Intel. 
		        Device == 0x2829 )     //Intel(R) ICH8M SATA AHCI Controller.
		       {
		     	//System->xxx = (unsigned long) 0x2829;
		        printf("hal_hardware_detect: Intel(R) ICH8M SATA AHCI Controller. Found.\n");
		       };
		
		       // vendor 0x10EC Realtek Semiconductor Corp.
		       //if( Vendor == 0x10EC && Device == 0 ){};
		
		// Continua...	
				
				//...
 
 */

 
/*
 * Beep support
 *
void Beep::tone(int freq) {
    if (freq == 0) {
        int i = io_in8(0x61);
        io_out8(0x61, i & 0x0d);
    } else {
        int i = 1193180000 / freq;
        io_out8(0x43, 0xb6);
        io_out8(0x42, i & 0xff);
        io_out8(0x42, i >> 8);
        i = io_in8(0x61);
        io_out8(0x61, (i | 0x03) & 0x0f);
    }
}
*/ 
 
 
/*
void halMain();
void halMain(){
	return;
};
*/


//@todo: criar. halGetBootInfo()



/*
 ************************************************
 * hal_backbuffer_putpixel:
 *     Coloca um pixel no backbuffer.
 */
void 
hal_backbuffer_putpixel ( unsigned long ax, 
                          unsigned long bx, 
						  unsigned long cx, 
						  unsigned long dx )
{	
	// Chama a rotina em assembly depois de enviar os valores para os 
	// registradores.
	// IN: cor, x, y, 0
	
	asm volatile ( "\n" : : "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
	
    swlib_backbuffer_putpixel ();	
};


/*
 ************************************************
 * hal_lfb_putpixel:
 *     Coloca um pixel no LFB.
 */
void 
hal_lfb_putpixel ( unsigned long ax, 
                   unsigned long bx, 
				   unsigned long cx, 
				   unsigned long dx )
{	
	// Chama a rotina em assembly depois de enviar os valores para os 
	// registradores.
	// IN: cor, x, y, 0
	
	asm volatile ( "\n" : : "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
	
	swlib_lfb_putpixel ();
};



/*
 * sys_vsync:
 *     Sincroniza o retra�o vertical do monitor.
 *     ? Isso ainda est� em uso ?
 */
void sys_vsync (){
	
    hal_vsync ();	
};


/*
 * sys_showpciinfo:
 *     Mostra informa��es encontradas na interface PCI.
 */
int sys_showpciinfo (){
	
    return (int) hal_showpciinfo ();
};


/*
 * sys_reboot:
 *     Reboot, Servi�o do sistema.
 *     Chamando uma rotina interna de reboot do sistema.
 */
void sys_reboot (){
	
    KiReboot ();
    panic ("sys_reboot:");
};


/*
 * sys_shutdown:
 *     Chama uma rotina interna para desligar a m�quina.
 */
void sys_shutdown (){
	
    KiShutDown ();
    panic ("sys_shutdown:");
};


/*
 *********************************************************
 * hal_hardware_detect:
 *     Detecta fabricantes espec�ficos suportados pelo n�cleo.
 *
 * 8086, 1237  //PCI & Memory.
 * 8086, 7000  //PIIX3 PCI-to-ISA Bridge (Triton II).
 * 1022, 2000  //Advanced Micro Devices, PCnet LANCE PCI Ethernet Controller.
 * 8086, 7113  //PIIX4/4E/4M Power Management Controller.
 * 8086, 2829  //Intel(R) ICH8M SATA AHCI Controller.
 * //...
 *
 */

// Consumer Chipsets (Z87, H87, H81) Haswell LGA1150. 
// Business Chipsets (Q87, Q85, B85) Haswell LGA1150.
 
 
int hal_hardware_detect (){
	
    return (int) 0;    //#todo	
};


//#bugbug: tem algo errado aqui nos nomes das fun��es.
//hal_showpciinfo deveria ser a rotina que pci.c chama 
//para obter acesso as informa��es em baixo n�vel.
//@todo: rever os nomes das fun��es.

int hal_showpciinfo (){
	
	return (int) pciInfo();
};


/*
 ******************************************
 * init_cpu:
 *     Inicializa a estrutura do processador e as coisas associadas a ele.
 *     Inicializa apenas o que for independente da arquitetura.
 *     @todo: Essa rotina pode ir para outro modulo do /hal. como cpu.c
 */
 
void init_cpu (){
	
    int Status = 0;
	
	//
	//  ## Processor struct ##
	//
	
	processor = (void *) malloc ( sizeof(struct tagProcessor) );
	
	if ( (void *) processor == NULL )
	{
	    panic ("hal-init_cpu: processor");
		
	} else {
	    
		//@todo: set processor id: escolhe o processador atual. ??
	
	    //Inicializa alguns valores da estrurura.
		
	    processor->Gdt = (unsigned long) getGdt();
	    processor->Idt = (unsigned long) getIdt();
	    processor->Tss = 0;  //processor->Id=getTss();
        
		//...
        
		if ( (void *) Hardware != NULL ){
			Hardware->Processor = processor;
		}
		
        //Nothing.		
    };	
	
	//sonda.
	//Checa qual cpu �, e inicializa mais vari�veis.
	
	Status = (int) hal_probe_cpu (); 
	
	if( Status != 0 )
	{
	    panic ("init_cpu: hal_probe_cpu Status \n");  		
	};
	
    //More?!	


#ifdef HAL_VERBOSE
    printf("Done\n");
#endif	

    //#todo, o retorno deve ser int, para refletir o status.
    return;
};


void hal_set_machine_type ( unsigned long type ){
	
    g_machine_type = (unsigned long) type;
};


unsigned long hal_get_machine_type (){
	
    return (unsigned long) g_machine_type;
};


/*
 *****************************************************
 *  hal_init_machine:
 *      Faz inicializa��es dado o tipo de m�quina.
 *      @todo: Trocar o nome para hal_init_current_machine. 
 */
 
int hal_init_machine (){
	
	// Limits for machine type.
	
	if ( g_machine_type < 0 || g_machine_type > 4)
	{   
		panic ("hal-hal_init_machine:");  
	};
	
	
	// Type.
	
	switch (g_machine_type)    
	{
		//Unknow.
	    case 0:
		    //processor->Type = (unsigned char) 0;
		    break;
        
		//Intel.
	    case 1:
		    //processor->Type = (unsigned char) 1;
		    break;

	    //Amd.
		case 2:
		    //processor->Type = (unsigned char) 2;
		    break;

		//...	
	    case 3:
		    //processor->Type = (unsigned char) 3;
		    break;
        
		//Unknow.
		default:
		    //processor->Type = (unsigned char) 0;
	        printf("hal_init_machine error: default type\n");
            return (int) 0; 			
		    break;
	};

	//More?!

    return (int) 0;
};


unsigned long getGdt (){
	
    return (unsigned long) &gdt; 
};


unsigned long getIdt (){
	
    return (unsigned long) &idt; 
};


void hal_vsync (){
	
    vsync ();
};


/*
 ********************************
 * hal_shutdown:    
 */
void hal_shutdown (){
	
    shutdown ();
    panic ("hal_shutdown:");
};


/*
 ***********************************************
 * shutdown:
 *     Desligar a m�quina.
 *     @todo: APM, ACPI.
 *
 APM:
 ===
 This is the basic sequence of APM commands that must be given in order to shut down a computer.
 +Perform an installation check.
 +Disconnect any existing APM interface.
 +Connect the Real Mode interface.
 +Enable power management for all devices.
 +Set the power state for all devices to "Off" (03h).
 
 
 ACPI:
 ====
  The ACPI shutdown is technically a really simple thing all 
  that is needed is a 
  
  outw(PM1a_CNT, SLP_TYPa | SLP_EN ); 
 
 and the computer is powered off. 
 
 The problem lies in the gathering of these values 
 especialy since the SLP_TYPa is in the \_S5 object 
 which is in the DSDT and therefore AML encoded.
 
 */
void shutdown (){
	
	// @todo: switch APM, ACPI. modo smm	
	//        Obs: Aqui, temporariamente, poderia desabilitar todo o
	//             sistema e permitir que o usu�rio desligue a energia
	//             manualmente. 
	//            (Ex: O computador pode ser desligado com seguran�a).  
	
    panic ("hal-shutdown: fail");
};


/*
 * KiReboot:
 *     Inicializa��o da parte de hardware do processo de reboot.   
 */
 
void KiReboot (){
	
	// @todo: fechar as coisas aqui antes de chamar hal_reboot()
	// pois hal fica responsavel pela parte de hardware.
    // As rotinas de desligamento do sistema foram para syste.c systemReboot().
	// ficar� aqui somente o que prescede o shamamento de hal.
	// em hal ficar�o as rotinas de reiniamento de hardware.
    
    // hal	
    // Reboot via keyboard.	
	
	hal_reboot ();
	panic ("KiReboot");
};


/*
 * KiShutDown:
 *    @todo: Isso ser� uma interface para chamar o deligamanto.
 */
 
void KiShutDown (){
	
    hal_shutdown ();
    panic ("KiShutDown");
};


/*
 * hal_reboot:
 *     O hal � a camada mis pr�xima do hardware, n�o h� tratamento nenhum
 * para fazer, somente chamar o reboot via teclado. 
 */

//em headlib.s 
extern void asm_reboot(); 
void hal_reboot (){
	
    asm_reboot (); 
	panic("hal_reboot");
};


/*
 ********************************************************
 * init_hal:
 * Initialize kernel base hal.
 *     Archtecture independent inicialization ...
 *
 */
//int halInit() 
int init_hal (){
	
    int Status = 0;
	
#ifdef KERNEL_VERBOSE	
	printf("HAL:\n");
#endif	
	
	// CPU - Cria e inicializa a estrutura de cpu.
#ifdef HAL_VERBOSE
	printf("init_hal: cpu\n");
#endif	
	init_cpu();

	
	//
	// Bug Bug - PCI Initialization.
	//
	
	// PCI - Cria e inicializa estrutura de pci.
#ifdef HAL_VERBOSE	
	printf("init_hal: pci\n");
#endif	
	init_pci();    //bugbug (a rotina esta incompleta.)	

	//@todo: Isso � um teste de inicializa��o de pci.
	//printf("* init_hal: Debug!");
    //refresh_screen();
    //while(1){}		
	
	
	// TIMER - Cria e inicializa estrutura do timer.
#ifdef HAL_VERBOSE	
	printf("init_hal: Timer\n");
#endif    
	timerInit();	

	// RTC - Cria e inicializa estrutura de rtc.
#ifdef HAL_VERBOSE	
	printf("init_hal: rtc\n");
#endif	
	init_clock();
	get_cmos_info();
		
		
	//
    //@todo 8042 ... teclado e mouse.
    //
    //P8042_install();	
		
    //keyboard.
//#ifdef HAL_VERBOSE	
	//printf("init_hal: kd\n");
//#endif	
	//init_keyboard();
	
	//mouse
	//printf("init_hal: mouse..\n");
	//init_mouse();  //isso est� em keyboard.c
	
	//printf("init_hal: init machine (bug)\n");	
	//hal_init_machine();	
	

	//Ports components.
	
	//
	// bios date. (detect)
	//
	
	//Collecting Disk Geometry
	//Floppy Component 
	

	//Video Display Adapter.
	 //init_screen();
	
	//Mouse components

	//
	// Detecta fabricantes espec�ficos suportados pelo n�cleo.  
	//
	
	hal_hardware_detect();
	
	//
	// Continua ...
	//

//Done:

    Initialization.hal = 1;	
	
#ifdef HAL_VERBOSE
	printf("Done\n");
#endif


#ifdef BREAKPOINT_TARGET_AFTER_HAL
    //#debug 
	//a primeira mensagem s� aparece ap�s a inicializa��o da runtime.
	//por isso n�o deu pra limpar a tela antes.
	printf(">>>debug hang: after init");
	refresh_screen(); 
	while (1){
		asm ("hlt");
	}
#endif	
	
	return (int) Status;
};


//
// End.
//
