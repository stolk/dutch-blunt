extern "C" {
#include "dblunt.h"
}

namespace raven
{
namespace ogl
{

/** Convert Text to Triangles

A C++ wrapper for dblunt_string_to_vertices

Usage:
<pre>
        raven::ogl::cString2Vx S2V;     // text converter
        S2V.Vertex( theVx );            // vertex output
        S2V.Color( theClr );            // color output
        S2V.Color(1, 1, 1);             // input color for text
        S2V.Locate(x, y z);             // input location for text
        S2V.Scale( xs, ys );            // input size of text

        S2V.String( "this is a test" ); // convert
</pre>
 */

class cString2Vx
{
public:

    /** Location for displayed text */
    void Locate( float x, float y, float z )
    {
        myX = x;
        myY = y;
        myZ = z;
    }

    /** Scale of displayed text */
    void Scale( float x, float y )
    {
        myXscale = x;
        myYscale = y;
    }
    /** Triangles verices for displayed text will be appended to this vector */
    void Vertex( std::vector<float>& v )
    {
        myVx = &v;
    }
    /** Color for displayed text vertices will be appended to this vector */
    void Color(  std::vector<float>& c )
    {
        myClr = &c;
    }

    /** Color of displayed text */
    void Color( float r, float g, float b )
    {
        myColor.clear();
        myColor.push_back(r);
        myColor.push_back(g);
        myColor.push_back(b);
    }
    /** Text to display */
    void String( const std::string& str )
    {
        float textw, texth;
        float buf[2000];
        int trcount = dblunt_string_to_vertices(
                          str.c_str(),
                          buf,
                          2000 * sizeof(float),
                          myX, myY,
                          myXscale, myYscale,
                          &textw, &texth );

        float * pt = buf;
        for( int textv = 0; textv < trcount; textv++ )
        {
            myVx->push_back( *pt++ );
            myVx->push_back( *pt++ );
            myVx->push_back( myZ );
            myVx->push_back( *pt++ );
            myVx->push_back( *pt++ );
            myVx->push_back( myZ );
            myVx->push_back( *pt++ );
            myVx->push_back( *pt++ );
            myVx->push_back( myZ );

        }

        for( int k = 0; k<3*trcount; k++ )
        {
            myClr->insert( myClr->end(),
                          myColor.begin(), myColor.end());
        }
    }

private:
    float myX, myY, myZ;
    float myXscale, myYscale;
    std::vector<float> myColor;

    std::vector<float> * myVx;
    std::vector<float> * myClr;
};


}
}
