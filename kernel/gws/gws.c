/*
 * Gramado GUI - The main file for the GUI layer.
 * (c) Copyright 2015~2018 - Fred Nora.
 */
 
 
// ## GWS - Gramado Window Server ##

// Aplicativo poder�o se conectar com o servidor de recusros
// gr�ficos, principalmente a GUI, que ser� chamada de 
// Gramado Window Manager.
// Outros window managers poder�o ser criados, e eles precisar�o 
// ser registrados e abrirem o servidor para usarem seus recursos.
// do mesmo modo o shell atual poder� se registrar,
// o processo de logon poder� se registrar em um servidor de logon.
//


#include <kernel.h>



// unsigned char  gws_bame[] = "GWS - Gramado Window Server";
// unsigned char  GramadoName[] = "GRAMADO LAYER";

//Status do gws.
int gws_status;

//Qual � o id do processo que � o atual window manager.
//o oficial � o Gramado Window Manager, hoje 
//chamado de shell.bin
int gws_wm_PID;
int gws_wm_status;



//
// cursor
//


/*
void gwsEnableTextCursor (){
	
    g_show_text_cursor = 1;	
};

void gwsDisableTextCursor (){
	
    g_show_text_cursor = 0;	
};
*/ 
 
//abrir o servidor de janelas. 

int gwsOpen (){
	
	//Aberto.
	gws_status = 1;
	//..
	
	return (int) 0;
};


//fechar o servidor de janelas
int gwsClose (){
	
	gws_status = 0;
	
	return (int) 0;
};


// Registrar um window manager.
int gwsRegisterWindowManager ( int pid ){
	
	int Status = 0;
	
	if( gws_status != 1 )
	{
		Status = 1;
		goto fail;
	}else{
		
	    gws_wm_PID = (int) pid;	
	    gws_wm_status = 1;
		goto done;
	};
	
fail:
    printf("gwsRegisterWindowManager: fail\n");	
done:
    return (int) 0;		
};
 


//internas
void gui_create_screen();
void gui_create_background();
void gui_create_logo();
void gui_create_taskbar();
void gui_create_mainwindow();
void gui_create_controlmenu();
void gui_create_infobox();
void gui_create_messagebox();
void gui_create_debug();
void gui_create_navigationbar();
void gui_create_grid();

	
	
/* 
 * create_gui:
 *     Cria as janelas principais: Screen, Background,Logo ...
 *     Cria Window Station, Desktop, Windows, Menu ...
 *     @todo: guiCreate();
 *
 * Obs: esse � o user environment. 
 *(deixe o desenvolvedor saber em qual desktop ele est�.)
 */
 
