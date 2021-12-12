# PCB_ChristmasTree

My own version of a PCB Christmas tree, using an ATtiny85 / ATtiny13/13A.

- Some of the ideas are my own, others are _borrowed_ from various projects around the web.
- There are 11 LEDs controlled by four ATtiny pins, plus a TTP223 touch sensor linked to another ATtiny pin which acts as a switch who toggle between different lightning patterns (including an OFF "pattern").
- I designed the PCB in order to be able to use both SMD and classic 3mm LEDs.
- Please read this file **entirely** before start.

![PCB_ChristmasTree](https://user-images.githubusercontent.com/33284097/145599348-2ee62fd5-432d-4f92-88df-88caba1b6874.gif)

# Hardware

## BOM 

- 1x Christmas Tree PCB
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
 
- Please be aware when you order the PCB. The PCB is designed for a **thickness of 1.2mm**.If you want to use SMD leds you need to order the PCB with a **1.2mm thickness** , otherwise you won't be able to solder the LEDs. If you plan to use regular LEDs and you want a 1.6 mm thick PCB, you have to adjust **the width of that middle cuts** (where PCBs interconnect with eachother and make it 1.6mm in width as well).

![8](https://user-images.githubusercontent.com/33284097/145671972-7d71fac2-ec73-46ed-a595-982298f65a97.jpeg)

 
 ## Soldering SMD LEDs
 
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



 # Software
 
 ## Implementation
 
 - I'm just a regular guy passionate about making stuff, so dont expect a very neat code.
 - I made a small sketch using Arduio IDE and ATtiny registers in order to reduce the size of it a little bit, so I can upload it into ATtiny13A as well (which, as you know, have a lot less memory available).

```C++
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>


// Define pins for switch the LED, plus the chosen interrupt for reacting to

#define INTERRUPT_PIN PCINT2  // This is PB2 per the schematic
#define DATADIRECTIONPIN DDB2 //page 64 data sheet
#define INT_PIN PB2           // Interrupt pin of choice: PB1 (same as PCINT2) - Pin 7

#define LED_PIN1 PB0           // PB0 - Pin 5
#define LED_PIN2 PB1           // PB1 - Pin 6
#define LED_PIN3 PB3           // PB3 - Pin 2
#define LED_PIN4 PB4           // PB4 - Pin 3


/*
 * Alias for the ISR: "PCINT_VECTOR" (Note: There is only one PCINT ISR. 
 * PCINT0 in the name for the ISR was confusing to me at first, 
 * hence the Alias, but it's how the datasheet refers to it)
 */
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarity

volatile uint8_t pushCount = 0; // variables used within ISR must be declared Volatile.

void setup() {
    cli();                    // Disable interrupts during setup
    
    DDRB = (1 << LED_PIN1) | (1 << LED_PIN2) 
         | (1 << LED_PIN3) | (1 << LED_PIN4); //replaces pinMode(LED_PIN1, OUTPUT);

    //configuring and enabling the interrupt 
 
    PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)

    GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask

    DDRB &= ~(0 << DATADIRECTIONPIN);  //replaces pinMode(INT_PIN, INPUT);
    
    PORTB |= (1<< INT_PIN); //disable pull-up. must hook up pulldown resistor [ 1K ].

    sei();                            //last line of setup - enable interrupts after setup

}


void loop() {

  switch(pushCount){
    case 0 :
    pattern1();
    break;
    case 1 :
    pattern2();
    break;
    case 2 :
    pattern3();
    break;
    case 3 :
    pattern4();
    break;
    default:
    pattern1();
  }

}


// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny85 only has one handler
ISR(PCINT_VECTOR)
{
    if((PINB >> PINB2)&1!=0) {  // or if ( (PINB & PINB2) != 0 ) or (PINB >> PINB2)&1
      if(pushCount > 3) {
        pushCount = 0;
      }else{
        pushCount ++;
      }   
  }
  delay(50); //small debounce
}

void pattern1(){
  PORTB |= (1 << LED_PIN2) | (1 << LED_PIN4); //replaces digitalWrite(LED_PINx,HIGH);
  delay(150);
  PORTB &= ~((1 << LED_PIN2) | (1 << LED_PIN4)); //replaces digitalWrite(LED_PINx,LOW);
  delay(150);
  PORTB |= (1 << LED_PIN1) | (1 << LED_PIN3); 
  delay(150);
  PORTB &= ~((1 << LED_PIN1) | (1 << LED_PIN3));
  delay(150);
}

void pattern2(){
  PORTB |= (1 << LED_PIN1);
  delay(200);
  PORTB &= ~(1 << LED_PIN1);
  PORTB |= (1 << LED_PIN3);
  delay(200);
  PORTB &= ~(1 << LED_PIN3);
  PORTB |= (1 << LED_PIN2);
  delay(200);
  PORTB &= ~(1 << LED_PIN2);
  PORTB |= (1 << LED_PIN4);
  delay(200);
  PORTB &= ~(1<< LED_PIN4);
  
}

void pattern3(){
  PORTB |= (1 << LED_PIN2) | (1 << LED_PIN4);
  delay(80);
  PORTB &= ~((1 << LED_PIN2) | (1 << LED_PIN4));
  delay(80);
  PORTB |= (1 << LED_PIN2) | (1 << LED_PIN4);
  delay(80);
  PORTB &= ~((1 << LED_PIN2) | (1 << LED_PIN4));
  delay(80);
  PORTB |= (1 << LED_PIN1) |  (1 << LED_PIN3);
  delay(80);
  PORTB &= ~((1 << LED_PIN1) | (1 << LED_PIN3));
  delay(80);
  PORTB |= (1 << LED_PIN1) |  (1 << LED_PIN3);
  delay(80);
  PORTB &= ~((1 << LED_PIN1) | (1 << LED_PIN3));
  delay(80);
}

void pattern4(){  // POWER OFF
  PORTB &= ~((1 << LED_PIN1) | (1 << LED_PIN2) | (1 << LED_PIN3) | (1 << LED_PIN4)); //LEDs off
  power_off();
}

void power_off()
{
    cli();                               // Disable interrupts before next commands
    wdt_disable();                       // Disable watch dog timer to save power
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode power down
    sleep_enable();
    sleep_bod_disable(); // Disable brown-out detector
    sei();               // Enable interrupts
    sleep_cpu();
    sleep_disable();
}
```
 
 ## Compiling and Uploading
 
**BEWARE ! - you must upload the sketch BEFORE soldering the microchip to the PCB !** 
You can try to do it after, but sometimes you'll get an uploading error, so is safer to do it before soldering.
 - As I said earlier, I'm just a regular guy, so I stick to the basics and I used Arduino IDE to compile and upload the sketch. Depending on what kind of chip do you use, you will need one of the following board packages installed in your Arduino IDE.
 - --------------------------------------------------
  - For **ATtiny85** you will need damellis attiny board package installed https://github.com/damellis/attiny  then :
    - Go to **Tools -> Board -> ATtiny Microcontrollers** and select **ATtiny25/45/85**.
    - Go to **Tools** and choose the following board options:
      - **Processor:** ATtiny85
      - **Clock:** Internal 1 Mhz
      - **Programmer:** Arduino as ISP
    - --------------------------------------------------
  - For **ATtiny13/13A** you will need MCUdue MicroCore board package installed https://github.com/MCUdude/MicroCore then:
    - Go to **Tools -> Board -> MicroCore** and select **ATtiny13**.
    - Go to **Tools** and choose the following board options:
      - **BOD:** 1.8 V
      - **EEPROM:** Retained
      - **Clock:** 1.2 Mhz internal
      - **Timming:** Micros disabled
      - **Programmer:** Arduino as ISP slow (MicroCore)
  - --------------------------------------------------
  
- To upload the sketch I used a programmer testing clip ( https://www.aliexpress.com/item/1005002401064440.html ) and an Arduino Uno board.

![5](https://user-images.githubusercontent.com/33284097/145583569-b59e9766-1832-4188-b715-24c346c8e05d.jpeg)

 - Proceed to solder the microchip, and all of the other remaining components, insert the PCB into each other.
 - **Beware that in the lower part of the PCBs there are two little circles that needs to be _face to face_ when you solder the PCBs together (see the picture below)** 
 
![7](https://user-images.githubusercontent.com/33284097/145625428-3a7c118d-9201-4078-b927-46d33bdeb2f6.jpeg)

- After you placed the PCBs in the right way, align them properly and then solder **ALL** of the connecting pads.

![6](https://user-images.githubusercontent.com/33284097/145626576-d5d8aa6d-4edb-4404-9ec8-0659f093521a.jpg)

- **Connect a microUSB cable and enjoy your little PCB Christmas tree !** ☺




