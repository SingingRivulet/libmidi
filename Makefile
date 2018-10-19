all:midi-index
	
midi-index:midifile/lib/libmidifile.a
	g++ midi-index.cpp -I ./midifile/include/ midifile/lib/libmidifile.a -std=c++11 -o midi-index
midi-raw:midifile/lib/libmidifile.a
	g++ midi-raw.cpp -I ./midifile/include/ midifile/lib/libmidifile.a -std=c++11 -o midi-raw
midifile/lib/libmidifile.a:
	cd midifile && make