void create_gui (){
	
	//struct window_d *hWindow; 	 

	//
	// Set minimal gui set up!.
	//
	
	//g_guiMinimal = 1;
	
	
    //	
	// Initializing 'gui' structure.
	// Obs: Essa estrutura foi antes usada pelo Logon.
	//      Agora a GUI reinicializa.
	//      Todos os parametros da estrutura precisam ser inicializados.
	// @todo: Estamos reinicializando tudo, recriando estruturas 
	//        que devem ser deletadas na finaliza��o do logon. @todo.
    // 		
	
	gui = (void *) malloc ( sizeof(struct gui_d) );
	
    if ( (void *) gui == NULL )
	{	
	    panic ("gws-create_gui:");
		
	} else {
		
		// Set user session, room (Window Station), desktop. 
		// window and menu.
		
		current_usersession = 0;        
	    current_room = 0;             
	    current_desktop = 0;        
		
	    current_window = 0;         
	    current_menu = 0;           
		
		
		// @todo: O logon devveria ter selecionado o perfil do usu�rio
		//        e essa rotina n�o deveria ser chamada.
				
		init_user_info ();
		
		// Em ordem:
		// user section, window station and desktop.
		// windows and menus.
		
		
#ifdef KERNEL_VERBOSE		
		printf("create_gui: User Session..\n");
#endif
		init_user_session ();
		
#ifdef KERNEL_VERBOSE				
		printf("create_gui: Window Station..\n");
#endif
		init_room_manager();
	 
#ifdef KERNEL_VERBOSE			 
		printf("create_gui: Desk..\n");
#endif
	    init_desktop ();	
	 
#ifdef KERNEL_VERBOSE			 
		printf("create_gui: Wind..\n");
#endif	    
		init_windows ();	
	 
#ifdef KERNEL_VERBOSE			 
		printf("create_gui: Menu..\n");
#endif	    
		init_menus ();
		
		//
		// System MenuBar: (Barra no topo da tela). 
		//
	
#ifdef KERNEL_VERBOSE			
		printf("create_gui: System Menu bar..\n");
#endif		
		systemCreateSystemMenuBar();
		
		//
		// Continua ...
        //
		
    };
	
	
	//
	// Configura quais janelas devem ser pintadas.
	//
	
    if(g_guiMinimal == 1)
	{		

        SetGuiParameters( 0,    //Refresh.         
                          1,    //*Screen.          
                          1,    //*Background.       
                          0,    //Logo.            
                          1,    //*Taskbar. #test.        
                          1,    //*Main.(Desktop window, �rea de trabalho) com base na taskbar.             
                          0,    //Menu.           
                          0,    //Info Box.         
                          0,    //Message Box.       
                          0,    //Debug.           
                          0,    //Navigation Bar.    
                          0 );  //Grid. #test.	
						  
	}else{
		
        SetGuiParameters( 0,    //Refresh.         
                          1,    //*Screen.          
                          1,    //*Background.       
                          0,    //Logo.            
                          1,    //*Taskbar. #test.        
                          1,    //*Main.(Desktop window, �rea de trabalho) com base na taskbar.             
                          0,    //Menu.           
                          0,    //Info Box.         
                          0,    //Message Box.       
                          0,    //Debug.           
                          0,    //Navigation Bar.    
                          0 );  //Grid. #test.	
						   
	};
  	
	
	//#importante.
	
	if ( (void *) gui == NULL ){
		
		panic ("gws-create_gui: gui struct");
	};
	
	
	//Grupo1:
	//Base windows:
	//screen,background,taskbar
	
	//grupo2
	//main (com base na screen e na taskbar.)
	
	//grupo3
	//outros.

	
//creatingWindows:
	
    	
	//Grupo1:
	//Base windows:
	//screen,background,taskbar
	
	if (gui->screenStatus == 1){ 
	    gui_create_screen(); 
	};
	
	if (gui->backgroundStatus == 1){	
	    gui_create_background(); 
	};

	if (gui->taskbarStatus == 1){	
	    gui_create_taskbar(); 
	};

	//grupo2
	//main (com base na screen e na taskbar.)
	
	
    if (gui->mainStatus == 1){ 
	    gui_create_mainwindow(); 
	};

	//grupo3
	//outros.

	
	if (gui->logoStatus == 1){ 
	    gui_create_logo(); 
	};	
	
	
    if (gui->messageboxStatus == 1){ 
	    gui_create_messagebox(); 
	};
	
    if (gui->menuStatus == 1){ 
	    gui_create_controlmenu(); 
	};	

    if (gui->navigationbarStatus == 1){ 
	    gui_create_navigationbar(); 
	};
	
	// Grid: Dentro do grid haver� sempre 3 partes: 
	//       Header | Content | Footer.
	//       Obs: @todo: O grid deve levar em considera��o a
	// �rea que lhe pertence, ou seja a �rea de trabalho.  
	
    if (gui->gridStatus == 1){ 
	    gui_create_grid(); 
	};
	
	
//drawingStrings:
	
	// Strings.
	// String no background.
    // Obs: Informa��es de ambiemte para o desenvolvedor.
	//@todo: Informar o desktop atual.
	
	if ( (void *) CurrentUser != NULL )
	{
        if (g_guiMinimal != 1)
	    {				
		    draw_text ( gui->main, 0, 0, COLOR_WHITE, "User Environment");
		    draw_text ( gui->main, 0, 8, COLOR_WHITE, "================");
			draw_text ( gui->main, 8*14, 8*4, 
			    COLOR_WHITE, "Welcome to User Environmet, press F1=HELP");
			draw_text ( gui->main, 8*14, 8*5, COLOR_WHITE, 
			    "This is the user environment");
			//...
			
			//Outros:
			//StatusBar( gui->main, "Dektop", "GUI");
			//Message box
			//...
	    };
		
		//Outras op��es ?
		
		//Nothing.
	};	
	
	//
	// Testando client area.
	//
	//draw_text(gui->ClientArea, 8, 8, COLOR_WHITE, ".Desktop");
	//draw_text(gui->screen, 8, 8, COLOR_WHITE, ".Desktop");
	//draw_text(gui->screen, 1*(480/8), 1*(640/8), COLOR_WHITE, ".Desktop"); 
    //StatusBar( gui->screen, "StatusBar: ", "Barra de status na area de trabalho.");
	//

	
	//
	// @todo: More ?!!
	//
	
//done:

	//Refresh.
    //(O 1 avisa que � necess�rio dar refresh pois foram efetuadas rotinas de pintura).	
	if ( gui->refresh == 1 )
	{
		refresh_screen();
		
		//Status.
		gui->refresh = 0;  
	};
};



