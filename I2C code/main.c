#include "twi.h"

int main () {
DDRC &= ~(1 << 4); // Part 1
DDRC &= ~(1 << 5);

PORTC |= (1 << 4);
PORTC |= (1 << 5);

uint8_t buffer1[2]; // creating an empty buffer array cocntaining 2 elements
uint8_t buffer2[2]; // same thing but another time


  // Part 7

  twi_init(100000); // Seeting the frequency to 100k HZ or 100 KHZ. HZ = hertz, KHZ = kiloherts

  twi_message_t message[2]; // Creating an array using the twi_message_t datatype. This is crucial because it allows you to then access the buffer member of that same data type.
  
  message[0].buffer = buffer1; // setting message equal to the empty array to assign it a valid address in memory
  message[1].buffer = buffer2; // that way, you don't get any memory corruption from lines 25 onwards.
  
  message[0].buffer[0] = (0x68 << 1) | 0; // Device address Notice how the buffer is also indexed into. This is because the buffer is a pointer and this reflects the active change going on inside of it. (Everything inside of the buffer will ALWAYS be an address)
  message[0].buffer[1] = 0x75; // With this principal in mind, we can index to the second message through the buffer array(pointer)
  message[0].buffer_size = 2; // The size of the buffer is equal to 2 because you send the register and device adress to the ISR
  
  message[1].buffer[0] = (0x68 << 1) | 1;  // We bitshift by 1 because it gauruntee's that the far-most right bit is 0, allowing us to dictate weather it will be 1 or 0.
  message[1].buffer[1] = 0; // You do this so that when the Master revieves data, the buffer is allotted 1 free space in the form of zero, for future messages.
  message[1].buffer_size = 2; // You set the buffer size equal to 2 because the size of the buffer is also 2

    while (twi_enqueue(&message[0], message[0].buffer_size) == twi_status_busy){ // I constantly check if the enqueue function is busy
      twi_enqueue(&message[0], message[0].buffer_size); // If it is, keep on running it anways until it isn't and is instead a success
      if (twi_enqueue(&message[0], message[0].buffer_size) == twi_status_success){ // when it is an eventual success, break out from everything.
        break;
      }
    }

        while (twi_enqueue(&message[1], message[1].buffer_size) == twi_status_busy){ 
      twi_enqueue(&message[1], message[1].buffer_size);
      if (twi_enqueue(&message[1], message[1].buffer_size) == twi_status_success){ 
      }
    }
}
 
// The idea behind lines 19-30 is to make an array of messages under the datatype twi_message_t and the messages contain  

// An initializer list is when you declare something and give it values in the same line of code


// 0 = WRITE, 1 == READ


