# simpleKeypad_I2C

https://github.com/citriena/simpleKeypad_I2C  
(c) citriena (citriena@yahoo.co.jp)

## 概要

Adafruitの[LCD Shield Kit w/ 16x2 Character Display - Only 2 pins used!](https://www.adafruit.com/product/772)、およびその互換シールド用のキーパッド入力補助ライブラリ

## 特徴

 * キーを押し続けた場合のキー入力リピート速度を指定可能
 * キーを押し続けた場合の最初のリピート開始までの時間を指定可能

## 使い方
### 必要なライブラリ
押されたキー情報の取得に[LiquidTWI2](https://github.com/lincomatic/LiquidTWI2)ライブラリが必要です。
LiquidTWI2はLCD表示機能もあるので（というかこちらが主）、別途LCD表示用のライブラリは不要です。

### コンストラクタ
````
simpleKeypad(int16_t interval, int16_t repeatDelay);
````
* interval（省略可;初期値: 200ms）
  * キーリピート時の入力間隔(ms)
* repeatDelay（省略可;初期値: 500ms）
  * キーリピートを開始するまでの時間(ms)
  * この設定値が上のキーリピート間隔（interval）より大きければ、最初のリピート入力開始までの時間はこの設定値となります。

### 機能
````
btnCODE_t read_buttons();
````
押されたキーのコードを返します。キーコードは以下の通り

 * None   - btnNONE   ( 0)
 * Select - btnSELECT ( 1)
 * Right  - btnRIGHT  ( 2)
 * Down   - btnDOWN   ( 4)
 * Up     - btnUP     ( 8)
 * Left   - btnLEFT   (16)
 * Void   - btnVOID   (32)

Voidは、物理的にキーは押されているけれども、キーリピート間隔の制限で押されているキーのキーコードを返さない場合です。

見てわかるようにキーとキーコードのビットが次の表のように対応しています。複数のキーを同時に押した場合はそれぞれ対応するビットが1になります。

|bit 4|bit 3|bit 2|bit 1|bit 0 |
|-----| ----|---- |-----|----- |
|Left |  Up |Down |Right|Select|

````
void repeatInterval(int16_t interval);
````
キーリピート間隔を指定します(ms)。コンストラクタで指定できるので、普通は使うことはないと思います。

### サンプルスケッチ
具体的な使い方はサンプルスケッチを見てください。

## 履歴

### 1.0.0 - Mar  05, 2021