/*
 * SetGuiParameters:
 *     Configura a inicializa��o das janelas gerenciadas pelo kernel.
 *     @todo: Limite de janelas (+- 10)
 *
 */
void SetGuiParameters(
int refresh,         //Flag. Colocar ou n�o o backbuffer na tela.
int screen,          //Tela do computador.
int background,      //Background da area de trabalho. 
int logo,            //Logo, Janela para imagem da �rea de trabalho.
int taskbar,         //Barra de tarefas.(�cones para programas).
int main,            //Janela principal.(Desktop, ou Kernel Setup Utility). 
int menu,            //Control menu da �rea de trabalho.
int infobox,         //Janelinha no canto, para alerta sobre sobre eventos ocorridos no sistema.
int messagebox,      //O message box do kernel.(Cada app cria seu messagebox). 
int debug,           //Janela do debug do kernel.(sonda por problemas).
int navigationbar,   //Janela de navega��o(Semelhante a navigation bar dos dispositivos mobile). 
int grid             //Grid da janela principal.
//Limites ...
)
{ 
    //Passados via argumento.
	
	//
	//@todo: gui status. fail or not.
	//
	
	//Checa validade da estrutura.
	
	if ( (void*) gui == NULL ){
		return;
	}else{	
	    gui->refresh = refresh; 
	    gui->screenStatus = screen;
	    gui->backgroundStatus = background; 
	    gui->mainStatus = main;
        gui->logoStatus = logo; 		
	    gui->taskbarStatus = taskbar;
	    gui->menuStatus = menu;
	    gui->infoboxStatus = infobox;
	    gui->messageboxStatus = messagebox;
	    gui->debugStatus = debug;	
	    gui->navigationbarStatus = navigationbar; 
	    gui->gridStatus = grid;
		//...
    };

    //
    // @todo: More ?!!
    //	
	
//done:
    //return;
};


/*
 * gui_create_screen:
 *     Cria a tela. (Screen)
 *     Atribuindo as dimens�es.
 *     ...
 * @todo: Cria buffer dedicado.
 */
void gui_create_screen()
{
    struct window_d *hWindow; 
	
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();

	//
	// * IMPORTANTE: N�o tem Parent Window!
	//
	
	if( (void*) gui == NULL ){
        return;
    };		
	
	//Window:
	//Pintado uma janela simples, toda preta, do tamanho da tela 
	//do sispositivo.
	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "Screen",
	                               Left, Top, Width, Height,
						           0, 0, 0, COLOR_BLACK );     
	if( (void*) hWindow == NULL ){
	    printf("gui_create_screen:");
		die();
	}else{

		RegisterWindow(hWindow);
		set_active_window(hWindow); 
		
		//Isso impede que rotinas mudem as caracter�scicas 
		// da janela principal sem antes destravar ou sem 
		//ter autoriza��o para isso.
		windowLock(hWindow); 
		
		//Estrutura gui.
	    if( (void*) gui != NULL ){
	        gui->screen = (void*) hWindow;
		};
		
	    //Desktop.
		//a janela pertence ao desktop 0
	    //hWindow->desktop = (void*) desktop0;
		
		//Nothing.
	};
	
	// @todo: More ??!!
done:
    //bugbug: N�o usar set focus nessa que � a primeira janela.
    return; 
};


