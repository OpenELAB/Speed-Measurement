
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Hi__ 👋, we’ve created a bicycle speed detection project using the M5StickC Plus 2 and a Hall Effect Unit. As the bicycle wheel rotates, the Hall sensor detects changes in the magnetic field and displays the speed data in real time on the M5StickC Plus 2. This is a great project for learning how to combine sensors with IoT devices. We'll guide you through the detailed steps below.

- 📝 Project Description
- ✨ Functional Features
- 🏗 Project Structure
- 🚀 Installation and Operation
- 🔧 Rationale
# Speed Measurement Project
## Project Description
This project utilizes the M5Stack Hall Effect Unit sensor. We began by attaching three small magnets to the inner rim of the wheel and mounting the Hall sensor to the frame. As the wheel turns, the magnets rotate with it. Each time a magnet passes by the Hall sensor, it generates an electrical signal. We first measure the circumference of the wheel and then calculate the time interval between two signals to determine how long it takes for the magnet to complete one full revolution with the wheel. By dividing the wheel's circumference by this time, we can calculate the bicycle's speed. However, it's important to implement an anti-shaking mechanism to prevent the Hall sensor from triggering multiple times in a short period when the wheel is spinning slowly, which could result in inaccurate speed readings.

## Functional Features
- ⏱️ Real-time speed display: The current speed of the bike is displayed in real-time on the M5StickC Plus 2 screen, providing precise speed measurements.
- 🧲 Hall Effect Sensor Detection: Utilizing the M5Stack Hall Effect Sensor, the bike’s speed is calculated by detecting the frequency of the magnet passing over the sensor.
- 🚫 Anti-shaking mechanism: A built-in anti-shaking algorithm prevents repeated signal triggering caused by slow wheel speeds or bumps, ensuring the accuracy of the speed display.
- 🚴‍♂️ Lightweight design: With the M5StickC Plus 2 as the controller, its compact and lightweight form makes it ideal for use on bicycles, taking up minimal space.
## Project structure
``` 
│── README.md             # Project description file
│── speed.ino             # Source code for the speed checker program
```
## Installation and operation  
### precondition
Software dependency: __Arduino IDE__ etc.  
Hardware requirements: __USB-C cable__, __M5StickCPlus2__,__Hall Effect Unit__， etc.  
Dependencies: __M5StickCPlus2 library__, __Arduino library__, etc.  
### Installation of dependencies
1、After installing the Arduino IDE, open the settings, paste the M5 development board link into the appropriate field, and click OK to save the changes.
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
### Rationale
1、First, we obtain the Hall Effect Unit and learn that whenever a magnet comes near, the indicator on the output pin lights up, and an electrical signal is sent from the I pin.  
![image](https://github.com/user-attachments/assets/54295e92-9a09-413d-bc22-1f558653af65)  
![image](https://github.com/user-attachments/assets/4533bdfa-d954-4d9a-abb7-8dd698ec3582)  
2、First, we began by defining the receive pins for the Hall sensor’s electrical signals.
```
#define WHEEL_CIRCUMFERENCE 2000  // Tire circumference in mm
#define HALL_PIN 33  // Hall Sensor Pins
#define DEBOUNCE_TIME 50  // Dithering time in milliseconds

#define INPUT_PIN 26
```
3、Initializing the Plus 2 display
```
// Initialize M5StickC Plus2
  M5.begin();
  
  // Setting the orientation and font of the display
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(4);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
```
4、Anti-shaking design: By detecting the interval between the electrical signals from the Hall sensor, we can determine whether repeated detection has occurred.
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
5、The key to obtaining the speed is calculating it by measuring the time interval between two signals and combining that with the circumference of the wheel, then displaying the result on the M5StickC Plus 2's screen.
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
1、First, download the zip archive. Once the download is complete, unzip it and open the speed.ino file.

![image](https://github.com/user-attachments/assets/c44796c7-88c2-4c29-af01-314654969dbb)

2、Connect the Plus2 to your computer using a USB-C cable. Then, go to Tools -> Port and select the correct port.

![image](https://github.com/user-attachments/assets/96a163ba-4613-4bd9-a76b-f56a3f1e7696)  

3、Click Compile, and once the compilation is complete, click Upload.

![image](https://github.com/user-attachments/assets/b28d30c5-f0b9-4af2-9723-8935a5390a40)   

### Project Showcase

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

