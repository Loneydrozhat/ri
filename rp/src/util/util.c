#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include <util/util.h>

//#ifndef _QUIET  
static const char *errmsgs[] = {      /* Mensagens de erro */
  /* E_NONE        0 */  "sem erro\n",
  /* E_NOMEM      -1 */  "memoria insuficiente\n",
  /* E_FOPEN      -2 */  "nao consegue abrir arquivo '%s'\n",
  /* E_FREAD      -3 */  "erro de leitura no arquivo '%s'\n",
  /* E_FWRITE     -4 */  "erro de escrita no arquivo '%s'\n",
  /* E_OPTION     -5 */  "opcao desconhecida -%c\n",
  /* E_OPTARG     -6 */  "opcao de argumento invalida -'%s'\n",
  /* E_FINEXIST   -7 */  "arquivo de entrada inexistente\n",
  /* E_UNKNOWN    -8 */  "erro desconhecido '%s'\n",
  /* E_NOTFOUND   -9 */  "elemento nao encontrado\n",
  /* E_PARSEFAIL -10 */  "parsing problems: '%s'\n"
};
//#endif

extern char *prgname;

void error (int code, ...) {                          
	va_list    args;   
	const char *msg;  

	if ((code > 0) || (code < E_UNKNOWN)) code = E_UNKNOWN;

	msg = errmsgs[-code];     
	fprintf(stderr, "\n%s: ", prgname);
	va_start(args, code);         
	vfprintf(stderr, msg, args); 
	va_end(args);                

	exit(code);
}

uint32 actual_line_size = 128;

char *read_line(FILE *file) {
	register uint32 size;
	char *line;
	
	line = (char*)calloc(actual_line_size,sizeof(char));
	if (line == NULL) error(E_NOMEM);

	fgets(line, actual_line_size, file);
	size = strlen(line);
	while (!feof(file) && line[size-1] != '\n') { 
		char *comp_line;
		uint32 new_size = (actual_line_size * 2) / 3;

		actual_line_size += new_size;

		comp_line = (char*)calloc(new_size,sizeof(char));
		if (comp_line == NULL) error(E_NOMEM);

		fgets(comp_line, new_size, file);

		line = (char*)realloc(line, actual_line_size*sizeof(char));
		if (line == NULL) error(E_NOMEM);

		strcat(line, comp_line);
		size = strlen(line);

		free(comp_line);
	}	
	
	if (feof(file) && size == 0) {
		free(line);
		line = NULL;
	} else {
		line[size-1] = '\0';
	}	
	
	return line;
}

double elapsed_time(const struct timeval tbegin, const struct timeval tend) {
	double begin, end;

	begin = tbegin.tv_sec + (tbegin.tv_usec/MICROSEC);
	end = tend.tv_sec + (tend.tv_usec/MICROSEC);

	return (double)(end - begin);
}