/*
 * gui_create_background:
 *     Cria o background. Pinta ele de uma cor.
 *     O background � filha da janela screen.
 *
 * Criando um pano de fundo do mesmo tamaho da tela,
 * Penso que � nessa janela que podemos carregar uma imagem de 
 * pano de fundo.
 *
 * @todo: Cria buffer dedicado.
 */
void gui_create_background()
{ 
    struct window_d *hWindow; 
	
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	
	
	if( (void*) gui == NULL ){
        return;
    };		
	
	//Window:
	//0x00808000 (verde)
	//Criando um pano de fundo do mesmo tamaho da tela,
	//Penso que � nessa janela que podemos carregar uma imagem de 
	//pano de fundo.
	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "Background", 
		                            Left, Top, Width, Height,
							        gui->screen, 0, 0, COLOR_BACKGROUND);  
	if( (void*) hWindow == NULL )
	{
	    printf("gui_create_background:");
		die();
	}else{
	    
		RegisterWindow(hWindow);
		set_active_window(hWindow); 
		windowLock(hWindow);  
		
        //Estrutura gui.
		if( (void*) gui != NULL ){
	        gui->background = (void*) hWindow;
		};
		
		//Desktop.
	    //a janela pertence ao desktop 0
	    //hWindow->desktop = (void*) desktop0;
		
		//Nothing.
	};
	
done:
	SetFocus(hWindow);
    return; 
};



/*
 * gui_create_taskbar:
 *      Cria a taskbar da �rea de trabalho. gui->taskbar.
 *      Cria somente a janela que ser� uada pelo file manager.
 * e que servir� de refer�ncia para a janela gui->main
 *
 */
void gui_create_taskbar()
{ 
	struct window_d *hWindow; 

	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	
	
	if( (void*) gui == NULL ){
        return;
    };
    	
draw_bar:	

    Height = (Height/8);

	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "taskbar", 
	                                Left, Top, Width, Height, 
							        gui->screen, 0, 0, COLOR_WINDOW );
									
	if( (void*) hWindow == NULL)
	{
	    printf("gramado-gramado-gui_create_taskbar:\n");
		die();
	}
	else
	{
	    RegisterWindow(hWindow);
	    windowLock(hWindow); 
	    
		if( (void*) gui != NULL ){
            gui->taskbarStatus = (int) 1;
			gui->taskbar = (void*) hWindow;
	    };
		
		//...
	};
	
	//...
	
done:	
	return; 
};


/*
 * gui_create_mainwindow:
 *      A �rea de trabalho.
 *      *Importante: � a �rea dispon�vel na tela para o aplicativo. 
 *      Onde ficam os �cones.
 *      
 *     A �rea de trabalho � toda a tela menos a GMB(Global Menu Bar) e menos a TaskBar. 
 * @todo: Criar o buffer dedicado para a janela principal.
 *
 *     Obs: Essa janela � especial e usar� o Backbuffer como buffer dedicado.
 */
void gui_create_mainwindow()
{
    struct window_d *hWindow; 
	
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();	
	
	//estrutura gui.
	if( (void*) gui == NULL )
	{
        printf("gui_create_mainwindow: gui\n");
        die();
	};

	//janela taskbar.
	if( (void*) gui->taskbar == NULL )
	{
        printf("gui_create_mainwindow: taskbar\n");
        die();
	};
	
	//
	// Usando a janela taskbar como refer�ncia para criar a janela main.
	//
	
	//Top = gui->taskbar->bottom;
	
	
	//Window:
	//Color? (?? Nem precisa pintar, mas precisa criar ??)
	//�rea de trabalho.
	//Um ret�ngulo que pode ser menor que o tamanho da tela do 
	//dispositivo.
	//� onde ficam as janelas dos aplicativos.
	//A janela principal perence ao desktop
	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "gramado desktop window", 
	                                Left, Top, Width, Height,           
							        gui->screen, 0, 0, COLOR_WINDOW );   
	if( (void*) hWindow == NULL){
	    printf("gramado-gui_create_mainwindow:\n");
	    die();
	}else{   
	    		
	    RegisterWindow(hWindow);
	    windowLock(hWindow); 
		set_active_window(hWindow); 
		
	    //a janela pertence ao desktop 0
	    //hWindow->desktop = (void*) desktop0;
		
		if( (void*) gui == NULL ){
		    return;
		}else{
	        gui->main = (void*) hWindow;
		};	
	};
	
	
	//
	//  Desktop Window:
	//      Criar a janela gui->desktop.
	//      Na verdade a janela do desktop
	//      � a janela da �rea de trabalho.
	// @todo:
	// #bugbug: Confunde-se com a estrutura de desktop_d.
	//
	//
	
    /*	
	gui->desktop = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "Desktop Window", 
	                                0, 16, 800, 600-16-16, 
							        gui->screen, 0, 0, COLOR_WINDOW ); 
	*/
	
	gui->desktop = (void*) gui->main;
	
	if( (void*) gui->desktop == NULL){
		
		gui->desktop = (void*) gui->screen;
		
		if( (void*) gui->desktop == NULL){ 
		    goto done;
		};
	};
	
	//N�o registrar pois main menu ja est� registrada.
	//RegisterWindow(gui->desktop);
	
