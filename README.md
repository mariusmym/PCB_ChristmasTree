# PCB_ChristmasTree

My own reinterpretation of a PCB Christmas tree. 

- Some of the ideas are my own, others are borrowed from various projects around the web.
- There are 11 LEDs controlled by four ATtiny pins, plus a TTP223 touch sensor linked to another ATtiny pin which acts as a switch who toggle between different lightning patterns (including an OFF "pattern").
- I designed the PCB in order to be able to use both SMD and classic 3mm leds.

## BOM 

- 1x ATtiny 85 / 13A
- 1x TTP223E-BA6
- 1x microUSB connector (used only for power)
- 1x AMS1117 3.3V SOT223
- 1x 30pF capacitor
- 1x 0.1uF capacitor
- 1x 1kΩ resistor
- 1x 0805 LED
- 2x 1uF capacitor
- 11x 470Ω resistor
- 11x 1206 LED (or classic 3mm leds)

 ## About the PCB
 
- Please be aware when you order the PCB. The PCB is designed for a **thickness of 1.2mm**.If you want to use SMD leds you need to order the PCB with a **1.2mm tichness** , otherwise you won't be able to solder the LEDs. If you plan to use regular LEDs and you want a 1.6 mm thick PCB, you have to adjust the width of that middle cut (where PCBs interconnect with eachother and make it 1.6mm in width).
 
 ## Two words about soldering SMD LEDs
 
- The PCB is designed to allow the vertical mount of the SMD LEDs on the edges of those cutouts. This way you can see them from both sides of the PCB.
- In order to do that you have to apply some solder on one of the pads and then place the LED on the edge with the help of a tweezer.
 - Use the same tweezer to keep the led against that edge and apply some more solder until the pads on the LED connect with the PCB pad. 
 
 ![1](https://user-images.githubusercontent.com/33284097/145580714-597fc16f-fdea-422a-a43f-3ff2a5e16f6b.jpeg)
  
 ![2](https://user-images.githubusercontent.com/33284097/145580819-ba80ed19-8baa-4e0d-b862-c0c5cefcba12.jpeg)
 
 - Make some final touch and you're done ! 

![3](https://user-images.githubusercontent.com/33284097/145580881-8666a608-05f6-4b3a-ad52-8d6e7d0e8b59.jpeg)

 - Now you have to start over on the other side of the LED and then again for the other 10 LEDs :D !
 ![4](https://user-images.githubusercontent.com/33284097/145580998-8b2ee41b-caf5-4cb1-81a2-4e128e31540a.jpeg)

 - It will become easier after few LEDs but it will be hard anyway.
 
 - Is definitely easier to use normal 3mm LEDs, but to be honest i like it more with the SMD LEDs.
 
 ![3mm](https://user-images.githubusercontent.com/33284097/145581015-fbfd24c9-448b-4595-a6d0-730f812f5741.jpeg)



 ## Uploading the sketch
 
 
**Beware that you must upload the sketch BEFORE soldering the microchip to the PCB !**

 - I'm just a regular guy passionate about making stuff, so dont expect a very neat code. 
 - I made a small sketch using Arduio IDE and ATtiny registers in order to reduce the size of it a little bit, so I can upload it on ATtiny13A (which, as you know, have a lot less memory available).
  - You will need damellis attiny library https://github.com/damellis/attiny if you want to use ATtiny85.
  - If you use an ATtiny13A you will need MCUdue MicroCore library https://github.com/MCUdude/MicroCore .
  For MicroCore library I used the following settings 
  
- BOD: 1.8 V
- EEPROM: Retained
- Clock: 1.2 Mhz internal
- Timming: Micros disabled
- Programmer : Arduino as ISP slow (MicroCore)
  
To upload the sketch i used a programmer testing clip https://www.aliexpress.com/item/1005002401064440.html.

![5](https://user-images.githubusercontent.com/33284097/145583569-b59e9766-1832-4188-b715-24c346c8e05d.jpeg)

