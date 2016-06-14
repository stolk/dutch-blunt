#!/usr/bin/python

import sys


def process_blob( blob, indexoffset ) :
	lines = blob.split( '\n' )
	lines = [ l.strip() for l in lines ]
	name = lines[ 0 ]
	nr = int( name[1:], 16 )
	row = nr / 8
	col = nr % 8
	yoff = row*6
	xoff = col*6
	vlines = [ l for l in lines if "v " in l ]
	verts = []
	faces = []
	trias = []
	width=0
	for vline in vlines:
		fields = vline.split( " " )
		x = -float( fields[2] ) - xoff
		y =  float( fields[3] ) - yoff
		v = ( x, y )
		verts.append( v )
		width = max( width, x )
	flines = [ l for l in lines if "f " in l ]
	for fline in flines :
		fields = fline.split( " " )[ 1: ]
		indices = [ int( x.split("/")[0] ) - indexoffset for x in fields ]
		faces.append( indices )
	for face in faces :
		numt = len( face ) - 2
		for t in range( numt ) :
			i0 = face[   0 ]
			i1 = face[ t+1 ]
			i2 = face[ t+2 ]
			trias.append( (i0,i1,i2) )
	#print name, len(verts)
	vstream = []
	for t in trias :
		vstream.append( verts[ t[0] ] )
		vstream.append( verts[ t[1] ] )
		vstream.append( verts[ t[2] ] )
	return ( nr, width, vstream, indexoffset+len(verts) )


# main entry point:

if len( sys.argv ) != 2 :
	print "Usage: %s foo.obj" % ( sys.argv[0], ) 
	sys.exit( 1 )

f = open( sys.argv[1], 'r' )
assert f
blob = f.read()
f.close()

blobs = blob.split( "o " )[ 1: ]
assert len(blobs) == 128-1

widths  = [ 0 for x in range(128) ]
sizes   = [ 0 for x in range(128) ]
streams = [ None for x in range(128 ) ]

indexoffset = 1

for blob in blobs :
	nr, width, vstream, indexoffset = process_blob( blob, indexoffset )
	widths [ nr ] = width
	sizes  [ nr ] = len(vstream)
	streams[ nr ] = vstream

assert not -1 in widths
assert not -1 in sizes

totalsize = sum( sizes )

print "// Machine-generated from %s by tools/fnt.py, do not edit." % ( sys.argv[1], )
print '#pragma clang diagnostic ignored "-Wmissing-braces"'
print '#pragma clang diagnostic ignored "-Wconversion"'
print ""
print "#define VDATASZ %d" % ( totalsize, )
print "#define NUMGLYPHS 128"

print "// glyph widths. 1 for '!' and 4 for 'M'."
print "static float widths[NUMGLYPHS] =\n{",
for i,width in enumerate( widths ) :
	if i%8 == 0 :
		print "\n  ",
	print str(width)+",",
print "\n};"

print "// glyph sizes in number of triangle vertices (multiple of 3.)"
print "static int sizes[NUMGLYPHS] =\n{",
for sz in sizes :
	if i%8 == 0 :
		print "\n  ",
	print str(sz)+",",
print "\n};"

print "// vdata offsets for each glyph."
print "static int vdataoffsets[NUMGLYPHS] =\n{",
vdataoff = 0
for sz in sizes :
	if i%8 == 0 :
		print "\n  ",
	print str(vdataoff)+",",
	vdataoff += sz
print "\n};"

print "// vertex data for all glyphs combined."
print "static float vdata[VDATASZ][2] =\n{"
for stream in streams :
	if stream:
		for v in stream:
			print "%g,%g," % ( v[0],v[1] ),
	print ""
print "};"

