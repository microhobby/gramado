/*
 * File:  font.c 
 *
 *     Gerenciador de fontes.     
 */


#include <kernel.h>


void gwsSetCurrentFontAddress ( unsigned long address ){
	
	gws_currentfont_address = (unsigned long) address;
};


unsigned long gwsGetCurrentFontAddress (){
	
	return (unsigned long) gws_currentfont_address;
};


/* 
 * gwsInstallFont:
 *     Carregando e instalando uma fonte dado o nome. 
 */
 
int gwsInstallFont ( char *file_name ){
	
    unsigned long fileret;
	
	//#debug 
	//printf(" #### Loading font ####\n");
	
	//14KB	
	void *font_buffer = (void *) allocPageFrames(8);
	
	if ( (void *) font_buffer == NULL )
	{
	    panic ("gwsInstallFont: font_buffer\n");
		//die();		
	};

	//fileret = (unsigned long) fsLoadFile ( (unsigned char *) file_name, 
	//                            (unsigned long) font_buffer );

	fileret = (unsigned long) fsLoadFile ( VOLUME1_ROOTDIR_ADDRESS, 
	                            (unsigned char *) file_name, 
	                            (unsigned long) font_buffer );
								
	if ( fileret != 0 )
	{
		panic ("gwsInstallFont: fileret\n");		
		//die();
	};	
	
	//Configurando o endereço da fonte atual.
	gwsSetCurrentFontAddress( (unsigned long) (font_buffer + 0x2000) );	
		
	//#debug
	//printf("OK\n");	
    
	return (int) 0;	
};


//
// End.
//

