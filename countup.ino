// This #include statement was automatically added by the Particle IDE.
#include "InternetButton/InternetButton.h"

/* Count up to trigger – mainly animation */
/* will initially use delay - but consider a timing function */

InternetButton b = InternetButton();
int     count;
int     red[3] = {255, 0,0};
int     green[3] = {0, 255, 0};
int     blue[3] = {0, 0, 255};
int     white[3] = {255, 255,255};
int     chosenColour[3];
int     watching;
String  state;
//unsigned long time; // for tick timer - later

/*
namespace state {
    
}

typedef thing {
    
}

struct rgb {
    
}
*/

void chooseColour(int picked[3]) {
    chosenColour[0] = picked[0];
    chosenColour[1] = picked[1];
    chosenColour[2] = picked[2];
}

void reactToPress() {
    if (b.buttonOn(2)) {
        chooseColour(red);
        startCounting(2);
    }
    if (b.buttonOn(3)) {
        chooseColour(green);
        startCounting(3);
    }
    if (b.buttonOn(4)) {
        chooseColour(blue);
        startCounting(4);
    }
}

void startCounting(int pressed) {
    state = "counting";
    watching = pressed;
    count = 0;
}

int increaseOne(int theCount, int theButton) {
    ++theCount;
    b.ledOn(theCount, chosenColour[0], chosenColour[1], chosenColour[2]);
    if(theCount > 10) {
        startFlashing();
    }
    if(!b.buttonOn(theButton)) {
        startWaiting();
    }
    return(theCount);
}

void startFlashing() {
    state = "flashing";
}

void startWaiting() {
    state = "waiting";
    count = 0;
    watching = 0;
    b.allLedsOff();
    while (b.buttonOn(2) || b.buttonOn(3) || b.buttonOn(4)) {
        delay(100);
    }
    b.ledOn(3, 5, 0, 0);
    b.ledOn(6, 0, 5, 0);
    b.ledOn(9, 0, 0, 5);
}

void flash() {
    delay(200);
    b.allLedsOff();
    delay(200);
    b.allLedsOn(chosenColour[0], chosenColour[1], chosenColour[2]);
    delay(200);
    b.allLedsOff();
    delay(200);
    b.allLedsOn(chosenColour[0], chosenColour[1], chosenColour[2]);
    delay(200);
    b.allLedsOff();
}

void fireEvent(int pressed) {
    b.playNote("C2",1);
    Particle.publish("buttonPressed", String(pressed));
}

int flashDeviceWhite(String dummy) {
    chooseColour(white);
    flash();
    return(1);
}

///////////////////////////////////

void setup() {
    b.begin();
    startWaiting();
    Particle.function("respond", flashDeviceWhite);
}

void loop(){
    
    if (state == "waiting") {
        reactToPress();
    }

    if (state == "counting") {
        delay(100);
        count = increaseOne(count, watching);
    }
    
    if (state == "flashing") {
        flash();
        fireEvent(watching);
        startWaiting();
    }
}