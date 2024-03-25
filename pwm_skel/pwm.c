#include <sys/signal.h>
#include <libzed/axi_gpio.h>
#include <libzed/zed_common.h>

/// stop infinite loop flag
volatile static unsigned char stopLoop = 0;

/* @brief the busyWait function
   @param delayCount number of cycles to wait */
static inline void busyWait(unsigned long delayCount) {
  while(delayCount--) {
    // nothing here!
  }
}

/* @brief Handle signals
   @param num Signal Number */
static void signal_handler(int num)
{
  stopLoop = 1;
}

int main(void)
{
  struct axi_gpio_data* leds;

  // handle SIGINT (ctrl-c)
  signal(SIGINT, signal_handler);

  // initialize peripherals here, use libzed, lookup .h files in
  // /usr/share/EECE45343/include/libzed

  while (!stopLoop)
    {

      //software PWM

    }

  //de-initialize peripherals here

  return 0;
}
