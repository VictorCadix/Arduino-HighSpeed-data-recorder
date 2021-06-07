#include "Modulo_SD.h"

Prog_moduloSD sd;

bool isTelem = false;

void setup(){

    Serial.begin(9600);

    sd.setup_();
    Serial.println(F("Setup DONE"));
}

void loop(){

    while (Serial.available()){
        char c = Serial.read();
        Serial.print(c);
        if (c == '#'){
            isTelem = true;
        }
        else if (c == ';'){
            sd.save2sd("\r\n", 2);
            isTelem = false;
        }
        else{
            if (isTelem){
                char str[1];
                str[0] = c;
                sd.save2sd(str,1);
            }
        }
    }
}