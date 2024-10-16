#include <M5StickCPlus2.h>

#define WHEEL_CIRCUMFERENCE 2000  // 轮胎周长，单位为毫米
#define HALL_PIN 33  // 霍尔传感器引脚
#define DEBOUNCE_TIME 50  // 消抖时间，单位为毫秒

#define INPUT_PIN 26

int counter = 0;  // 记录轮胎旋转的次数
unsigned long lastPulseTime = 0;  // 上一次检测到脉冲的时间
unsigned long lastDisplayTime = 0;  // 上一次更新显示的时间
unsigned long currentTime = 0;

void setup() {
  // 初始化M5StickC Plus2
  M5.begin();
  
  // 设置显示屏的方向和字体
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(4);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  
  // 设置霍尔传感器引脚为输入模式
  pinMode(HALL_PIN, INPUT_PULLUP);
  pinMode(INPUT_PIN,OUTPUT); //初始化引脚26用于代替按键M5-A
  
  // 初始化时间
  lastDisplayTime = millis();
}

void loop() {
  currentTime = millis();
  
  // 读取霍尔传感器状态
  bool hallState = digitalRead(HALL_PIN);

  // 检测到霍尔传感器的变化，并且要过滤抖动
  if (hallState == LOW && (currentTime - lastPulseTime > DEBOUNCE_TIME)) {
    counter++;  // 计数增加，表示轮胎转了一圈
    lastPulseTime = currentTime;  // 更新上次脉冲时间
  }

  // 每隔1秒更新一次屏幕显示
  if (currentTime - lastDisplayTime >= 1000) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(20, 20);
    
    // 计算速度，单位为毫米/秒，转换为米/秒
    double speed = ((double)counter * (double)WHEEL_CIRCUMFERENCE) / 1000.0;
    
    // 创建一个字符缓冲区来存储格式化后的速度值
    char buffer[20];
    snprintf(buffer, sizeof(buffer), " Speed:     %.2f m/s", speed);

    // 打印格式化后的速度值到LCD
    M5.Lcd.print(buffer);
    
    // 重置计数器
    counter = 0;
    
    // 更新显示的时间
    lastDisplayTime = currentTime;
  }
}
