unsigned short int le2h_short(unsigned char *x)
{
	unsigned short int r;

	r = ((unsigned short int) x[0]) + 
	    ((unsigned short int) x[1]) * 0x0100; 

	return r;
}

unsigned short int be2h_short(unsigned char *x)
{
	unsigned short int r;

	r = ((unsigned short int) x[1]) + 
	    ((unsigned short int) x[0]) * 0x0100; 

	return r;
}

unsigned int le2h_int(unsigned char *x)
{
	unsigned int r;

	r = ((unsigned int) x[0]) + 
	    ((unsigned int) x[1]) * 0x00000100 +
	    ((unsigned int) x[2]) * 0x00010000 + 
	    ((unsigned int) x[3]) * 0x01000000;

	return r;
}

unsigned int be2h_int(unsigned char *x)
{
	unsigned int r;

	r = ((unsigned int) x[3]) + 
	    ((unsigned int) x[2]) * 0x00000100 +
	    ((unsigned int) x[1]) * 0x00010000 + 
	    ((unsigned int) x[0]) * 0x01000000;

	return r;
}

void h2le_short(unsigned short int r, unsigned char *x)
{
	x[0] = r % 0x00000100;
	x[1] = (r / 0x00000100) % 0x00000100;
}

void h2be_short(unsigned short int r, unsigned char *x)
{
	x[1] = r % 0x00000100;
	x[0] = (r / 0x00000100) % 0x00000100;
}

void h2le_int(unsigned short int r, unsigned char *x)
{
	x[0] = r % 0x00000100;
	x[1] = (r / 0x00000100) % 0x00000100;
	x[2] = (r / 0x00010000) % 0x00000100;
	x[3] = (r / 0x01000000) % 0x00000100;
}

void h2be_int(unsigned short int r, unsigned char *x)
{
	x[3] = r % 0x00000100;
	x[2] = (r / 0x00000100) % 0x00000100;
	x[1] = (r / 0x00010000) % 0x00000100;
	x[0] = (r / 0x01000000) % 0x00000100;
}
