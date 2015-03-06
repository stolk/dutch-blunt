CC = clang-3.5

dblunt.o: src/dblunt.c src/vdata.h
	$(CC) -c src/dblunt.c

src/vdata.h:
	tools/fnt.py models/font.obj > src/vdata.h