done:
    SetFocus(hWindow);		
    return; 
};




/*
 * gui_create_logo:
 *     Cria a janela para o logo da �rea de trabalho. O logo da �rea de 
 * trabalho � a imagem da �rea de trabalho.
 *
 * @todo: Cria buffer dedicado.
 */
void gui_create_logo()
{ 

/*
 * #suspensa
 
    struct window_d *hWindow; 
	
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();
	
	if( (void*) gui == NULL ){
        return;
    };		
	
	//Window:
	//White. Pequena no meio. Pode ser usada na inicializa��o.
	//O logo pertence ao desktop.
	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "Logo", 
	                                (Width/3), (Height/3), (Width/3), (Height/3), 
							        gui->screen, 0, 0, COLOR_WINDOW );
	if( (void*) hWindow == NULL )
	{
	    printf("gui_create_logo:");
	    die();
	}else{
	    		
		RegisterWindow(hWindow);
		set_active_window(hWindow); 
	    windowLock(hWindow); 
		
		//Estrutura gui.
		if( (void*) gui != NULL ){
	        gui->logo = (void*) hWindow;
		};
		
		//Desktop.
	    //A janela pertence ao desktop0.
	    //hWindow->desktop = (void*) desktop0;
		
		//Nothing.
	};

done:
	SetFocus(hWindow);	
	
 */
    return; 
};



/*
 * gui_create_controlmenu:
 *     Esse � o controlmenu da area de trabalho.
 * mesmo que nao esteja rodando o programa gerenciador de
 * deprogramas esse control menu existir�.
 *
 */
void gui_create_controlmenu()
{ 

/*
 * #suspensa
 
    struct window_d *cmWindow;
	
	if( (void*) gui == NULL ){
        return;
    };	

	//Usando a janela anteriormente criada.
    if( (void*) gui->screen == NULL ){
        return;
    };	
	
	//
    // Cria a janela e o menu. 
	// (o argumento � a janela mae)
    //	
	
    cmWindow = (void *) create_menu(gui->screen,2,0,0,0);
	if( (void *) cmWindow == NULL ){
	    printf("gui_create_controlmenu:");
		return;
	}else{
	    
		//configura a janela do menu do sistema.
	    set_current_menuwindow(desktop0, cmWindow);
		
		//configura o menu de sistema a ser usado no desktop.
		set_current_sysMenu(desktop0, cmWindow->defaultMenu);
		
		//itens.(test)
	    initmenuArray(cmWindow->defaultMenu,2);	
	    create_menu_item(cmWindow->defaultMenu,"cmitem0",0);
        create_menu_item(cmWindow->defaultMenu,"cmitem1",0);
        //...

        //gui->menu = (void*) cmWindow; //janela do control menu.		
	};			
	
done:
    SetFocus(cmWindow);
    refresh_screen();
	
*/
    return; 
};


/*
 *gui_create_infobox:
 * janelinha de informa��es do sistema.
 * avisa o usuario sobre eventos do sistema.
 *como novos dispositivos encontrados.
 * @todo: Cria buffer dedicado.
 */
