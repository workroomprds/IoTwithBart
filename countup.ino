// This #include statement was automatically added by the Particle IDE.
#include "InternetButton/InternetButton.h"

/* Count up to trigger – mainly animation */
/* will initially use delay - but consider a timing function */

InternetButton b = InternetButton();
int     count;
int     red[3] = {255, 0,0};
int     green[3] = {0, 255, 0};
int     blue[3] = {0, 0, 255};
int     white[3] = {255, 255, 255};
int     chosenColour[3];
int     watching;
String  state;
String  buttonColours[4] = { "null", "red", "green", "blue" };
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

void chooseColourFromString(String colour) {
    chooseColour(white);
    if (colour == "red" ) {
        chooseColour(red);
    }
    if (colour == "green" ) {
        chooseColour(green);
    }
    if (colour == "blue" ) {
        chooseColour(blue);
    }
}

int reactToPress() {
    if (b.buttonOn(2)) {
        return(2);
    }
    if (b.buttonOn(3)) {
        return(3);
    }
    if (b.buttonOn(4)) {
         return(4);
    }
    return(0);
}

void startCounting(int pressed) {
    chooseColourFromString(buttonColours[pressed - 1]);
    state = "counting";
    watching = pressed;
    count = 0;
}

int increaseOne(int theCount, int theButton) {
    ++theCount;
    b.ledOn(theCount, chosenColour[0], chosenColour[1], chosenColour[2]);
    return(theCount);
}

bool buttonIsUp(int theButton) {
    return( !b.buttonOn(theButton) );
}

void startFlashing() {
    state = "flashing";
}

void startWaiting() {
    state = "waiting";
    count = 0;
    watching = 0;
    b.allLedsOff();
    while (reactToPress() != 0) {
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
    b.playNote("C1",1);
    Particle.publish("buttonPressed", String(pressed));
}

int pulseDevice(String colour) {
    chooseColourFromString(colour);
    fadeUp(20,200, 10);
    fadeDown(20,200, 30);
    fadeUp(40,255,50);
    fadeDown(40,255, 100);
    startWaiting();
    return(1);
}

void fadeUp(int steps, int dim, int wait) {
    for (int i=0; i<= steps; i++) {
        lightRing( (int) i*chosenColour[0]/dim, (int) i*chosenColour[1]/dim, (int) i*chosenColour[2]/dim );
        delay(wait);
    }
}

void fadeDown(int steps, int dim, int wait) {
    int i;
    for (int j=0; j<= steps - 1; j++) {
        i = steps - j;
        lightRing( (int) i*chosenColour[0]/dim, (int) i*chosenColour[1]/dim, (int) i*chosenColour[2]/dim );
        delay(wait);
    }
}

void lightRing(int redVal, int greenVal, int blueVal) {
    for (int led = 1; led <=11; led++){
        b.ledOn(led, redVal, greenVal, blueVal );
    }
}

int flashDevice(String colour) {
    chooseColourFromString(colour);
    flash();
    startWaiting();
    return(1);
}


///////////////////////////////////

void setup() {
    b.begin();
    startWaiting();
    Particle.function("respond", pulseDevice);
}

void loop(){
    if (state == "waiting") {
        int pressed = reactToPress();
        if (pressed > 0) startCounting(pressed);
    }

    if (state == "counting") {
        delay(100);
        count = increaseOne(count, watching);
        if ( buttonIsUp(watching) ) startWaiting();
        if ( count > 10 ) startFlashing();
    }
    
    if (state == "flashing") {
        flash();
        fireEvent(watching);
        startWaiting();
    }
}


