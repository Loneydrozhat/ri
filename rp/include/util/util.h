#ifndef _UTIL_H_

#define _UTIL_H_

#include <stdio.h> 
#include <sys/time.h>

/* define common functions */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a)	   (((a) < 0) ? -(a) : (a))

/* define common types */
#define TRUE 1
#define FALSE 0

#define REALLOC_SIZE 10

/* error codes */
#define OK            0         /* Sem erro                       */
#define E_NONE        0         /* Sem erro                       */
#define E_NOMEM     (-1)        /* Memoria insuficiente           */
#define E_FOPEN     (-2)        /* Nao consegue abrir arquivo     */
#define E_FREAD     (-3)        /* Erro na leitura do arquivo     */
#define E_FWRITE    (-4)        /* Erro na escrita do arquivo     */
#define E_OPTION    (-5)        /* Opcao desconhecida             */
#define E_OPTARG    (-6)        /* Opcao de argumento invalida    */
#define E_FRINEXIST (-7)        /* Arquivo de entrada inexistente */
#define E_UNKNOWN   (-8)        /* Erro desconhecido              */
#define E_NOTFOUND  (-9)        /* Elemento nao encontrado        */
#define E_PARSEFAIL (-10)       /* Problems parsing input file    */

#define MICROSEC 1000000.0

/* type definitions */
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8 ;

typedef int int32;
typedef short int16;
typedef char int8;
typedef char bool;

/* function definitions */
typedef void _DESTROY(void *value);
typedef uint32 _HASH_CODE(void *value);
typedef void* _CLONE(const void *value);

/* compare function defined according with the qsort compare */
typedef int32 _COMPARE(const void *value1, const void *value2);

void error (int, ...);

char *read_line(FILE*);

double elapsed_time(const struct timeval, const struct timeval);

#endif
