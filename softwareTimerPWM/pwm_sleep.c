/*
Katherine Cloutier and Jared Cohen
Lab 2
Issue 3: software timer PWM



Now, instead of the busyWait function, 
change your implementation so that it uses the OS sleep function usleep 
(check man pages for description).
*/

#include <sys/signal.h>
#include <libzed/axi_gpio.h>
#include <libzed/zed_common.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <math.h>   //for pow
#include <fcntl.h>
#include <sys/mman.h> //Kat: Unix header and is not available on Windows.

/// stop infinite loop flag
volatile static unsigned char stopLoop = 0;

static int fd;

// constants!
const unsigned long period = 1000; // total cycle of 1ms (1000us)
const unsigned long duty_cycle = period * 0.5;        // one duty cycle lasting 50%.



// LED GPIO Base Address
#define LED_GPIO_ADDR 0x41200000

unsigned int *gpioregs = NULL;

// register offsets
#define REG_OFFSET(REG, OFF) ((REG) + (OFF))

// map GPIO peripheral; base address will be mapped to gpioregs pointer
static int map_gpio(unsigned int** gpioregs, unsigned int gpio_addr)
{

    if (!gpioregs || !gpio_addr)
    {
        // invalid pointers
        return 1;
    }

    fd = open( "/dev/mem", (O_RDWR | O_SYNC)); //Open /dev/mem and check for errors
    if (fd == -1)
    {
          printf("/dev/mem could not be opened\n");
          return 1;
    }

    // use mmap here to map the appropriate address (gpio_addr) into the gpioregs pointer passed
    // by the caller.
    // return non-zero codes on errors!
    size_t len = 0x10000; //Span from the one GPIO to the next
    *gpioregs = (unsigned int *)mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd, gpio_addr);
    if (*gpioregs == MAP_FAILED)		// check for errors
    {
        close (fd);		// close memory before exiting
        exit(1);        // Returns 1 to the operating system;
    }
    // success
    return 0;
}

/* @brief the busyWait function
   @param delayCount number of cycles to wait */
static inline void busyWait(unsigned long delayCount)
{
  //printf("%ld\n", delayCount);
  while (delayCount--)
  {
    // nothing here!
  }
}

/* @brief Handle signals
   @param num Signal Number */
static void signal_handler(int num)
{
  stopLoop = 1;
  //int x = system("echo 1 > /sys/class/pulsecap/pulsecap1/control/cancel");
  //printf("\n%d: Stopping loop!\n", stopLoop);
}

void setupGPIO()
{
  // try to setup LEDs
  if (map_gpio(&gpioregs, LED_GPIO_ADDR))
  {
    printf("failed to setup!");
    // failed to setup
    //return 1;
  }

  // reset control register
  *REG_OFFSET(gpioregs, 0) = 0x00;
}

/*Close all the GPIO by unmapping the LEDs*/
void closeGPIO()
{
  if (munmap ((void *)LED_GPIO_ADDR, 0x10000) != 0)
	{
      printf("ERROR: munmap() failed...\n");
      exit(1);
	}
      close (fd); 	// close memory
}

/*Output the given char to the LEDs*/
void gpio_out(unsigned char state)
{
  *REG_OFFSET(gpioregs, 0) = state; // turn on
}


int main(void)
{
  //Setup the GPIO
  setupGPIO();

  // handle SIGINT (ctrl-c)
  signal(SIGINT, signal_handler);

  // initialize peripherals here, use libzed, lookup .h files in
  // /usr/share/EECE45343/include/libzed

  //Start pulsecap module
  int x = system("modprobe pulsecap");

  // get max number of edges
  x = system("cat /sys/class/pulsecap/pulsecap1/control/max_edges");

  // get current state
  x = system("cat /sys/class/pulsecap/pulsecap1/control/state");

  // start recording 20 edges
  x = system("echo 1000 > /sys/class/pulsecap/pulsecap1/control/capture");

  while (!stopLoop)
  {

    // software PWM
    gpio_out(1);                   // high
    //busyWait(duty_cycle);          // high for duration of duty cycle
    usleep(duty_cycle);
    
    gpio_out(0);                   // low
    usleep(period-duty_cycle);
    //busyWait(period - duty_cycle); // low for the rest of the period
  }

  printf("\nFinishing up....\n");

  // check how many edges have been recorded already
  x = system("cat /sys/class/pulsecap/pulsecap1/control/capture");

  // copy data into file
  x = system("cat /sys/class/pulsecap/pulsecap1/control/edge_data > data");

  //Stop pulsecap system
  x = system("echo 1 > /sys/class/pulsecap/pulsecap1/control/cancel");

  // de-initialize peripherals
  gpio_out(0);
  closeGPIO();


  return 0;
}
