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
 * If fadestart >= 0, then opacity values are written as well: (x,y,o,x,y,o,x,y,o).
 */
int dblunt_string_to_vertices
(
	const char* str,		//!< characters to render.
	float* destbuf,			//!< output buffer to hold vertices.
	int destbufsz, 			//!< size of output buffer in bytes.
	float posx, float posy,		//!< target position for text render.
	float sclx, float scly,		//!< scale of the text render.
	float fadestart,		//!< if >=0, start fading out at this character number and write opactities.
	float* __restrict__ textw,	//!< out: size of longest line.
	float* __restrict__ texth	//!< out: height of text.
)
{
	int trias_written = 0;
	float* buf_writer = destbuf;
	const int floats_per_vertex = fadestart >= 0 ? 3 : 2;
	const int tri_capacity = destbufsz / ( 3 * floats_per_vertex * sizeof(float) );
	sclx *= 0.2f;	// The 'M' glyph has size 4x5, so we compensate.
	scly *= 0.2f;
	const int l = (int) strlen(str);
	const float linepitch = 1.25f * 5.0f * scly;
	const float linespacing = 0.25f * 5.0f * scly;
	float x = posx;
	float y = posy - 5.0f*scly;
	float maxx = x;
	int numlines = 1;
	int numprinted = 0;
	for ( int charnr=0; charnr<l; ++charnr )
	{
		int c = str[ charnr ];
		if ( c == '\n' )	// carriage return.
		{
			y -= linepitch;
			x = posx;
			numlines += 1;
			continue;
		}
		if  ( c == ' ' )
		{
			x += 3.0f * sclx;
			continue;
		}
		if ( c > 127 )
			continue;	// only render ascii 0 to 127
		const float width = widths[ c ];
		const int   sz    = sizes [ c ];
		const int   voffs = vdataoffsets[ c ];
		const int   trias = sz/3;
		if ( x + width*sclx > maxx ) maxx = x + width*sclx;
		if ( trias_written + trias <= tri_capacity )
		{
			for ( int vnr=0; vnr<sz; ++vnr )
			{
				const float vx = vdata[ voffs+vnr ][ 0 ];
				const float vy = vdata[ voffs+vnr ][ 1 ];
				*buf_writer++ = x + vx * sclx;
				*buf_writer++ = y + vy * scly;
				int cut = numprinted+4;
				if ( fadestart >= 0 )
				{
					if ( cut < fadestart )
						*buf_writer++ = 1.0f;
					else
					{
						float o = 1.0f - 0.25f * ( cut - fadestart );
						*buf_writer++ = o < 0 ? 0 : o;
					}
				}
			}
			x += ( width + 1.0f ) * sclx;	// advance to next pos
			trias_written += trias;
		}
		numprinted++;
	}
	*texth = numlines * linepitch - 1 * linespacing;
	*textw = maxx - posx;
	return trias_written;
}


/*!
 * Convert an ascii string to a stream of vertex data.
 * Returns the number of triangles(x,y,x,y,x,y) written to the buffer.
 * If fadestart >= 0, then opacity values are written as well: (x,y,o,x,y,o,x,y,o).
 */
int dblunt_attributed_string_to_vertices
(
	const char* str,		//!< characters to render.
	const char* att,		//!< character attributes.
	float* destbuf,			//!< output buffer to hold vertices.
	int destbufsz, 			//!< size of output buffer in bytes.
	float posx, float posy,		//!< target position for text render.
	float sclx, float scly,		//!< scale of the text render.
	float* __restrict__ textw,	//!< out: size of longest line.
	float* __restrict__ texth	//!< out: height of text.
)
{
	int trias_written = 0;
	float* buf_writer = destbuf;
	const int floats_per_vertex = 2;
	const int tri_capacity = destbufsz / ( 3 * floats_per_vertex * sizeof(float) );
	sclx *= 0.2f;	// The 'M' glyph has size 4x5, so we compensate.
	scly *= 0.2f;
	const int l  = (int) strlen(str);
	const int la = att ? (int) strlen(att) : 0;
	assert( la==0 || la==l );
	const float linepitch = 1.25f * 5.0f * scly;
	const float linespacing = 0.25f * 5.0f * scly;
	float x = posx;
	float y = posy - 5.0f*scly;
	float maxx = x;
	int numlines = 1;
	int numprinted = 0;
	for ( int charnr=0; charnr<l; ++charnr )
	{
		int c = str[ charnr ];
		int a = att ? att[ charnr ] : 0;
		const float csclx = ( a=='^' || a=='_' ) ? 0.6f * sclx : sclx;
		const float cscly = ( a=='^' || a=='_' ) ? 0.6f * scly : scly;
		const float coffy = ( a=='^' ) ? 0.5f * scly : ( a=='_' ) ? -0.5f * scly : 0.0f;
		if ( c == '\n' )	// carriage return.
		{
			y -= linepitch;
			x = posx;
			numlines += 1;
			continue;
		}
		if  ( c == ' ' )
		{
			x += 3.0f * sclx;
			continue;
		}
		if ( c > 127 )
			continue;	// only render ascii 0 to 127
		const float width = widths[ c ];
		const int   sz    = sizes [ c ];
		const int   voffs = vdataoffsets[ c ];
		const int   trias = sz/3;
		if ( x + width*csclx > maxx ) maxx = x + width*csclx;
		if ( trias_written + trias <= tri_capacity )
		{
			for ( int vnr=0; vnr<sz; ++vnr )
			{
				const float vx = vdata[ voffs+vnr ][ 0 ];
				const float vy = vdata[ voffs+vnr ][ 1 ];
				*buf_writer++ = x + vx * csclx;
				*buf_writer++ = y + vy * cscly + coffy;
			}
			x += ( width + 1.0f ) * csclx;	// advance to next pos
			trias_written += trias;
		}
		numprinted++;
	}
	(void)numprinted;
	*texth = numlines * linepitch - 1 * linespacing;
	*textw = maxx - posx;
	return trias_written;
}

