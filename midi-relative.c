#include "midi-relative.h"
void midi_relative_parse(
  struct midi_parser * parser ,
  void(*callback)(int note,int channel,void * ary) ,
  void * arg){
    enum midi_status status;
    int lastnote[16];
    int i;
    int channel;
    for(i=0;i<16;i++){
        lastnote[i]=-1;
    }
    while (1) {
        status = midi_parse(parser);
        if(
          status==MIDI_PARSER_TRACK_MIDI &&
          parser->midi.status==MIDI_STATUS_NOTE_ON){
            channel=parser->midi.channel;
            if(channel<0 || channel>15)
                continue;
            if(lastnote[channel]==-1){
                lastnote[channel]=parser->midi.param1;
                callback(0,channel,arg);
            }else{
                if((lastnote[channel]-12) > parser->midi.param1)
                    continue;
                callback(parser->midi.param1-lastnote[channel],channel,arg);
                lastnote[channel]=parser->midi.param1;
            }
        }else
        if(status==MIDI_PARSER_EOB || status==MIDI_PARSER_ERROR){
            return;
        }
    }
}
