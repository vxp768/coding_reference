#include <iostream>

using namespace std;
enum note { middleC, Csharp, Eflat };

class Instrument {
public:
 void play(note) {
    cout <<"Instrument::play"<<endl;
 }
};
class Wind: public Instrument {
public:
 void play(note) {
    cout<<"Wind:: play"<<endl;
 }
};
void tune(Instrument &i ) {
    i.play(middleC);
}
int main() {
    Wind flute;
    //Interface "play" of "class Instrument" will be called as "tune"
    // function is taking Instrument reference as argument
    // so flute will be upcast
    tune(flute); //Upcasting
}
