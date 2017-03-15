#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "_endian.h"
#include "basic.h"

static int basic_tokenize(char *line, int *number, char **content) 
{
	int r,l,n=0;

	char *nstr, *cstr;

	/* skip leading whitespace */
	while(line[n]!=0 && (line[n]==' ' || line[n]=='\t')) n++;

	if(line[n]==0) {
		fprintf(stderr, "No BASIC line number found\n");
		return -1;
	}

	nstr=&line[n];

	/* find end of line number */
	while(line[n]!=0 && (line[n]!=' ' && line[n]!='\t')) n++;

	line[n]=0;

	if(strlen(nstr)<1) {
		fprintf(stderr, "No BASIC line number found\n");
		return -1;
	}

	n++;

	/* skip whitespace between line number and line contents*/
	while(line[n]!=0 && (line[n]==' ' || line[n]=='\t')) n++;

	if(line[n]==0) {
		fprintf(stderr, "Empty BASIC line\n");
		return -1;
	}

	cstr=&line[n];

	/* find end of line content */
	while(line[n]!=0 && line[n]!='\n') n++;

	line[n]=0;

	if(strlen(cstr)<1) {
		fprintf(stderr, "Empty BASIC line\n");
		return -1;
	}

	r=sscanf(nstr, "%d", &l);
	if(r!=1) {
		fprintf(stderr, "BASIC line number not an integer\n");
		return -1;
	}

	*number=l;
	*content=strdup(cstr);

	return 0;
}

static int basic_parse(char *in, char **out, int *outlen)
{
	int r,l,destlen,n;
	char *c, *dest;
	unsigned short int ls; 

	r=basic_tokenize(in, &l, &c);
	if(r) {
		fprintf(stderr, "Nonsense in BASIC\n");
		return -1;
	}

	if(l & ~0xffff) {
		fprintf(stderr, "BASIC line number out of range\n");
		free(c);
		return -1;
	}

	for(n=0;n<strlen(c);n++) {
		c[n]=toupper(c[n]);
		if(c[n]<' ' || c[n] > '_') {
			fprintf(stderr, "BASIC line contains invalid characters\n");
			free(c);
			return -1;
		}
	}

	destlen=strlen(c)+3;
	dest=malloc(destlen);
	if(dest==NULL) {
		fprintf(stderr, "Can't allocate memory\n");
		free(c);
		return -1;
	}

	ls=l;

	h2le_short(ls,(unsigned char*) dest);

	strcpy(&dest[2],c);

	dest[strlen(c)+2]='\r';

	*out=dest;
	*outlen=destlen;

	free(c);

	return 0;
}

int basic_read(char *filename, char **data, int *datalen)
{
	FILE *f;
	char buff[MAX_LINE_LEN];
	char *dest=NULL, *line;
	int ln,r,destlen=0,linelen;

	f=fopen(filename, "r");
	if(f==NULL) {
		fprintf(stderr, "Can't open '%s': %s\n", filename, 
							strerror(errno));
		return -1;
	}

	ln=1;
	while(fgets(buff, MAX_LINE_LEN, f)!=NULL) {
		r=basic_parse(buff, &line, &linelen);
		if(r) {
			fprintf(stderr, "At line %d\n", ln);
			fclose(f);
			if(dest!=NULL) free(dest);
			return -1;
		}

		if(dest==NULL) {
			dest=line;
			destlen=linelen;
		} else {
			dest=realloc(dest, destlen+linelen);
			memcpy(&dest[destlen],line,linelen);
			free(line);
			destlen=destlen+linelen;
		}
		ln++;
	}

	fclose(f);

	if(dest==NULL) {
		fprintf(stderr, "Empty BASIC file\n");
		return -1;
	}

	*data=dest;
	*datalen=destlen;

	return 0;
}
