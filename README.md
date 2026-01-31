# libgpiod-I2C-TF-Luna-CPP
Uses the libgpiod library to get sensor data from the TF-Luna using C++ on the Raspberry Pi and I2C Communication.

The TF-Luna is a LiDAR (Light Radar) that measures accurate, single-point distance measurement in a small package that is ideal for robotics, and obstacle detection. Attached is code from C++ to retreive and set information on the TF-Luna, the header MDLTFLuna25.h containing library code is specific to the TF-Luna.

![](https://github.com/eugenedakin/libgpiod-I2C-TF-Luna-CPP/blob/main/ScreenGrab.png)

1. install Raspberry Pi OS (64-bit)
2. Open a terminal and type the following commands:
3. sudo apt install libwxgtk3.2-dev build-essential
4. 	sudo apt install -y libi2c-dev
5. 	sudo apt install libgpiod-dev
6. 	make sure TF-Luna is detected at &h10: i2cdetect -y 1
7. 	Create the TF-Luna example program in a terminal with: g++ TFLunaCPP25.cpp -o TFLunaCPP25 `wx-config –cxxflags –libs`
8. 	Run the program with: ./TFLunaCPP25.cpp

wx.h is located in usr/include/wx-3.2/wx/wx.h

Below are the breadboard connections with the Raspberry Pi and TF-Luna.
![](https://github.com/eugenedakin/libgpiod-I2C-TF-Luna-CPP/blob/main/LiDARBreadboardRev2.png)

A description of the button functions are shown here:
![](https://github.com/eugenedakin/libgpiod-I2C-TF-Luna-CPP/blob/main/ButtonDescription.png)
