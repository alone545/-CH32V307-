#include <BleKeyboard.h>

BleKeyboard bleKeyboard("CH32_KEYBOARD", "Espressif", 100);

// HID 报告缓冲
static uint8_t hidReport[8];

void setup() {
  Serial.begin(115200);  // 用于调试输出
  Serial1.begin(115200, SERIAL_8N1, 16, 17);  // 配置串口1（接到 CH32V307）

  bleKeyboard.begin();  // 启动蓝牙键盘
}

void loop() {
  static uint8_t receivedByte = 0;  // 用来接收串口数据的一个字节
  static uint8_t frameState = 0;    // 用于帧头识别和接收数据的状态机

  if (Serial1.available()) {
    receivedByte = Serial1.read();

    switch (frameState) {
      case 0:  // 等待帧头（0x80）
        if (receivedByte == 0x80) {
          frameState = 1;  // 识别到帧头，进入读取 HID 报告状态
        }
        break;

      case 1:  // 读取 8 字节 HID 报告
        hidReport[0] = receivedByte;  // 第一个字节是帧头后的第一字节
        for (uint8_t i = 1; i < 8; i++) {
          if (Serial1.available()) {
            hidReport[i] = Serial1.read();
          }
        }

        // 打印接收到的 HID 报告
        Serial.print("Received HID Report: ");
        for (int i = 0; i < 8; i++) {
          Serial.print(hidReport[i], HEX);  // 打印每个字节的十六进制值
          Serial.print(" ");
        }
        Serial.println();  // 换行

        // 将 8 字节拷贝到 KeyReport 结构体
        KeyReport kr = { 0 };
        kr.modifiers = hidReport[0];   // 修饰键
        memcpy(kr.keys, &hidReport[2], 6);  // 从第三个字节开始是按键码

        // 发送 HID 报告到蓝牙
        bleKeyboard.sendReport(&kr);  // 发送 HID 报告

        frameState = 0;  // 重置状态机，准备接收下一帧
        break;
    }
  }

  delay(1);  // 稍微延迟，避免占用过多 CPU 资源
}
