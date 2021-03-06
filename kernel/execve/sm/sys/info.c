/*
 * File: info.c
 *
 * Descri��o:
 *     Rotinas de informa��es sobre o sistema.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2016 - Revision.
 */
 

#include <kernel.h>


//Usados para mostrar informa��es sobre a localiza��o 
//das partes da imagem do kernel.
extern unsigned long kernel_begin;
extern unsigned long kernel_end;
extern unsigned long code_begin;
extern unsigned long code_end;
extern unsigned long data_begin;
extern unsigned long data_end;
extern unsigned long bss_begin;
extern unsigned long bss_end;



/*
 * KeInformation: 
 *    @todo:
 *    Mostra em informa��es sobre o kernel.
 *    Chama m�dulo externo (Server em user mode) para
 *    mostrar informa��es sobre o kernel ou sobre o sistema.
 *
 *    Obs: 
 *    As inform��es podem ser salvas em um arquivo de sa�da padr�o.
 */

void KeInformation (){
	
	//return;    //Ainda n�o implementada.
};


/*
 *********************************************************
 * KiInformation: 
 * Mostra uma lista de informa��es sobre o Sistema.
 * Obs: @todo: A lista de informa��es de ser bem completa, 
 * pois servir� de ajuda ao desenvolvedor. Deve conter 
 * de tudo, abordando todas as camadas do sistema. 
 * 
 * Ordem por classe:
 * ================
 * RAM: ram, sm, uigm, uitm.
 * IO: cpu, dma.
 * DEVICES: umblocked, blocked
 * THINGS:
 *
 * Obs: As inform��es podem ser salvas em um arquivo de sa�da padr�o.
 */

//void infoShowKernelInfo() 

