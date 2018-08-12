all:midi-dump midi-relative-dump
	
midi-relative-dump : midi-relative-dump.c midi-relative.o midi-parser.o
	cc -std=c99 midi-relative-dump.c midi-relative.o midi-parser.o -I./ -o midi-relative-dump

midi-relative.o : midi-relative.c
	cc -std=c99 midi-relative.c -c -I./

midi-dump : midi-parser.o midi-dump.c
	cc -std=c99 midi-parser.o midi-dump.c -o midi-dump -I./

midi-parser.o : midi-parser.c
	cc -std=c99 midi-parser.c -c -I./
