#define bitRead(value, bit)                 (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)                  ((value) |= (1UL << (bit)))
#define bitClear(value, bit)                ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)      ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))
#define bit(Bit)                            (0x01 << (Bit))
#define lowByte(value) ((unsigned char)     ((value) & 0xff))
#define highByte(value) ((unsigned char)    ((value) >> 8))

#define byte(value)                         ((byte)value)
#define char(value)                         ((char)value)
#define float(value)                        ((float)value)
#define int(value)                          ((int)value)
#define long(value)                         ((long)value)
#define word(value)                         ((short)value)

typedef unsigned char   byte;

#ifndef CY_GPIO_INTR_LOW
#define CY_GPIO_INTR_LOW    4
#endif

#define HIGH            1
#define LOW             0

#define LSBFIRST        0
#define MSBFIRST        1

#define INPUT           CY_GPIO_DM_HIGHZ    // 1
#define OUTPUT          CY_GPIO_DM_STRONG   // 6
#define INPUT_PULLUP    CY_GPIO_DM_PULLUP   // 2


#define CHANGE          CY_GPIO_INTR_BOTH       // 3
#define RISING          CY_GPIO_INTR_RISING     // 1
#define FALLING         CY_GPIO_INTR_FALLING    // 2

void        pinMode(uint8_t pin, uint8_t mode);
void        digitalWrite(uint8_t pin, uint8_t value);
uint8_t     digitalRead(uint8_t pin);

uint8_t     shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void        shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

void        attachInterrupt(uint8_t pin, void (*userFunc)(void), int mode);
void        detachInterrupt(uint8_t pin);

void        interrupts(void);
void        noInterrupts(void);

void        delay(unsigned int ms);
void        delayMicroseconds(unsigned int us);

void        tone(uint8_t pin, unsigned int grequency);
void        noTone(uint8_t pin);

uint16_t    analogRead(uint8_t pin);
void        analogWrite(uint8_t pin, uint8_t value);

uint32_t    micros();
uint32_t    mills();
