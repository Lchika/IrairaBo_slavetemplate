/* slave_template.ino                      */
/* I2Cのスレーブとなるモジュールのテンプレコード */

#include <Wire.h>

#define SLV_ADDRESS     1 //スレーブアドレス(0はゴールモジュール固定、それ以外は1~)
#define PIN_GOAL        4 //ゴール判定用ピン
#define PIN_HIT         5 //当たった判定用ピン
#define PIN_GOAL_SENSOR 6 //通過/ゴールしたことを検知するセンサのピン
#define PIN_HIT_SENSOR  7 //当たったことを検知するセンサのピン
#define MASTER_BEGIN_TRANS 0 //通信を開始すること
#define MASTER_DETECT_HIT  1 //HITを受信したこと
#define MASTER_DETECT_GOAL 2 //通過/ゴールを受信したこと

/* 変数宣言 */
unsigned char active = 0; //0のときこのモジュール内にいない/1のときこのモジュール内にいる

void setup(void) {
  /* ここから各スレーブ共通コード */
  Wire.begin(SLV_ADDRESS);     //スレーブアドレスをSLV_ADDRESSとしてI2C開始
  Serial.begin(57600);          //デバッグ用
  pinMode(PIN_GOAL, INPUT); //通過判定用ピンを入力として設定
  pinMode(PIN_HIT, INPUT);     //当たった判定用ピンを入力として設定
  /* ここまで各スレーブ共通コード */

  /* ここから各モジュール独自コード */

  /* ここまで各モジュール独自コード */
}

void loop(void) {
  /* ここから各スレーブ共通コード */
  if(active == 0) {
    if(Wire.read() == MASTER_BEGIN_TRANS) {
      active = 1;
      pinMode(PIN_GOAL, OUTPUT); //通過/ゴール判定ピンを出力に設定　
      pinMode(PIN_HIT, OUTPUT); //当たった判定ピンを出力に設定
      digitalWrite(PIN_GOAL, LOW);
      digitalWrite(PIN_HIT, LOW);
    }
  } else {
    /* 通過/ゴールを検知したとき */
    if(digitalRead(PIN_GOAL_SENSOR) == HIGH) {
      digitalWrite(PIN_GOAL, HIGH);
      /* マスタがHIGHを検知したらそのことをメッセージで送信するので、受信するまで待機する */
      while(Wire.read() == MASTER_DETECT_GOAL) {}
      /* 通過/ゴール判定ピン、当たった判定ピンをLOWにしてから入力に切り替える */
      digitalWrite(PIN_GOAL, LOW);
      digitalWrite(PIN_HIT, LOW);
      pinMode(PIN_GOAL, INPUT);
      pinMode(PIN_HIT, INPUT);
      active = 0;
    }
    /* 当たったことを検知したとき */
    if(digitalRead(PIN_HIT_SENSOR) == HIGH) {
      digitalWrite(PIN_HIT, HIGH);
      /* マスタがHIGHを検知したらそのことをメッセージで送信するので、受信するまで待機する */
      while(Wire.read() == MASTER_DETECT_HIT) {}
      digitalWrite(PIN_HIT, LOW);
    }
  /* ここまで各モジュール共通コード */

  /* ここから各モジュール独自コード */

  /* ここまで各モジュール独自コード */
  }
}