void gui_create_infobox()
{ 
	if( (void*) gui == NULL ){
        return;
    }else{
		
		gui->infobox = NULL;
	};	    
	
done:
    return; 
};


/*
 * Cria a janela para message box.
 */
void gui_create_messagebox()
{ 
	if( (void*) gui == NULL ){
        return;
    }else{
		
		gui->messagebox = NULL;
	};	    
	
done:
    return; 
};


/*
 * cria uma janela para debug do kernel. ser� um progama em kernel mode.
 * @todo: Cria buffer dedicado.
 */
void gui_create_debug()
{ 
	if( (void*) gui == NULL ){
        return;
    }else{
		
		gui->debug = NULL;
	};	    

	//
    // @todo: Efetuar� ou n�o algum tipo de debug de inicializa��o ou
    //	      dos elementos b�sicos do sistema. Essa tela de debug
	//        pode se mostrar uma op��o importante para o desenvolvedor.
	//        Mostraria um log de acertos e problemas durante o processo 
    //        de inicializa��o do sistema operacional.
	//
	
done:
    return; 
};

	
/*
 * gui_create_navigationbar:
 *     Barra de navega��o. 
 *     Semelhante �s barras encontradas nos dispositivos mobile. Usada para 
 * navega��o simples. Fica em baixo e deve ser escura.
 *
 *	   Obs: A navigation bar pertence a janela principal. gui->screen.
 *     Obs: A navigation bar poder� mudar para dentro da janela do shell.
 * que funciona como um navegador.
 *  
 * @todo: Cria buffer dedicado.
 */
void gui_create_navigationbar()
{
	
/*
 * #suspensa
 
	struct window_d *hWindow;  
	
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();

	if( (void*) gui == NULL ){
        return;
    };	    
	
	hWindow = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "NavigationBar", 
	                                Left, Height-(8*5), Width, (8*5), 
							        gui->screen, 0, 0, COLOR_BLACK);  
	if((void*) hWindow == NULL){
	    MessageBox(gui->screen,1,"ERRO","Navigation Bar fail!");
	}else{
	    	    
	    RegisterWindow(hWindow);
	    windowLock(hWindow); 
		set_active_window(hWindow); 

	    
		//hWindow->procedure = NULL;
		
		//@todo:
		//a janela pertence ao desktop 0
	    //hWindow->desktop = (void*) desktop0;
		
		if( (void*) gui == NULL ){
		    return;
		}else{
	        gui->navigationbar = (void*) hWindow;
		};

	};
    //Nothing.

// Buttons.
draw_buttons:
	//Back.
    draw_button( gui->navigationbar, "<",  1, 
	             1*(Width/16), 8, 24, 24, COLOR_BLACK);				   
	//Home.
    draw_button( gui->navigationbar, "O", 1, 
	             2*(Width/16), 8, 24, 24, COLOR_BLACK);	
	//TaskList, (lista os processos).
    draw_button( gui->navigationbar,  "=", 1, 
	             3*(Width/16), 8, 24, 24, COLOR_BLACK);	
	
    // Text. 
    //draw_text( gui->navigationbar, 1*(480/8), 8, COLOR_WINDOWTEXT, ".Desktop"); 	
	
	

//    * Text.   
//    if( (void*) gui->navigationbar != NULL )
//	{
//        //draw_text( gui->navigationbar, 1*(800/8), 8, COLOR_WINDOWTEXT, "/WindowStation0/Desktop0 GUI");
//        //draw_text( gui->navigationbar, 5*(800/8), 8, COLOR_WINDOWTEXT, "F1=HELP");
//	};	


    // Continua ...
    
done:	
    SetFocus(hWindow); 

*/    
	return; 
};


/*
 * gui_create_grid:
 *     Apenas autoriza a utiliza��o de um grid na area de trabalho. 
 *     Havendo essa altoriza��o, o gerenciador de arquivos poder�
 *     desenh�-lo. 
 *     cada item ser� um arquivo da pasta "area de trabalho".
 * @todo: Cria buffer dedicado.
 */
void gui_create_grid()
{ 
	if( (void*) gui == NULL ){
        return;
    }else{
		
		//initScreenGrid();	
		gui->grid = NULL;
	};	    


done:
    return; 
};


