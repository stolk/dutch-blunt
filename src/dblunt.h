// dblunt.c
// Dutch-Blunt low poly font system.
// (c)2015-2020 Abraham Stolk

#if defined(_MSC_VER)
#	define __restrict__ __restrict
#endif


extern int dblunt_string_to_vertices
(
	const char* str,		//!< characters to render.
	float* destbuf,			//!< output buffer to hold vertices.
	int destbufsz, 			//!< size of output buffer in bytes.
	float posx, float posy,		//!< target position for text render.
	float sclx, float scly,		//!< scale of the text render.
	float fadestart,		//!< if >=0, start fading out at this character number and write opactities.
	float* __restrict__ textw,	//!< out: size of longest line.
	float* __restrict__ texth	//!< out: height of text.
);


extern int dblunt_attributed_string_to_vertices
(
	const char* str,		//!< characters to render.
	const char* att,		//!< character attributes, like superscript/subscript.
	float* destbuf,			//!< output buffer to hold vertices.
	int destbufsz, 			//!< size of output buffer in bytes.
	float posx, float posy,		//!< target position for text render.
	float sclx, float scly,		//!< scale of the text render.
	float* __restrict__ textw,	//!< out: size of longest line.
	float* __restrict__ texth	//!< out: height of text.
);

