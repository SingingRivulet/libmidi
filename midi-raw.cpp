#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;
using namespace smf;

int main(int argc, char** argv){
    if(argc!=3)
        return 1;
    const char * inf =argv[1]
    const char * outf=argv[2];
    
    MidiFile midifile;
    auto out=fopen(outf,"w");
    if(out==NULL)
        return 1;
    midifile.read(inf);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    
    int tracks = midifile.getTrackCount();
            
    for (int id=0; id<tracks; id++) {
        
        if(id!=0)
            fprintf(out,"\n");
        fprintf(out,"channel:%d\n",id);
        int notenum=midifile[id].size();
        for(int nn=0;nn<notenum;nn++){
            int lastTick=-1;
            if (midifile[id][nn].isNoteOn() && midifile[id][nn].size()>=3){
                int begin =midifile[id][nn].tick;
                int note  =midifile[id][nn][1];
                if(lastTick!=begin && lastTick!=-1)
                    fprintf(out,"\n");
                lastTick=begin;
                fprintf(out,"%d ",note);
            }
        }
    }
            
    fclose(out);
    return 0;
}
