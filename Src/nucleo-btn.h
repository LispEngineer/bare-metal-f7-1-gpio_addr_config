
// Blue button is a user input
// Per Nucleo user guide, it's PC13 (UM1974 Rev 10 p24 sec 6.6)
// Per Datasheet, this is also on AHB1
// Per RM, we can set the GPIOC
// IDR - input data register - lower 16 bits

// Pin numbers in a bank
#define USER_BTN_B  13

// Mode registers
#define USER_BTN_MODER  (~(3U << (USER_BTN_B * 2))) // Input

// Input registers
#define USER_BTN        (1U << USER_BTN_B)
