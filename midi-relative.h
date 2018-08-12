#pragma once
#include "midi-parser.h"
void midi_relative_parse(struct midi_parser * paraer,void(*callback)(int note,int channel,void * arg),void * arg);
