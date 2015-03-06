all:
	tools/fnt.py models/font.obj > out.c
	cc -c out.c

