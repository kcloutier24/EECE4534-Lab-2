/*
Katherine Cloutier and Jared Cohen
Lab 2
Issue 4:

Use the AXI Timers to create a PWM output on the LED0.
*/

#include <sys/signal.h>
#include <libzed/axi_gpio.h>
#include <libzed/zed_common.h>
#include <libzed/mmap_regs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <fcntl.h>
#include <sys/mman.h> //Kat: Unix header and is not available on Windows.

/// stop infinite loop flag
volatile static unsigned char stopLoop = 0;

// constants!
const float periodS = 0.001;
const float hightimeS = periodS * 0.5;
const unsigned long timerfreq = 100000000; //1Mhz timer speed
const float AXI_CLOCK_PERIOD = 1.0/timerfreq; 
const unsigned int MAX_COUNT = 0xFFFFFFFF;
const unsigned int TLR0 = (periodS / AXI_CLOCK_PERIOD) - 2; //Values to be stored in the TLR0 and TLR1 registers
const unsigned int TLR1 = (hightimeS / AXI_CLOCK_PERIOD) - 2;

#define UDT_MASK   (1<<1) //Define the necessary bits used for configuring the timer to PWM mode
#define GENT_MASK  (1<<2)
#define ARHT_MASK  (1<<4)
#define LOAD_MASK  (1<<5)
#define ENT_MASK   (1<<7)
#define PWMA0_MASK (1<<9)
#define ENALL_MASK (1<<10)

#define TCSR0_OFFSET 0x0 //Register offsets
#define TCSR1_OFFSET 0x10
#define TLR0_OFFSET 0x04
#define TLR1_OFFSET 0x14
#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x18


//GPIO Base Address
#define TIMER0_GPIO_ADDR 0x41240000
#define AXI_SIGNAL_MUX1_ADDR 0x41260000

// register offsets
#define REG_OFFSET(REG, OFF) ((REG) + (OFF))

/* @brief Handle signals
   @param num Signal Number */
static void signal_handler(int num)
{
  stopLoop = 1;
}

int main(void)
{
  // handle SIGINT (ctrl-c)
  signal(SIGINT, signal_handler);

  char* mux = map_region(AXI_SIGNAL_MUX1_ADDR, 32); //Create the MUX to send the PWM output to the LED

  clear_reg_mask(mux, 0, 0x1); //Set the bit 1 in the mux
  set_reg_mask(mux, 0, 0x1);

  char* timers = map_region(TIMER0_ADDR, 32);
  clear_reg_mask(timers, TCSR0_OFFSET, 0xFFF); //Clear the first twelve bits of the TCSR0
  set_reg_mask(timers, TCSR0_OFFSET, UDT_MASK|GENT_MASK|ARHT_MASK|PWMA0_MASK); //Set the necessary bits for configuration

  clear_reg_mask(timers, TCSR1_OFFSET, 0xFFF); //Clear the first twelve bits of the TCSR1
  set_reg_mask(timers, TCSR1_OFFSET, UDT_MASK|GENT_MASK|ARHT_MASK|PWMA0_MASK); //Set the necessary bits for configuration

  printf("TCSR0: 0x%X\n", read_reg(timers, TCSR0_OFFSET)); //Read the values to ensure they are correct
  printf("TCSR1: 0x%X\n", read_reg(timers, TCSR1_OFFSET));

  //Starting timer
  
  write_reg(timers, TLR0_OFFSET, TLR0); //Write the timer values to the respective timers
  write_reg(timers, TLR1_OFFSET, TLR1);

  printf("TCR0: 0x%X\n", read_reg(timers, TCR0_OFFSET)); //Print to ensure they are correct
  printf("TCR1: 0x%X\n", read_reg(timers, TCR1_OFFSET));

  set_reg_mask(timers, TCSR0_OFFSET, LOAD_MASK); //Load the timer values into the register
  clear_reg_mask(timers, TCSR0_OFFSET, LOAD_MASK);

  printf("TCR0: 0x%X\n", read_reg(timers, TCR0_OFFSET)); //Read it to ensure they are correct
  printf("TCR1: 0x%X\n", read_reg(timers, TCR1_OFFSET));

  set_reg_mask(timers, TCSR0_OFFSET, ENALL_MASK); //Enable all the timers
  clear_reg_mask(timers, TCSR0_OFFSET, ENALL_MASK); //Clear the bit because it doesn't affect the timers if it is off

  printf("TCSR0: 0x%X\n", read_reg(timers, TCSR0_OFFSET)); //Read back the control bits to ensure correctness
  printf("TCSR1: 0x%X\n", read_reg(timers, TCSR1_OFFSET));


  //Start pulsecap module
  int x = system("modprobe pulsecap");

  // get max number of edges
  x = system("cat /sys/class/pulsecap/pulsecap1/control/max_edges");

  // get current state
  x = system("cat /sys/class/pulsecap/pulsecap1/control/state");

  // start recording 20 edges
  x = system("echo 1000 > /sys/class/pulsecap/pulsecap1/control/capture");

  while(!stopLoop)
  {
    //printf("Timer0 Value: %d\n", read_reg(timers, TCR0_OFFSET));
    //printf("Timer0 Value: %d\n", read_reg(timers, TCR0_OFFSET));
  }

  printf("\nFinishing up...\n");

  // check how many edges have been recorded already
  x = system("cat /sys/class/pulsecap/pulsecap1/control/capture");

  // copy data into file
  x = system("cat /sys/class/pulsecap/pulsecap1/control/edge_data > issue4data1k");

  //Stop pulsecap system
  x = system("echo 1 > /sys/class/pulsecap/pulsecap1/control/cancel");


  //disable timers
  clear_reg_mask(timers, TCSR0_OFFSET, ENT_MASK);
  clear_reg_mask(timers, TCSR1_OFFSET, ENT_MASK);

  return 0;
}
