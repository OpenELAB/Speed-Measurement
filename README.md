
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Hi__ 👋, 我们制作了一个基于M5StickC Plus 2与Hall Effect Unit 结合的自行车速检测的项目，当车轮转动时，会通过M5StickC Plus 2将速度数据显示出来。接下来我们将会按照如下步骤来讲解这个项目。
- 📝 Project Description
- ✨ Functional Features
- 🏗 Project Structure
- 🚀 Installation and Operation
- 🔧 Instructions for use
# Speed Measurement Project
## Project Description
这个项目采用了M5Stack Hall Effect Unit传感器，我们首先将三颗小的磁铁固定在车轮内圈上，并将霍尔传感器固定在车轮钢架处，使得车轮转动带动磁铁转动，每当磁铁经过霍尔传感器处就会收到电信号，根据首先测量出车轮大小，通过两次电信号的间隔从而得到磁铁跟随车轮转动一圈所需的时间，通过车轮长度除以时间我们就能得到自行车的速度，但是需要注意的是我们需要添加防抖，防止出现车轮速度过慢，霍尔传感器短时间内多次检测到信号，从而显示出错误的速度。

## Functional Features
- ⏱️ 实时速度显示 ：通过M5StickC Plus 2的屏幕实时显示自行车的当前速度，提供精确的速度测量。
- 🧲 霍尔效应传感器检测 ：利用M5Stack的霍尔效应传感器，通过检测磁铁经过传感器的频率来计算自行车速度。
- 🚫 防抖机制 ：内置防抖算法，避免因车轮转速过慢或颠簸导致的重复信号触发，确保速度显示的准确性。
- 🚴‍♂️ 轻量级设计：使用M5StickC Plus 2作为控制器，小巧轻便，适用于自行车的移动场景，不占用空间。
## Project structure
``` 
│── README.md             # Project description file
│── speed.ino    # 速度检测程序源代码
```
## Installation and operation

### precondition
Software dependency: __Arduino IDE__ etc.  

Hardware requirements: __USB-C cable__, __M5StickCPlus2__,__Hall Effect Unit__， etc.  

