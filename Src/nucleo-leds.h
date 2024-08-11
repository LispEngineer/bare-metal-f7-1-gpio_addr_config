

// Pin numbers in a bank
#define GREEN_PIN_B 0
#define BLUE_PIN_B  7
#define RED_PIN_B   14


// Mode registers
#define USER_LED1_MODER (1U << (GREEN_PIN_B * 2)) // Output
#define USER_LED2_MODER (1U << ( BLUE_PIN_B * 2))
#define USER_LED3_MODER (1U << (  RED_PIN_B * 2))


// Output registers
#define USER_LED1       (1U << GREEN_PIN_B)
#define USER_LED2       (1U <<  BLUE_PIN_B)
#define USER_LED3       (1U <<   RED_PIN_B)

