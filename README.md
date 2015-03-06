# dutch-blunt
Low poly font (plus rendering system.)
Uses triangles not textures, for always sharp edges.

## The Font
Dutch-Blunt is both the name of the minimalistic font that I created, and its rendering API.
The font was created with the following criteria:

* Rendered as pure triangles as opposed to curves, textures, or lines.
* Low polygon (triangle) count for each glyph.
* ASCII only (characters 33 to 127 inclusive.)

## The API
The font was created in Wings3D and exported to Wavefront OBJ.
Dutch-Blunt comes with a python script that will read the wavefront OBJ file and create C-source code.
The glyph data containst glyph widths, the triangle vertices, the vertex count, and an offset into a global vertex data set.

