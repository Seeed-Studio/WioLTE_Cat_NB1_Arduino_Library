# WioLTE_Cat_NB1_Arduino_Library
THe Wio LTE Cat NB1 is boards combined with STM32F405RGT6 and the ublox SARA-R4 LTE Cat NB1 cellular module.


Install USB drive
===

**Windows Users**: Most versions of Windows won't automatically load the built-in driver for USB com ports. You'll have to download ST's USB driver:

Non-Windows XP [Users download version 1.4.0 drivers](http://www.espruino.com/files/stm32_vcp_1.4.0.zip). Unzip the file, run the executable, and then go to ```C:\Program Files (x86)\STMicroelectronics\Software\Virtual comport driver``` in Windows Explorer and double-click either ```dpinst_amd64.exe``` for 64 bit systems, or ```dpinst_x86.exe``` for 32 bit.

Windows XP [Users download version 1.3.1 drivers](http://www.espruino.com/files/stm32_vcp_1.3.1.zip). Unzip the file, run ```VCP_V1.3.1_Setup.exe```, and then go to ```C:\Program Files\STMicroelectronics\Software\Virtual comport driver``` in Windows Explorer and double-click the executable.

**Linux users** to ensure that you have the correct permissions to connect as a normal user you'll need to copy the file [45-espruino.rules](https://github.com/espruino/Espruino/blob/master/misc/45-espruino.rules) to ```/etc/udev/rules.d```, reload rules with ```udevadm control --reload-rules```, and ensure your user is in the plugdev group (you can check by typing ```groups```). You add it by typing ```sudo adduser $USER plugdev``` and then logging out and back in. Arch Linux users need to add their user to ```uucp``` and ```lock``` groups instead.

**Mac OS X and Chromebook Users**: The board will just plug in and work, without drivers!

Change dfu driver
===

**For windows users**: Press and hold BOOT button and connect to computer you will see ***STM32 Device in DFU Mode*** at device manager, this say that you need to use [zadig_xx.exe](https://zadig.akeo.ie/) to change DFU driver from ***STTub30*** to ***WinUSB*** see bellow. <br>

![zadig](img/zadig.png "zadig")

Usage
===
1. Install Arduino IDE. <br>
2. Copy the URL below, open Arduino board manager install **Seeed STM32F4 Boards**.

```
https://raw.githubusercontent.com/Seeed-Studio/Seeed_Platform/master/package_seeeduino_boards_index.json
```

![](img/boardManager.png)

3. Download this repo to your Arduino libraries path. <br>
4. Choose a sketch to compile. <br>
5. Before uploading the sketch, press and hold both BOOT0 and RST buttons, release the RST button than the BOOT0 button,in this way the board will go into **STM BOOLARDER** mode. <br>
6. Don't choose any port in tools label just upload the sketch.<br>