Dependencies: __M5StickCPlus2 library__, __Arduino library__, etc.  
### Installation of dependencies
1、After installing the Arduino IDE, open the settings, paste the M5 development board link into the designated field, and click OK to save the changes.
```
https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
```
![QQ_1726105473838](https://github.com/user-attachments/assets/367bd060-13ab-4eda-9a43-13fbc0250580)  
  
2、Open Tools->Board->Boards Manager

![image](https://github.com/user-attachments/assets/dbee7434-4453-462b-a75f-32ac5b8d714c)  
  
3、Search for M5Stack in the Arduino Library Manager and install it. Since it's already installed on this system, I won’t repeat the installation process.

![QQ_1726105854495](https://github.com/user-attachments/assets/11b18b6c-c8db-4ea4-b209-d22dd26eebbe) 

4、Select development version, Tools->Board->M5Stack Arduino->M5StickCPlus2 

![image](https://github.com/user-attachments/assets/804770ad-ce05-4d9c-9cec-183462cace63)  

5、Next, install the M5StickCPlus2 library by selecting Tools -> Manage Libraries, searching for "M5StickCPlus2," and clicking Install. If the library is already installed, the installation process will be skipped.

![QQ_1726106703496](https://github.com/user-attachments/assets/312bc9e1-521c-479e-831a-a3c22e45a6ec)  
### 原理解释
1、首先我们拿到Hall Effect Unit知道每当有磁铁靠近输出引脚将会亮起并产生电信号从I处发出

![image](https://github.com/user-attachments/assets/54295e92-9a09-413d-bc22-1f558653af65)  
![image](https://github.com/user-attachments/assets/4533bdfa-d954-4d9a-abb7-8dd698ec3582)  
2、首先我们先定义了霍尔传感器电信号的接收引脚
```
#define WHEEL_CIRCUMFERENCE 2000  // Tire circumference in mm
#define HALL_PIN 33  // Hall Sensor Pins
#define DEBOUNCE_TIME 50  // Dithering time in milliseconds

#define INPUT_PIN 26
```
3、初始化Plus 2的显示屏 
```
// Initialize M5StickC Plus2
  M5.begin();
  
  // Setting the orientation and font of the display
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(4);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
```
4、防抖设计、通过对霍尔传感器发出的电信号发出间隔来判断是否重复检测。
```
int counter = 0;  // Record the number of tire rotations
unsigned long lastPulseTime = 0;  // Time of last detected pulse
unsigned long lastDisplayTime = 0;  // The last time the update was displayed
unsigned long currentTime = 0;

currentTime = millis();
  // Read Hall sensor status
  bool hallState = digitalRead(HALL_PIN);

  // Changes in Hall sensors are detected and jitter has to be filtered out
  if (hallState == LOW && (currentTime - lastPulseTime > DEBOUNCE_TIME)) {
    counter++;  // Increased count indicates one tire rotation.
    lastPulseTime = currentTime;  // Update last pulse time
  }
```
5、获取速度主要在于通过得到时间间隔，以及车轮长度从而计算出速度，呈现在Plus 2的显示屏上
```
// Update screen display every 1 second
  if (currentTime - lastDisplayTime >= 1000) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(20, 20);
    
    // Calculation of velocity in millimeters per second, converted to meters per second
    double speed = ((double)counter * (double)WHEEL_CIRCUMFERENCE) / 1000.0;
    
    // Create a character buffer to store the formatted speed value
    char buffer[20];
    snprintf(buffer, sizeof(buffer), " Speed:     %.2f m/s", speed);

    // Prints formatted speed value to LCD
    M5.Lcd.print(buffer);
    
    // Reset Counter
    counter = 0;
    
    // Update the displayed time
    lastDisplayTime = currentTime;
  }
```
### compile and run
1、首先我们先下载压缩包，下载后解压打开文件speed.ino.

![image](https://github.com/user-attachments/assets/c44796c7-88c2-4c29-af01-314654969dbb)

2、Connect the Plus2 to your computer using a USB-C cable. Then, go to Tools -> Port and select the correct port.

![image](https://github.com/user-attachments/assets/96a163ba-4613-4bd9-a76b-f56a3f1e7696)  

3、Click Compile, and once the compilation is complete, click Upload.

![image](https://github.com/user-attachments/assets/b28d30c5-f0b9-4af2-9723-8935a5390a40)   

### 项目展示

## How to contact the maintainer or developer
__OpenELAB:__   
[![OpenELAB_logo_resized_150](https://github.com/user-attachments/assets/5d3de375-359c-46a3-96bb-aaa211c6c636)](https://openelab.io)  
__YouTube:__  
[![youtube_logo_200x150](https://github.com/user-attachments/assets/d2365e7f-4ffe-4124-bf62-21eba19a71e4)](https://www.youtube.com/@OpenELAB)  
__X :__  
[![X_logo_150x150](https://github.com/user-attachments/assets/4ad5095f-2573-4791-9360-b355530093bf)](https://twitter.com/openelabio)  
__FaceBook:__  
[![facebook_logo_cropped_150x150](https://github.com/user-attachments/assets/52f2dc9a-a564-49a5-b72e-30eafbbc281f)](https://www.facebook.com/profile.php?id=61559154729457)  
__Discord__  
[![resized_image_150x150](https://github.com/user-attachments/assets/93ecd098-3391-45bb-9d80-b166c197a475)](https://discord.gg/VQspWyck)

## Material Purchase Links
[M5StickCPlus2](https://openelab.io/products/m5stickcplus2?_pos=2&_sid=d60b08054&_ss=r)  

[USB-C](https://openelab.io/products/usb-a-to-usb-type-c-cable-white?_pos=2&_psq=USB-C&_ss=e&_v=1.0)

