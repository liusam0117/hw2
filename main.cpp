  
#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn mypin1(D8);
DigitalIn mypin2(D9);
DigitalIn mypin3(D10);
AnalogOut aout(PA_4); // D7
AnalogIn Ain(A0);

uLCD_4DGL uLCD(D1, D0, D2);


int freq = 0;
Thread t;
void gen_waveform();

int main()
{
    int counter = 0;
    int S;
    //mypin1.mode(PullDown);
    //mypin2.mode(PullDown);
    //mypin3.mode(PullDown);
    uLCD.text_width(2); //4X size text
    uLCD.background_color(0xFFFFFF);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.text_height(2);
    uLCD.color(RED);
    
    while(!mypin3){
        uLCD.locate(1,2);
        //uLCD.printf("%2d",counter);
        if(mypin1 && counter < 10){           
            counter++;
            freq = (counter+1)*10; 
            uLCD.printf("%3dHz",freq);
        }else if(mypin2 && counter > 0){            
            counter--;
            freq = (counter+1)*10; 
            uLCD.printf("%3dHz",freq);
        }else {
            freq = (counter+1)*10;
            uLCD.printf("%3dHz",freq);
        }
        ThisThread::sleep_for(300ms);
    }
    uLCD.locate(1,2);
    uLCD.textbackground_color(BLACK);
    freq = (counter+1)*10;
    uLCD.printf("%3dHz",freq);
    
    S = (1+0+8+0+6+0+0+0+4)%10; //9
    //printf("\n");
    t.start(gen_waveform);

    float data[500] = {0};
    ThisThread::sleep_for(500ms);
    for (int i = 0; i < 500; i++) {
        data[i] = Ain;
        ThisThread::sleep_for(2ms);
    }
    for (int i = 0; i < 500; i++) {
        printf("%f\n", data[i]);
        //ThisThread::sleep_for(100ms);
    }
}

void gen_waveform(void){
    // S = 1.
    uint16_t triwave = 0;
    while(1){
        for (int i = 0; i < 3; i++) {
            triwave = (uint16_t)(65535 * 3/3.3 * i/3);
            aout.write_u16(triwave);
            ThisThread::sleep_for(1000ms/30/freq);
        }
        for (int i = 0; i < 27; i++) {
            triwave = (uint16_t)(65535 * 3/3.3 - (65535 * 3/3.3 * i/27));
            aout.write_u16(triwave);
            ThisThread::sleep_for(1000ms/30/freq);
        }
    }
}