#include "twi.h"

struct ISR_TO_FUNC buffer_stuff; // Calling the struct and naming it that way I can call it later

#define F_CPU 16000000ULL     // Define the F_CPU variable since for some reason VSC can't read the make file
volatile uint8_t *END_BUFFER; // Making a variable representing the last message in the buffer
bool RUFINISHED = false;      // Making a variable used to see if the enqueue function is running or not

#define BUFFER buffer_stuff.buffer_info.buffer           // Calling the buffer itself and assigning it to a define macro for ease of use
#define BUFFER_SIZE buffer_stuff.buffer_info.buffer_size // doing the same thing but for the buffer count respectively

twi_status_t twi_enqueue(twi_message_t *messages, size_t count)
{ // Adding information to the messages buffer whenever the user wants to add something. In this case it'd be firing the gun with the intent to create a bullet hole.

  if (!(TWCR & (1 << TWINT)))
  { // While TWINT = 0, in other words, no data is being transmitted, run this while loop. The moment TWINT becomes 1, this becomes false and data is being transmitted.

    // REVIEW THE ATmega328P DATA SHEET IF YOU'RE CONFUSED. SPECIFICALLY PAGE 185 FOR INDEPTH EXPLAINATION
    // 10100101 Is the current control register in this state

    // How do I return the message back to the caller?
    // How do I ensure that I don't add anything to the que without disturbing any ongoing data transfers?
    // And how do I ensure that in the event that I do fail, it isn't a catastrophic failure? -- ERROR CODE STUFF

    BUFFER = messages;                   // Setting the buffer equal to the messages that way it gets added to the que
    BUFFER_SIZE = count;                 // Doing the same but for the count. This is also going to be used to make sure the user's messages are valid
    END_BUFFER = BUFFER + BUFFER_SIZE;   // Retrieving the last element of the message array
    TWCR |= (1 << TWINT) | (1 << TWSTA); // Makes interupts ready to be used as well as the start bit enabler. Also pulls the trigger and sends the code strait to the ISR

    RUFINISHED = true;
    return (twi_status_success);
    // I'm chilling
  }
  else
  {
    return twi_status_busy;
    // I'm busy
  }
}
twi_status_t twi_status(void)
{
  if ((TWCR & (1 << TWINT)) && RUFINISHED == true)
  { // Checks if the enqueue function is not runing
    return (twi_status_success);
  }
  return (twi_status_failure);
}
twi_status_t twi_cancel(void)
{
  TWCR |= (1 << TWINT) | (1 << TWSTA);
  return (twi_status_success);
}
twi_status_t twi_deinit(void)
{
  if ((TWCR & (1 << TWINT)) && RUFINISHED == false)
  {                                                     // If enqueue is running when it shouldn't then run this
    TWCR &= ~((1 << TWIE) | (1 << TWEA) | (1 << TWEN)); // Makes all of them Zero
    return (twi_status_success);
  }
  return (twi_status_failure); // Otherwise, return failure
}

static bool scl_parameters(const uint32_t set_frequency, uint8_t *const twbr, uint8_t *const twps)
{ // Part 4

  // SEE DATA SHEET IF CONFUSED JAVIER. SPECIFICALLY PAGE 200 AND AROUND FOR DETAILED EXPLAINATIONS.

  uint8_t prescaler = 1;                   // You set the initial clock prescaler to 1 so that both prescaling(twps) bit values are equal to 0
  uint16_t twbr_checker;                   // You create this variable so that you can store the calculated value of the bit rate before actually setting it to the real bit rate.
  *twps &= ~((1 << TWPS1) | (1 << TWPS0)); // Sets both starus register bits to be 0 durring initialization

  for (uint8_t i = 0; i < 4; i++)
  { // Looping between values of i going through 0, 1, 2, and 3 that are representative of 4 iterations

    twbr_checker = ((F_CPU / set_frequency) - 16) / (2 * prescaler); // The behind the scene calculation that solves for the bit rate given all other variables.

    if (twbr_checker <= 255)
    {                       // Checks if the calculated bit rate can fit within the 8-bit twbr variable
      *twbr = twbr_checker; // Setting the calculated bit rate to the real bit rate if true
      *twps += i;           // Adding the current iteration of i to the prescaling bits. You do this so that the prescaling value adjusts to the bit rate.
      return true;          // Returning true so that you know it worked correctly and make the code break from the function
    }
    prescaler = prescaler << 2; // Bitshifting the prescaler by 2 to turn the 1 into a 4 and so on. This is how the prescaling value adapts after each iteration.
  } // The way it works: 001 = 1, 100 = 4, 100 00 = 16, 100 00 00 = 64. After every bitshift of 2, you increase 4 times the amount.
  return false; // Returning false in the event that somthing goes horribly wrong and nothing ends up working. It makes it easier to debug.
}

