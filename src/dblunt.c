// dblunt.c
// Dutch-Blunt low poly font system.
// (c)2015 Abraham Stolk

#include "dblunt.h"	// our own interface.

#include <assert.h>	// paranoia helps.
#include <string.h>	// string handling.

#include "vdata.h"	// the font data.


/*!
 * Convert an ascii string to a stream of vertex data.
 * Returns the number of triangles(x,y,x,y,x,y) written to the buffer.
 */
int dblunt_string_to_vertices
(
	const char* str,		//!< characters to render.
	float* destbuf,			//!< output buffer to hold vertices.
	int destbufsz, 			//!< size of output buffer in bytes.
	float posx, float posy,		//!< target position for text render.
	float sclx, float scly,		//!< scale of the text render.
	int* numlines,			//!< out: nr of lines to be rendered.
	int* maxlinelen			//!< out: longest line to be rendered.
)
{
	int trias_written = 0;
	float* buf_writer = destbuf;
	const int tri_capacity = destbufsz / ( 3 * 2 * sizeof(float) );
	sclx *= 0.2f;	// The 'M' glyph has size 4x5, so we compensate.
	scly *= 0.2f;
	const int l = (int) strlen(str);
	const int linespacing = 1.2f * 5.0f * scly;
	float x = posx;
	float y = posy;
	*numlines = 1;
	for ( int charnr=0; charnr<l; ++charnr )
	{
		int c = str[ charnr ];
		if ( c == '\n' )	// carriage return.
		{
			y -= linespacing;
			x = 0.0f;
			*numlines += 1;
		}
		if ( c < 40 || c >= 96 )
			continue;	// only render ascii 33 to 127
		c -= 40;
		const int width = widths[ c ];
		const int sz    = sizes [ c ];
		const int voffs = vdataoffsets[ c ];
		const int trias = sz/3;
		if ( trias_written + trias <= tri_capacity )
		{
			for ( int vnr=0; vnr<sz; ++vnr )
			{
				const float vx = vdata[ voffs+vnr ][ 0 ];
				const float vy = vdata[ voffs+vnr ][ 1 ];
				*buf_writer++ = x + vx * sclx;
				*buf_writer++ = y + ( vy - 2.5f ) * scly;
			}
			x += ( width + 1.0f ) * sclx;	// advance to next pos
			trias_written += trias;
		}
	}
	return trias_written;
}

