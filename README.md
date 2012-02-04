# FPGA を ADK もしくは MicroBridge で Android と接続する

## 概要
* Arduino 用 USB Host Shield を接続した FPGA を Android 端末を USB ケーブルで接続
* Android との接続には ADK もしくは MicroBridge を使用可能
* FPGA 側では CPU コア NiosII で ADK もしくは MicroBridge のファームウェアを実行
* 使用言語: C++, Verilog-HDL

## 対応 FPGA ボード
以下の条件を全て満たすもの
* Altera FPGA 搭載
* NiosII 用に 16KByte 程度のメモリを使用可能
* USB Host Shield (もしくは MAX3421E) と SPI (SS, SCLK, MOSI, MISO), INT が接続可能

## 動作確認環境
* 環境1 (ADK & MicroBridge)
    * FPGA ボード: Terasic DE0-Nano (Cyclone IV EP4CE22 搭載)
    * USB Host Shield: sparkfun DEV-09947
    * 開発環境: QuartusII 11.0sp1 Web Edition
    * Android: Nexus S (Android 2.3.4)
    * 詳細は de0nano/README に記載

## ディレクトリ構造
* de0nano/ .... DE0-Nano 論理合成等
* software/
    * common/ .... ソースコード共通部分
    * de0nano_bsp/ .... DE0-Nano 用 BSP
    * de0nano_microbridge/ .... DE0-Nano 用 MicroBridge デモ
    * de0nano_adk/ .... DE0-Nano 用 ADK デモ