// Part 5

ISR(TWI_vect)
{ // Using the interupt service routine(ISR) to call an interupt using cases from the Master/Slave calls

  uint8_t randoVar = 67;

  switch (TW_STATUS)
  { // A giant switch case using all of the cases the TWSR in my driver will ever use
  case TW_START:
  {
    // You can't really reset the buffer pointer because the buffer itself is already a pointer and cannot be reseted. This isn't detrimental to the driver's succcess.
  }
  case TW_REP_START:
  {

    break;
  }
  case TW_MT_SLA_ACK:
  {
  }
  case TW_MT_DATA_ACK:
  {
    if (BUFFER == END_BUFFER)
    {                                      // Checks if the message is at it's last element
      TWCR |= (1 << TWINT) | (1 << TWSTO); // Writes a logic 1 to the TWINT flag, effectively reseting it. Writes a logic 1 to the TWSTO bit, ending transmition
    }
    else
    {
      TWDR = *BUFFER;       // Setting the data of the message equal to the data register after the master has recieved an ack from the slave
      BUFFER++;             // Incrmenting the buffer to go to the next message
      TWCR |= (1 << TWINT); // You know what this does
    }
    break; // Ends the interupt for this line, notice how the case's above didn't have any breaks. This is because all of those cases check the same thing Javier, and it is represented with a fall through into the next line
  }

  case TW_MR_DATA_ACK:
  {

    *BUFFER = TWDR; // Setting current buffer position equal to current message sent into the TWDR
    BUFFER++;
  }
  case TW_MR_SLA_ACK:
  {
    if (BUFFER < END_BUFFER)
    {                       // Check if the current buffer byte position is before end of message
      TWCR &= ~(1 << TWEA); // Setting the NACK bit
      TWCR |= (1 << TWINT);
    }
    else
    {
      TWCR |= (1 << TWEA);
      TWCR |= (1 << TWINT); // Setting the ACK bit
    }
    break;
  }

  case TW_MR_DATA_NACK:
  {
    *BUFFER = TWDR;         // Setting the position of the buffer equal to the position of the variable
    TWCR |= (1 << TWINT)    // Disabling TWI flag
            | (1 << TWSTO); // Transmitting STOP
    break;
  }

    // Is this stuff that I don't need? NO ERROR STATES HAVE BEEN INTRODUCED

  case TW_MT_SLA_NACK:
  {
    break;
  }
  case TW_MR_SLA_NACK:
  {
    break;
  }
  case TW_MT_DATA_NACK:
  {
    break;
  }
  case TW_ST_SLA_ACK:
  {
    break;
  }
  case TW_ST_LAST_DATA:
  {
    break;
  }
  case TW_SR_SLA_ACK:
  {
    break;
  }
  case TW_SR_DATA_ACK:
  {
    break;
  }
  case TW_SR_DATA_NACK:
  {
    break;
  }
  case TW_SR_STOP:
  {
    break;
  }
  case TW_NO_INFO:
  {
    break;
  }
  case TW_BUS_ERROR:
  {
    break;
  }
  }
}

// TWI translation unit.    Also part 7
twi_status_t twi_init(const uint32_t set_frequency)
{                                              // Creating an initializing function with the twi_status_t data type. All of the code in this function will run immediately, essentially loading the amunition and priming the trigger
  scl_parameters(set_frequency, &TWBR, &TWSR); // Calling the scl_parameters function and passing in a hardcoded 100 KHZ frequency as well as the addresses of both TWBR and TWSR so that the pointer variables in the aforemention function work as intended.
  TWCR |= (1 << TWEN);                         // Enables TWI
  TWCR |= (1 << TWIE);                         // Enables interrupts
  sei();                                       // Used to enable global interrupts
  return twi_status_success;                   // returning success upon a successful initialization
}
