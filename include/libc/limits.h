/*
 * File: limits.h 
 * 
 * Descri��o:
 *     Descreve alguns limites na linguagem C padr�o 32bit.
 *     @todo: Essa vers�o est� imcompleta.
 */

//
// # char support #
// 
 
//Number of bits in a 'char'.  
#define CHAR_BIT  8

//Minimum and maximum values a 'signed char' can hold.  
#define SCHAR_MIN  (-128)
#define SCHAR_MAX  127    
 
//Maximum value an 'unsigned char' can hold.  (Minimum is 0.).  
#define UCHAR_MAX  255

//
// # short support #
// 

//Minimum and maximum values a 'signed short int' can hold.  
#define SHRT_MIN  (-32768)
#define SHRT_MAX  32767
 
//Maximum value an 'unsigned short int' can hold.  (Minimum is 0.).  
#define USHRT_MAX  65535

//
// # int support #
// 

#define INT_MAX  2147483647
#define UINT_MAX	0xffffffff


//
// End.
//

