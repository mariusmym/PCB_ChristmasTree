/*
 *  ATTiny 85/13A simple blink with multiple pins / leds. 
 *  the pattern of the leds can be changed via interrupt pin on PB2.
 *    
 *    Shared under the Creative Commons.
 *    
 */

// Requires headers for AVR defines and ISR function
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