//screen
//m�trica do dispositivo.
void *guiGetScreenWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->screen;
};


//developer screen
void *guiGetDeveloperScreenWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->DEVELOPERSCREEN;
};


//background
void *guiGetBackgroundWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->background;
};


//logo
void *guiGetLogoWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->logo;
};


//desktop window
void *guiGetDesktopWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->desktop;
};


//task bar, top window, mac style
void *guiGetTaskbarWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->taskbar;
};



//main window
//GetDesktopWindow
void *guiGetMainWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->main;
};

//status bar window (do kernel base)
void *guiGetStatusbarWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->statusbar;
};


//grid window (do kernel base)
void *guiGetGridWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->grid;
};


//janela do 'control menu' atual.
void *guiGetMenuWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->menu;
};


//infobox (o sistema envia mensagens de informa��o)
void *guiGetInfoboxWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->infobox;
};

//tooltip  Janelinha que aparece quando repousa o mouse sobre elemento gr�fico.
void *guiGetTooltipWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->tooltip;
};


//message box do kernel base. (especial)
void *guiGetMessageboxWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->messagebox;
};


//dialogbox do kernel base. (especial)
void *guiGetDialogboxWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->dialogbox;
};


//janela de debug usada pelo kernel base.
void *guiGetDebugWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->debug;
};


//menubar usada pelo kernel base.
void *guiGetMbhWindowWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->mbhWindow;
};


//top bar de verdade, estilo mac ou mobile.
void *guiGetTopbarWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->topbar;
};


//navigation bar, estilo mobile.
//gerenciada pelo kernel base.
void *guiGetNavigationbarWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->navigationbar;
};


//janela do shell do kernel base.
//na� � um processo.
void *guiGetShellWindowWindow()
{
	if( (void*) gui == NULL){
		return NULL;
	}
	return (void*) gui->shellWindow;
};


//uma janela filha do shell do kernel base.
void *guiGetShellClientWindowWindow()
{
	if( (void*) gui == NULL ){
		return NULL;
	}
	return (void*) gui->shellClientWindow;
};


// reposiciona e muda o tamanho da gui->main window.
// configura a �rea de trabalho.
void guiSetUpMainWindow( unsigned long x, 
                         unsigned long y, 
						 unsigned long width, 
						 unsigned long height )
{
	unsigned long Left = (unsigned long) SCREEN_DEFAULT_LEFT;
	unsigned long Top  = (unsigned long) SCREEN_DEFAULT_TOP;
	unsigned long Width = (unsigned long) screenGetWidth();
	unsigned long Height = (unsigned long) screenGetHeight();		
		
	//check limits
	if( x > Width || y > Height ){ 
	    return; 
	}
	
	if( width > Width || height > Height ){ 
	    return; 
	}

	if( (void*) gui == NULL ){
		return;
	}	
	
	if( (void*) gui->main == NULL ){
		return;
	}else{
		
	    replace_window(gui->main, x, y);
	    resize_window( gui->main, width, height);		
	};	
	//Nothing.
};

/*
 * init_gui:
 *     Inicializa a GUI.
 *     � preciso criar a GUI antes de inicializar.
 *     @todo: Mudar para guiInit().
 * Obs: Nada alem de recursos gr�ficos. 
 */
int init_gui (){
	
	// ??
	// Habilita taskswitch e scheduler.
	set_task_status(UNLOCKED);
    scheduler_unlock();	 	
		
	//SetProcedure( (unsigned long) &system_procedure);	
	
	//
	// A partir de agora o scheduler pode procurar por threads 
	// no estado 'Initialized' e executar.
	//	


//done:

    //?? erro fatal ??
    if( (void*) gui == NULL )
	{
		printf("init_gui:\n"); 
		//gui->initialised = 0;
		//guiStatus = 0;
		return (int) 1;    //Fail.
	};

    gui->initialised = 1;	
    guiStatus = 1;	
	return (int) 0;
};


/*
int guiInit()
{};
*/

//deletar
void gramado(){
	
    return; 	
}

//deletar
int init_gramado(){
	
	return 0;
}

/*
int gwsInit();
int gwsInit()
{
	printf("GWS:")
	//#todo:
	
}
*/


//
// End.
//