void KiInformation (){
	
	//struct window_d *hWnd;
	//struct window_d *hWindow;
	
	/*

	unsigned long left;
	unsigned long top;    
	unsigned long width;  
	unsigned long height; 	
	
	//unsigned long HeapTotal = ((kernel_heap_end - kernel_heap_start)/1024);
	//unsigned long StackTotal = ((kernel_stack_start - kernel_stack_end)/1024);	
	
	if ( VideoBlock.useGui != 1 )
	{
		printf("KiInformation: No GUI\n");
		goto fail;
	}	
	
	//Parent window.
	
	if ( (void *) gui->main == NULL )
	{
		printf("KiInformation: parent window fail\n");
		goto fail;
		
	} else {
		
	    left = gui->main->left;
	    top = gui->main->top;
	    
		width = gui->main->width;
	    height = gui->main->height;		
        
	    // Cursor com base na janela m�e.
	    g_cursor_x = (left/8);
	    g_cursor_y = (top/8); 	
		
		//...
	};
	
	*/
	
	//printf ("#breakpoint");
	//refresh_screen();
	//while(1){}
	
	/*
	
	// ## Window ##
		
    //Obs: Podemos usar o esquema de cores padr�o.
    //Mas a inten��o � que as janelas do kernel tenham
    //sua pr�pria cor.	
	
	//hWindow = (void *) CreateWindow ( WT_OVERLAPPED, 1, 1, "KiInformation", 
	//                    left, top, width, height, 
	//		     		0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
	
	hWindow = (void *) CreateWindow ( WT_SIMPLE, 1, 1, "KiInformation", 
	                    left, top, width, height, 
			     		gui->main, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );


	if ( (void *) hWindow == NULL)
	{
	    printf("KiInformation: hWindow\n");
	    goto fail;
		
    }else{
		
		RegisterWindow(hWindow);
		set_active_window(hWindow);
		SetFocus(hWindow);
		//...
	
	
		// Auterando as margens.
		// Essas margens s�o usadas pela fun��o printf.
		// Obs: As medidas s�o feitas em n�meros de caracteres.
		// Obs: @todo: Devemos usar aqui o ret�ngulo da �rea de cliente,
		// e n�o as margens da janela.
		// A estrutura de janela deve nos oferecer os valores para a m�trica do 
		// ret�ngulo da �rea de cliente.
		// Obs: @todo:Acho que essa n�o � a forma correta de configurar isso. Uma 
		//rotina deveria perceber as dimens�es da janela de do caractere e determinar
		//as margens.
		
		g_cursor_left = (hWindow->left/8);
		g_cursor_top = (hWindow->top/8) + 4;   //Queremos o in�cio da �rea de clente.
		g_cursor_right = g_cursor_left + (width/8);
		g_cursor_bottom = g_cursor_top  + (height/8);
	
	
	
		//cursor (0, mas com margem nova).
		g_cursor_x = g_cursor_left; 
		g_cursor_y = g_cursor_top; 
	
        //...	
	
	};	
	
	
	//printf ("#breakpoint after window");
	//refresh_screen();
	//while(1){}
	
	*/
		
	// ## Messages ##
	
	printf("\n");
	//printf("[System Info:]\n");
	
	// OS info.
	// config/gramado.h
	
	printf("%s ", OS_NAME );
	printf("Version: %d.%d.%d \n",
	    SYSTEM_VERSION_MAJOR,
		SYSTEM_VERSION_MINOR,
		SYSTEM_VERSION_REVISION );
	
	// Screen resolution. (first of all)    
	printf("[Screen Resolution:]\n");
	printf("Width={%d} Height={%d}\n\n",
	    g_device_screen_width, g_device_screen_height );
		
	// Kernel info.
	// Version and time running.	
	//printf("[Kernel Info:]\n");
	printf("Kernel version: %s %s\n", SYSTEM_VERSION_STRING, SYSTEM_VERSION_NAME_STRING );
	printf("%d Hz | sys time %d ms | ticks %d \n", 
	    sys_time_hz, sys_time_ms, sys_time_ticks_total );
		
		
    //ShowUserInfo(int user_id);	

    // ## Status ##

	// Status do mecanismo de task switch.
	switch( task_switch_status )
	{
	    case LOCKED:
		    printf("Task switch is LOCKED\n");
			break; 
	    
		case UNLOCKED:
		    printf("Task switch is UNLOCKED\n");
		    break; 	
	};

	// Status do Scheduler.
	switch( g_scheduler_status )
	{
	    case LOCKED:
		    printf("Scheduler is LOCKED\n");
		    break; 
	    
		case UNLOCKED:
		    printf("Scheduler is UNLOCKED\n");
		    break; 
	};

		
	// Crit�rio de dispatch.
	// Mostra o n�mero de vezes que um crit�rio de sele��o 
	// de thread foi usado pelo dispatcher.
	
	printf("\n");
    printf("[Dispatch criteria:]\n");
	printf("cIdle={%d} cInit={%d} cNext={%d} cCur={%d} "
	       "cAny={%d} cIdeal={%d} cDisp={%d}\n\n",
		DispatchCountBlock->SelectIdleCount,
		DispatchCountBlock->SelectInitializedCount,
		DispatchCountBlock->SelectNextCount,
		DispatchCountBlock->SelectCurrentCount,
		DispatchCountBlock->SelectAnyCount,
		DispatchCountBlock->SelectIdealCount,
		DispatchCountBlock->SelectDispatcherQueueCount );
		
	printf("# thread info #\n");	
	printf("{ %d } threads_counter\n\n", ProcessorBlock.threads_counter );	
    
	show_thread_information ();		
		
    //
	// ## Heap and Stack ##
	//
	
    //Obs: Estou revendo isso.
	//printf("\n[Kernel Heap and Stack info:]\n");
	//printf("HEAP: Start={%x} | End={%x} | Total={%d KB} \n",
	//    kernel_heap_start ,kernel_heap_end ,HeapTotal);	
    //printf("AvailableHeap={%d KB}\n",g_available_heap/1024);
	    
	//@todo:
	//mostrar o tamanho da pilha..
	//#bugbug: A informa��es sobre a stack est�o incorretas, 
	//pois essas vari�veis mostram 
	// o endere�o da stack na hora da inicializa��o. 
	//Quando o processador retorna de ring3 para ring0
	//ele usa o endere�o de pilha indicado na TSS.
	//Pois bem, � mais digno mostrar aqui o endere�o da pilha, 
	//indicado no TSS.
	//printf("STACK: Start={%x} | End={%x} | Total={%d KB} \n",
	//    kernel_stack_start ,kernel_stack_end ,StackTotal);
    //printf("STACK: StartPhysicalAddress={%x} \n",kernel_stack_start_pa);
	
	//
	// ## Video ##
	//
	
	//printf("\n[Video Info:]\n");
	
	//Video mode.
	//printf("The video option is %x \n",g_video_mode);
	//printf("FrontbufferPA={%x} FrontbufferVA={%x} BackbufferVA={%x} \n",
	//    g_frontbuffer_pa ,g_frontbuffer_va ,g_backbuffer_va);
	
	//
	// ## User ##
	//
	
	printf("[User Info:]\n");
	
	//Group and user.
	printf("Group={%d} User={%d}\n", 
	    current_group, current_user );
	
	//user session, room, desktop.
	printf("UserSession={%d} Room={%d} Desktop={%d}\n",
		current_usersession, current_room, current_desktop );
															
	
	//#bugbug
	//@todo: Rever isso.
	ShowUserInfo(0);  
	
	
	//
	// ## Process and thread ##
	//
	
	printf("[Process Info:]\n");	
    
	//Current process and current thread.
	printf("CurrentProcess={%d} CurrentThread={%d}\n", 
	    current_process, current_thread );
	
    //
    //  ## Memory ##
    //	
	
	memoryShowMemoryInfo();
	
	
	//
	// ## Processor ##
	//
	
	//Espec�fico para intel.
	//Talves isso n�o deva ficar aqui.
	//show_cpu_intel_parameters();
	
	
	//
	// Disks and directories
	//
		
	//printf("\n[Disks and directories:]\n");
		
	//disk
	//printf("%s (system disk)\n",SYSTEM_DISK);
	//printf("%s (system volume)\n",SYSTEM_VOLUME);
		
	//directories
	//printf("%s\n",SYSTEM_GBUILD);
	//printf("%s\n",SYSTEM_GDOCS);
	//printf("%s\n",SYSTEM_GRAMADO);
	//printf("%s\n",SYSTEM_GSDK);
	//printf("%s\n",SYSTEM_GSRC);
	    
	//
	// Test.
	//
		
		
	//Tentando mostrar informa��es sobre a imagem do kernel.
	//o in�cio da �rea de c�digo e o in�cio da �rea de dados.
		
	/* #bugbug: os valores apresentados n�o foram os esperados.
	printf("KernelBegin={%x} Kernelend={%x}\n" ,kernel_begin ,kernel_end );
	printf("CodeBegin={%x} CodeEnd={%x} \n",code_begin ,code_end);
	printf("DataBegin={%x} DataEnd={%x} \n",data_begin ,data_end);
	printf("BSSBegin={%x} BSSEnd={%x} \n",bss_begin ,bss_end);
	*/
		
		
	//More?!
	
    //SetFocus(hWindow);
	
	/*
	
    //
    // ## Cursor ##
    //	
	
	// Voltando a margem normal a margem.
	g_cursor_left = left;       //0;
	g_cursor_top =  top;        //0;
	g_cursor_right  = (width/8);   
	g_cursor_bottom = (height/8);  
		
	//cursor (0, mas com margem nova)
	g_cursor_x = g_cursor_left; 
	g_cursor_y = g_cursor_top;
    //set_up_cursor(g_cursor_left,g_cursor_top); 		

    */
	
	//
	//  ## Status bar ##
	//
	
	//#bugbug:
	//N�o devemos criar uma status bar,
	//apenas atualizar as string s caso a janela 
	//tenha uma status bar.	

	//@todo criar fun��o.
	//UpdateStatusBar( hWindow, 
	//    "Esc=EXIT",
	//	"Enter=?" );		
	
	
	//Nothing.   	
    goto done;	
	
fail:
    printf("fail\n");	
done:
    //SetFocus(hWindow);
	
	// Talvez possamos da refresh apenas da janela 
	// onde as strings aparecem.	
    refresh_screen();
	return;	
};


/*
 Salva as informa��es em um arquivo de texto.
void infoSaveInfo();
void infoSaveInfo()
{
	return;
}
*/

/*
 * @todo:
 *
void infoShowKernelInfo();
void infoShowKernelInfo()
{
	return;
};
*/


//
// Fim.
//

