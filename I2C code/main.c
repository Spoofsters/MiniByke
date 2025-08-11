#include "twi.h"

#define BUFFER buffer_stuff.buffer_info.buffer
#define BUFFER_SIZE buffer_stuff.buffer_info.buffer_size

 static struct ISR_TO_FUNCTION{ 
    twi_message_t buffer_info;
};
struct ISR_TO_FUNCTION buffer_stuff;

int main () {
DDRC &= ~(1 << 4); // Part 1
DDRC &= ~(1 << 5);

  // Part 7

  twi_init(100000); // Seeting the frequency to 100k HZ or 100 KHZ. HZ = hertz, KHZ = kiloherts

  twi_message_t message[2] = {
    BUFFER[0] = (0x68 << 1) | 0, // Device address
    BUFFER_SIZE = 2,

    BUFFER[1] = (0x75), // Register Adress (WHO AM I)
  };
  {
    BUFFER[0] = (0x68 << 1) | 1,
    BUFFER_SIZE = 2,

    BUFFER[1] = 0;



  };

    twi_enqueue(&message[1], 2); // The '&' might ruin my code
    twi_enqueue(&message[2], 2); // The '&' might ruin my code
}
 


  


