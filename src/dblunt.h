// dblunt.c
// Dutch-Blunt low poly font system.
// (c)2015 Abraham Stolk

extern int dblunt_string_to_vertices
(
	const char* str,
	float* destbuf,
	int destbufsz, 
	float posx, float posy,
	float sclx, float scly,
	int* numlines,
	int* maxlinelen
);

