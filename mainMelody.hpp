#ifndef LIBMIDI_mainMelody
#define LIBMIDI_mainMelody
#include "step.hpp"
namespace libmidi{
    class mainMelody:public step{
        private:
            int lastNote;
        public:
            virtual void onStep(const noteStatus * notes,float tm,const std::set<int> & sound){
                int i;
                for(auto it=sound.rbegin();it!=sound.rend();it++){
                    i=*it;
                    if(!notes[i].noteOn)
                        continue;
                    
                    if(i==this->lastNote){
                        int min=i-12;
                        if(min<0)min=0;
                        for(;it!=sound.rend();it++){
                            i=*it;
                            if(i<min)
                                break;
                            if(!notes[i].noteOn)
                                continue;
                            if(this->beginAtThisStep(i)){
                                this->onNoteOn(i , notes[i].velocity);
                                this->lastNote=i;
                                break;
                            }
                        }
                        return;
                    }
                    
                    if(i!=this->lastNote && (this->lastNote-i)<12){
                        if(this->beginAtThisStep(i)){
                            this->onNoteOn(i , notes[i].velocity);
                            this->lastNote=i;
                        }
                    }
                    return;
                }
            }
            
            virtual void onNoteOn(int note,int velocity)=0;
            virtual void reset(){
                this->lastNote=0;
            }
    };
}
#endif
