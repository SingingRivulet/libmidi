#include "mainMelody.hpp"
#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;
using namespace libmidi;
using namespace smf;

class channelParser:public mainMelody{
    protected:
        
        virtual bool readFile(message & m){
            while(1){
                if(this->eventId >= this->midifile[this->channelId].size())
                    return false;
                if (
                  midifile[this->channelId][this->eventId].isNoteOn() && 
                  midifile[this->channelId][this->eventId].size()>=3){
                    
                    m.begin=midifile[this->channelId][this->eventId].seconds;
                    m.delay=midifile[this->channelId][this->eventId].getDurationInSeconds();
                    m.note=midifile[this->channelId][this->eventId][1];
                    m.velocity=midifile[this->channelId][this->eventId][2];
                    ++(this->eventId);
                    return true;
                    
                }else{
                    ++(this->eventId);
                }
            }
        }
        
        void parseChannel(int id){
            if(id!=0)
                fprintf(out,"\n");
            fprintf(out,"channel:%d\n",id);
            this->lastn=-1;
            this->reset();
            this->eventId=0;
            this->channelId=id;
            this->noteBlockNum=0;
            this->parse();
        }
        
        virtual void onNoteOn(int note,int velocity){
            if(this->noteBlockNum >= 4){
                fprintf(out," ");
                this->noteBlockNum=0;
            }
            if(this->lastn==-1)
                fprintf(out,"%d_",0);
            else
                fprintf(out,"%d_",note - this->lastn);
            this->lastn=note;
            ++(this->noteBlockNum);
        }
        
        int lastn;
    protected:
        MidiFile midifile;
        int eventId;
        int channelId;
        FILE * out;
        int noteBlockNum;
    public:
        bool start(const char * inf,const char * outf){
            out=fopen(outf,"w");
            if(out==NULL)
                return false;
            this->midifile.read(inf);
            this->midifile.doTimeAnalysis();
            this->midifile.linkNotePairs();

            int tracks = midifile.getTrackCount();
            
            for (int track=0; track<tracks; track++) {
                this->parseChannel(track);
            }
            
            fclose(out);
            return true;
        }
};
int main(int argc, char** argv){
    if(argc!=3)
        return 1;
    channelParser P;
    P.start(argv[1],argv[2]);
    return 0;
}
