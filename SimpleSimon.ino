/*
https://projecthub.arduino.cc/Arduino_Scuola/a-simple-simon-says-game-6f7fef
This sketch is a simple version of the famous Simon Says game. You can  use it and improved it adding
levels and everything you want to increase the  diffuculty!

There are five buttons connected to A0, A1, A2, A3 and A4.
The  buttons from A0 to A3 are used to insert the right sequence while A4 to start the  game.

When a wrong sequence is inserted all the leds will blink for three  time very fast otherwhise the
inserted sequence is correct.

This lesson aims to show how to make a simple Arduino based Simon Says light game. 

Hardware needed:
5x  pushbuttons
1x Blue led
1x Yellow led
1x Red led
1x Green Led
4x  1k resistors
4x 10k resisors
10x jumpers
1x Speaker or buzzer
*/
/*
HHS Modifications
Use Bounce library for switch debounce
Add tones for auditory feedback
Code cleanup
*/

#include <Bounce2.h>


#define BTN0    A0
#define BTN1    A1
#define BTN2    A2
#define BTN3    A3
#define BTN4    A4
#define LEDG    2
#define LEDR    3
#define LEDY    4
#define LEDB    5
#define LEDW    6
#define SPKR    7

#define DELAY1  100
#define DELAY2  200
#define DELAY3  300
#define DELAY4  400
#define DELAY10 1000

Bounce2::Button button0 = Bounce2::Button();    // Blue
Bounce2::Button button1 = Bounce2::Button();    // Yellow
Bounce2::Button button2 = Bounce2::Button();    // Red
Bounce2::Button button3 = Bounce2::Button();    // Green
Bounce2::Button button4 = Bounce2::Button();    // White

const int MAX_LEVEL  = 50;
const int startTone  = 1760, tone1 = 262, tone2 = 330, tone3 = 392, tone4 = 523;
//const int buzzer = 7;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level  = 1;
int difficulty_factor = 50;

int velocity = 1000;

void setup() {
    Serial.begin(115200);
    
    pinMode(BTN0, INPUT);
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(BTN3, INPUT);
    pinMode(BTN4, INPUT);

    pinMode(LEDG, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDY, OUTPUT);
    pinMode(LEDB, OUTPUT);
    pinMode(LEDW, OUTPUT);
    pinMode(SPKR, OUTPUT);

// attach debounce to pins, set the settling period and ON state
    button0.attach(BTN0);
    button0.interval(5);
    button0.setPressedState(LOW);
    button1.attach(BTN1);
    button1.interval(5);
    button1.setPressedState(LOW);
    button2.attach(BTN2);
    button2.interval(5);
    button2.setPressedState(LOW);
    button3.attach(BTN3);
    button3.interval(5);
    button3.setPressedState(LOW);
    button4.attach(BTN4);
    button4.interval(5);
    button4.setPressedState(LOW);
 
    reset();
}

void loop() {
    button4.update();
    if  (level == 1) {
        generate_sequence();//generate a sequence;
        digitalWrite(LEDW, HIGH);
    }

//If start button is pressed or you're winning
    if (button4.pressed() || level > 1) {
        if (button0.pressed())
            difficulty_factor = 100;
        start_tone();
        digitalWrite(LEDW, LOW);
        show_sequence();    //show the sequence
        get_sequence();     //wait for your sequence
    }
}

void reset() {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDW, HIGH);
    level  = 1;
    velocity = 1000;
}

void  show_sequence() {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDB, LOW);
    
    for (int i = 0; i < level; i++) {
        digitalWrite(sequence[i],  HIGH);
        delay(velocity);
        digitalWrite(sequence[i], LOW);
        delay(DELAY1);
    }
}

void  get_sequence() {
    bool is_correct = false; //this is_correct indicates if the sequence is correct

    for  (int i = 0; i < level; i++) {
        is_correct = false;
        while(is_correct == 0) {
            button0.update();
            button1.update();
            button2.update();
            button3.update();
            if (button0.pressed()) {
                digitalWrite(LEDB, HIGH);
                your_sequence[i] = 5;
                is_correct = true;
                delay(DELAY1);
                if  (your_sequence[i] != sequence[i]) {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LEDB,  LOW);
            }

            if (button1.pressed()) {
                digitalWrite(LEDY, HIGH);
                your_sequence[i] = 4;
                is_correct = true;
                delay(DELAY1);
                if (your_sequence[i] != sequence[i]) {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LEDY,  LOW);
            }

            if (button2.pressed()) {
                digitalWrite(LEDR, HIGH);
                your_sequence[i] = 3;
                is_correct = true;
                delay(DELAY1);
                if (your_sequence[i] != sequence[i]) {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LEDR,  LOW);
            }

            if (button3.pressed()) {
                digitalWrite(LEDG, HIGH);
                your_sequence[i] = 2;
                is_correct = true;
                delay(DELAY1);
                if (your_sequence[i] != sequence[i]) {
                    wrong_sequence();
                    return;
                }
                digitalWrite(LEDG,  LOW);
            }
        }
    }
    right_sequence();
}

void generate_sequence() {
    randomSeed(millis());  //in this way is really random!!!
    for (int i = 0; i < MAX_LEVEL; i++) {
        sequence[i]  = random(2,6);
    }
}

void wrong_sequence() {
    incorrect_tone();
    for (int i = 0; i < 5;  i++) {
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDY,  HIGH);
        digitalWrite(LEDB, HIGH);
        delay(DELAY2);
        digitalWrite(LEDG, LOW);
        digitalWrite(LEDR,  LOW);
        digitalWrite(LEDY, LOW);
        digitalWrite(LEDB, LOW);
        delay(DELAY2);
    }
    delay(DELAY10);
    reset();
}

void right_sequence() {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDB, LOW);
    delay(DELAY2);

    digitalWrite(LEDW, HIGH);
    delay(DELAY2);
    digitalWrite(LEDW, LOW);
    delay(DELAY2);
    digitalWrite(LEDW, HIGH);
    delay(DELAY2);
    digitalWrite(LEDW, LOW);
    delay(DELAY1);

    if (level < MAX_LEVEL)
        level++;

    if (velocity > difficulty_factor)
        velocity -= difficulty_factor; //increase difficulty
}

void start_tone() {
    tone(SPKR, startTone);
    delay(100);
    noTone(SPKR);
}

void incorrect_tone() {
    tone(SPKR, tone1);
    delay(DELAY2);
    tone(SPKR, tone2);
    delay(DELAY3);
    noTone(SPKR);
}
