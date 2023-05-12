#include "IM315PS4.hpp"

IM315PS4::IM315PS4(PinName TX, PinName RX){
    i = 0;
    checkSum = 0;
    errorCheck[0] = 0, errorCheck[1] = 0, errorCheck[2] = 0;
    data = new uint8_t[dataSize];
    buff = new uint8_t[charSize];
    device = new RawSerial(TX, RX);
    device -> baud(19200);
    for(int i = 0 ; i < 4 ; i++)data[i] = 0;
    for(int i = 4 ; i < 8 ; i++)data[i] = 0x7f;
    for(int i = 8 ; i < 10 ; i++)data[i] = 0;
    device -> attach(callback(this,&IM315PS4::getData),RawSerial::RxIrq);
}

void IM315PS4::getData(){
    if(device -> readable()){
        buff[i] = device -> getc();
        if(buff[i] == header) {
            if(i == 0) i++;
            else {
                buff[0] = header;
                i = 1;
            }
        } else {
            if(i == 0);
            else if(buff[i] == ',');
            else i++;
        }
        if(i >= charSize) {
            if(buff[0] == header) {
                data[0] = ASCII(buff[1], buff[2]);
                data[1] = ASCII(buff[3], buff[4]);
                data[2] = ASCII(buff[5], buff[6]);
                data[3] = ASCII(buff[7], buff[8]);
                data[4] = ASCII(buff[9], buff[10]);
                data[5] = ASCII(buff[11], buff[12]);
                data[6] = ASCII(buff[13], buff[14]);
                data[7] = ASCII(buff[15], buff[16]);
                data[8] = ASCII(buff[17], buff[18]);
                data[9] = ASCII(buff[19], buff[20]);
            }
            checkSum = 0;
            for(int j = 0 ; j < 8 ; j++) checkSum += data[j];
            if(checkSum != data[8]){
                for(int i = 0 ; i < 4 ; i++)data[i] = 0;
                for(int i = 4 ; i < 8 ; i++)data[i] = 0x7f;
            }
            i = 0;
        }
    }
}

uint8_t IM315PS4::ASCII(uint8_t data1, uint8_t data2){
    if(data1 >= '0' && data1 <= '9') data1 -= '0';
    else if(data1 >= 'A' && data1 <= 'F') data1 -= 'A' - 10;
    if(data2 >= '0' && data2 <= '9') data2 -= '0';
    else if(data2 >= 'A' && data2 <= 'F') data2 -= 'A' - 10;
    return 16*data1 + data2;
}

void IM315PS4::buttonState(){
    if(up()) printf("Up ");
    if(right()) printf("Right ");
    if(left()) printf("Left ");
    if(down()) printf("Down ");
    if(triangle()) printf("Triangle ");
    if(circle()) printf("Circle ");
    if(square()) printf("Square ");
    if(cross()) printf("Cross ");
    if(l1()) printf("L1 ");
    if(l2An() != 0) printf("L2:%2x ", l2An());
    if(l3()) printf("L3 ");
    if(r1()) printf("R1 ");
    if(r2An() != 0) printf("R2:%2x ", r2An());
    if(r3()) printf("R3 ");
    if(option()) printf("OPTIONS ");
    if(share()) printf("SHARE ");
    if(ps()) printf("PS ");
    if(touchpad()) printf("TOUCHPAD ");
    if(lsx() != 127) printf("LSX:%3d ", lsx());
    if(lsy() != 127) printf("LSY:%3d ", lsy());
    if(rsx() != 127) printf("RSX:%3d ", rsx());
    if(rsy() != 127) printf("RSY:%3d ", rsy());
    printf("\n");
}
void IM315PS4::printData(){printf("%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);}

void IM315PS4::checkError(){
    if(errorCheck[0] == data[9] && errorCheck[1] == data[9]){
        if(errorCheck[1] == data[9] && errorCheck[2] == data[9]){
            for(int i = 0 ; i < 4 ; i++)data[i] = 0;
            for(int i = 4 ; i < 8 ; i++)data[i] = 0x7f;
            data[8] = NEUTRAL;
        }
    }
    errorCheck[2] = errorCheck[1];
    errorCheck[1] = errorCheck[0];
    errorCheck[0] = data[9];
}

bool IM315PS4::neutral(){return data[8] == NEUTRAL ? 1 : 0;}

bool IM315PS4::up(){return data[1] & UP ? 1 : 0;}

bool IM315PS4::right(){return data[1] & RIGHT ? 1 : 0;}

bool IM315PS4::left(){return data[1] & LEFT ? 1 : 0;}

bool IM315PS4::down(){return data[1] & DOWN ? 1 : 0;}

bool IM315PS4::triangle(){return data[0] & TRIANGLE ? 1 : 0;}

bool IM315PS4::circle(){return data[0] & CIRCLE ? 1 : 0;}

bool IM315PS4::square(){return data[0] & SQUARE ? 1 : 0;}

bool IM315PS4::cross(){return data[0] & CROSS ? 1 : 0;}

bool IM315PS4::touchpad(){return data[0] & TOUCHPAD ? 1 : 0;}

bool IM315PS4::ps(){return data[0] & PS ? 1 : 0;}

bool IM315PS4::l1(){return data[0] & L1 ? 1 : 0;}

bool IM315PS4::r1(){return data[0] & R1 ? 1 : 0;}

bool IM315PS4::option(){return data[1] & OPTION ? 1 : 0;}

bool IM315PS4::share(){return data[1] & SHARE ? 1 : 0;}

bool IM315PS4::l3(){return data[1] & L3 ? 1 : 0;}

bool IM315PS4::r3(){return data[1] & R3 ? 1 : 0;}

uint8_t IM315PS4::l2An(){return data[2];}

uint8_t IM315PS4::r2An(){return data[3];}

uint8_t IM315PS4::lsx(){return data[4];}

uint8_t IM315PS4::lsy(){return data[5];}

uint8_t IM315PS4::rsx(){return data[6];}

uint8_t IM315PS4::rsy(){return data[7];}

IM315PS4::~IM315PS4(){
    delete device;
    delete buff;
    delete data;
}