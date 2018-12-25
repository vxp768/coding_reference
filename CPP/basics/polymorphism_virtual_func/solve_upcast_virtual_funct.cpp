#include <iostream>

using namespace std;
enum note { middleC, Csharp, Eflat };

class Instrument {
public:
virtual void play(note) {
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
    // tune function is taking Instrument reference as argument
    // but because of virtual function play 
    // dynamic binding makes sure that play of "Wind" is called
    tune(flute); //Upcasting
}
