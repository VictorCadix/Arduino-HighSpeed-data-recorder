#ifndef Modulo_SD_H
#define Modulo_SD_H

#include <SdFat.h>

#define chipSelect 10
#define fileName "Telem.txt"
#define fileParamsName "Params.txt"
#define BUF_SIZE 256
#define SEPARATOR ','

uint32_t ahora_sd;
uint32_t antes_sd = 0;
uint32_t dTiempo_sd;

class Prog_moduloSD{
    SdFat sd;
    SdFile dataFile;
    SdFile paramsFile;
    char sd_buff[BUF_SIZE];
    int pos = 0;        //posicion libre del buffer

public:
    void setup_();
    void save2sd(char* str, int str_len);
    void save2sd(int16_t int_val);
    void save2sd(uint16_t int_val);
    void save2sd(int32_t int_val);
    void save2sd(uint32_t int_val);
    void save2sd(float float_val);
};

void addSeparator(char* str, int *size){
    str[*size] = SEPARATOR;
    str[*size + 1] = '\0';
    (*size)++;
}

int indexOf(char c, char* buff){
	for (int i=0; buff[i]!='\0'; i++){
		if (c == buff[i]){
			return i;
		}
	}
	return -1;
}

void Prog_moduloSD::setup_(){
    Serial.print(F("Initializing SD card..."));
    // see if the card is present and can be initialized:
    if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
        Serial.println(F("Card failed, or not present"));
        // don't do anything more:
        digitalWrite(LED_BUILTIN, HIGH);
        while (1);
    }
    
    Serial.println(F("card initialized."));
}

void Prog_moduloSD::save2sd (char* str, int str_len){

    if (pos + str_len < BUF_SIZE){
        for (int i = 0; i < str_len; i++){
            sd_buff[pos] = str[i];
            pos++;
        }
    }
    else{
        int remaining = BUF_SIZE - pos;
        for (int i = 0; i <= remaining; i++){
            sd_buff[pos] = str[i];
            pos++;
        }
        //Send to Sd
        dataFile.open(fileName, O_CREAT | O_APPEND | O_WRITE);
        dataFile.write(sd_buff,BUF_SIZE);
        dataFile.close();

        pos = 0;
        for (int i = remaining; i < str_len; i++){
            sd_buff[pos] = str[i];
            pos++;
        }
    }
}

void Prog_moduloSD::save2sd (int16_t int_val){
    char str[15]; // max 10 + 1
    int nDigits = sprintf(str, "%d", int_val);
    addSeparator(str, &nDigits);
    save2sd(str, nDigits);
}

void Prog_moduloSD::save2sd (uint16_t int_val){
    char str[15]; // max 10 + 1
    int nDigits = sprintf(str, "%u", int_val);
    addSeparator(str, &nDigits);
    save2sd(str, nDigits);
}

void Prog_moduloSD::save2sd (int32_t int_val){
    char str[15]; // max 10 + 1
    int nDigits = sprintf(str, "%d", int_val);
    addSeparator(str, &nDigits);
    save2sd(str, nDigits);
}

void Prog_moduloSD::save2sd (uint32_t int_val){
    char str[15]; // max 10 + 1
    int nDigits = sprintf(str, "%lu", int_val);
    addSeparator(str, &nDigits);
    save2sd(str, nDigits);
}

void Prog_moduloSD::save2sd (float float_val){
    char str[15]; // max 10 + 1
    uint8_t nDecimales = 4;
    
    dtostrf(float_val, 5, nDecimales, str);
    
    int nDigits = 0;
    int intPart = float_val;
    while(intPart != 0){
        nDigits++;
        intPart /= 10;
    }
    nDigits += nDecimales + 1;      // +1 por la coma
    addSeparator(str, &nDigits);
    save2sd(str, nDigits);
}

#endif