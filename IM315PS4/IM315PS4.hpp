#ifndef IM315PS4_H
#define IM315PS4_H
#include "mbed.h"

//DATA[1]
#define TRIANGLE 0x01
#define CIRCLE   0x02
#define SQUARE   0x04
#define CROSS    0x08
#define TOUCHPAD 0x10
#define PS       0x20
#define L1       0x40
#define R1       0x80

//DATA[2]
#define UP       0x01
#define RIGHT    0x02
#define LEFT     0x04
#define DOWN     0x08
#define OPTION   0x10
#define SHARE    0x20
#define L3       0x40
#define R3       0x80

//DATA[8]
#define NEUTRAL 0xfc

/*IM315(IM920)を用いてPS4コントローラ(DualShock4)を使う *受信
  例 F446RE用IM315接続基板の場合
  IM315PS4 ds4(A0, A1);                           マイコン ~ PC間のボーレート指定なし(9600)
  
  
  DualShock4は読み取り基板を用いてボタンの状態を読み取り下記のような8バイトの文字列データ(16進数表記)にして送信してください
 /-------------------------------------------- 
  DATA[0]           上の表に対応するように
  DATA[1]           上の表に対応するように
  DATA[2]           0x00 ~ 0xff
  DATA[3]           0x00 ~ 0xff
  DATA[4] ~ DATA[7] 左右スティックのアナログ値 4 -> LスティックのX値 5 -> LスティックのY値 6, 7 -> Rスティックで同様
  
  例 01040000ff000000    △ボタンと十字左ボタンが押され、Lスティックを最大まで右に倒した場合
 /--------------------------------------------
 
 
 /---------- sbdbtライブラリとの相違点 ----------
  ・ニュートラル関数がない
  ・L2 R2ボタンがアナログでのみ使用可能
  ・L3 R3ボタン(スティック押し込み)が使用可能
  ・PSボタン タッチパッドが使用可能
  ・スティックの入力範囲が 0 ~ 255 となっている
  ・スティックの初期値(ニュートラル値)は 0x7f(127)
 /--------------------------------------------
 
*/

class IM315PS4{
public:
    IM315PS4(PinName TX, PinName RX);
    void buttonState();                           //全ボタン, スティックの状態表示
    void printData();                             //受信データ表示
    void checkError();
    bool up();
    bool right();
    bool left();
    bool down();
    bool triangle();
    bool circle();
    bool square();
    bool cross();
    bool l1();
    bool r1();
    bool l3();
    bool r3();
    bool option();
    bool share();
    bool touchpad();
    bool ps();
    bool neutral();
    uint8_t l2An();
    uint8_t r2An();
    uint8_t lsx();
    uint8_t lsy();
    uint8_t rsx();
    uint8_t rsy();
    ~IM315PS4();
private:
    RawSerial *device;
    Serial *_pc;
    uint8_t *data;
    uint8_t *buff;
    void getData();
    uint8_t ASCII(uint8_t char1, uint8_t char2);
    int i;
    uint8_t checkSum;
    uint8_t errorCheck[3];
    static constexpr uint8_t header = ':';
    static constexpr int charSize = 21;
    static constexpr int dataSize = 10;
};
#endif