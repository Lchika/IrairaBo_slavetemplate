/* slave_template.ino                      */
/* I2Cのスレーブとなるモジュールのテンプレコード */

#include "debug.h"
#include "dsub_slave_communicator.hpp"

#define PIN_GOAL_SENSOR     4   //  通過/ゴールしたことを検知するセンサのピン
#define PIN_HIT_SENSOR      5   //  当たったことを検知するセンサのピン
#define PIN_DIP_0           6   //  DIPスイッチbit0
#define PIN_DIP_1           7   //  DIPスイッチbit1
#define PIN_DIP_2           8   //  DIPスイッチbit2
#define PIN_DIP_3           9   //  DIPスイッチbit3

/* 変数宣言 */
unsigned char slv_address;      //  スレーブアドレス(0はゴールモジュール固定、それ以外は1~)
DsubSlaveCommunicator *dsubSlaveCommunicator = NULL;

void setup(void) {
  /* ここから各スレーブ共通コード */
  BeginDebugPrint();                //  デバッグ用
  /* ディップスイッチを入力として設定 */
  pinMode(PIN_DIP_0, INPUT);
  pinMode(PIN_DIP_1, INPUT);
  pinMode(PIN_DIP_2, INPUT);
  pinMode(PIN_DIP_3, INPUT);
  
  slv_address = ReadDipSwitch();    //  SLVアドレスを設定
  /* D-sub通信管理用インスタンスの生成 */
  dsubSlaveCommunicator = new DsubSlaveCommunicator(PIN_GOAL_SENSOR, PIN_HIT_SENSOR, slv_address);
  /* ここまで各スレーブ共通コード */

  /* ここから各モジュール独自コード */

  /* ここまで各モジュール独自コード */
}

void loop(void) {
  /* ここから各スレーブ共通コード */
  if(DsubSlaveCommunicator::is_active()){
    //D-sub関係イベント処理
    dsubSlaveCommunicator->handle_dsub_event();

    /* 各モジュール独自コードはここに書いてください */
    //hoge = fuga;

    /* 各モジュール独自コードここまで */
  }
}


/* ディップスイッチの値を10進数で読み取る */
unsigned char ReadDipSwitch(void) {
  unsigned char value = 0;

  if(digitalRead(PIN_DIP_0) == HIGH) {
    value += 1;
  }
  if(digitalRead(PIN_DIP_1) == HIGH) {
    value += 2;
  }
  if(digitalRead(PIN_DIP_2) == HIGH) {
    value += 4;
  }
  if(digitalRead(PIN_DIP_3) == HIGH) {
    value += 8;
  }

  return value;
}
