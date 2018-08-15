#ifndef LIBMIDI_step
#define LIBMIDI_step
#include <set>
#include <list>
namespace libmidi{
    class step{
        public:
            struct noteStatus{
                bool noteOn;
                float beginAt;
                float delay;
                int velocity;
            };
            struct message{
                float begin;
                float delay;
                int note;
                int velocity;
            };
        private:
            void notesUpdate(){
                std::list<int> rm;
                for(auto i : this->sound){
                    if(this->notes[i].noteOn){
                        if((this->notes[i].beginAt + this->notes[i].delay)<this->now){
                            this->notes[i].beginAt=0.0f;
                            this->notes[i].delay=0.0f;
                            this->notes[i].velocity=0;
                            this->notes[i].noteOn=false;
                            rm.push_back(i);
                        }
                    }
                }
                for(auto it:rm){
                    this->sound.erase(it);
                }
            }
            void initForNextStep(){
                //重设时间
                if(this->msgbuffer.begin > (this->now + this->timeStep))
                    this->now=this->msgbuffer.begin;
                else
                    this->now+=this->timeStep;
                this->notesUpdate();
            }
            void onMessage(message & m){
                if(m.note<0 || m.note>127)
                    return;
                
                this->notes[m.note].noteOn=true;
                this->notes[m.note].beginAt=m.begin;
                this->notes[m.note].delay=m.delay;
                this->notes[m.note].velocity=m.velocity;
                
                this->sound.insert(m.note);
            }
        private:
            void init(){
                for(int i=0;i<128;i++){
                    this->notes[i].beginAt=0.0f;
                    this->notes[i].delay=0.0f;
                    this->notes[i].velocity=0;
                    this->notes[i].noteOn=false;
                }
                this->now=0.0f;
                this->sound.clear();
            }
        protected:
            noteStatus notes[128];
            float now;
            message msgbuffer;
            std::set<int> sound;
        public:
            float timeStep;
            step(){
                this->timeStep=0.1f;
                //0.1秒是普通人听力辨别的最小单位
            }
            virtual bool readFile(message & m)=0;
            virtual void onStep(const noteStatus *,float ,const std::set<int> &)=0;
            bool beginAtThisStep(int n){
                if(n<0 || n>=128)
                    return false;
                if(this->notes[n].beginAt < this->now)
                    return false;
                else
                    return true;
            }
            virtual void parse(){
                this->init();
                while(1){
                    if(!this->readFile(this->msgbuffer))
                        break;
                    else
                    if(this->msgbuffer.begin > this->now + this->timeStep){
                        //不在当前扫描区域
                        this->onStep(this->notes , this->now , this->sound);
                        this->initForNextStep();
                        this->onMessage(this->msgbuffer);
                    }else
                        this->onMessage(this->msgbuffer);
                }
                this->onStep(this->notes , this->now , this->sound);
            }
    };
}
#endif
