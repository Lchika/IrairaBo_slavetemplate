# IrairaBo_slavetemplate

## 概要

- スレーブモジュールの共通部分だけ書いたコード  

## ピン関係

|ピン番号|役割|
|-|-|
|4|モジュール通過判定|
|5|コース接触判定|
|6|DIPスイッチbit0|
|7|DIPスイッチbit1|
|8|DIPスイッチbit2|
|9|DIPスイッチbit3|
|A4(18)|D-sub2番ピン(SDA)|
|A5(19)|D-sub3番ピン(SCL)|
|GND|D-sub1番ピン|

- 上記ピン番号はすべて仮決め
- I2C関係ピンは固定

## D-sub関係

- スレーブのアドレス指定：スタートモジュールに近い順に1,2,3...  
- D-sub関係はI2C(2,3番ピン)のみに集約し、D-subの4,5番ピン(ゴール通知・コース接触通知)は不要となった
- ただし、上記対応により各モジュールの通過判定が必須になった
  - どのモジュールと通信すればよいかわからないとI2C通信できないため

## モジュール共通部分の処理について

- D-sub関係の処理は`DsubSlaveCommunicator`クラス内の処理で完結する
- `setup()`内で`DsubSlaveCommunicator`クラスのインスタンス`*dsubSlaveCommunicator`を生成する
- DsubSlaveCommunicatorクラスは、マスタから通信開始通知を受け取ると、アクティブ状態になる
- アクティブ状態でなければ、モジュールは何もする必要がない
  - `loop()`内の以下のコードでアクティブ状態かどうか確認している
    ```c++
    if(DsubSlaveCommunicator::is_active()){
    ```
- D-sub関係処理は以下の部分で処理している
  ```c++
  dsubSlaveCommunicator->handle_dsub_event();
  ```
  - コース接触判定、コース通過判定もここで行っている
  - この関数は定期的に呼ぶ必要がある