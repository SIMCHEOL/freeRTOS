#include <io-tk1.h>

#define CON_LOCK(x)				( ( 0x1 << ( x ) ) << 8 )
#define CON_MASK_ENABLE(x)		( ( 0x1 << ( x ) ) << 8 )
#define CON_GPIO(x)				( ( 0x1 ) << ( x ) )
#define OE_DRIVEN(x,y) 			( ( x ) << ( y ) )
#define OE_MASK(x)				( ( 0x1 << ( x ) ) << 8 )
#define OUTPUT(x,y)				( ( x ) << ( y ) )
#define OUTPUT_MASK(x)			( ( 0x1 << ( x ) ) )
#define OUTPUT_CNF_MASK(x)		( ( 0x1 << ( x ) ) << 8 )
#define OUTPUT_CNF(x,y)			( ( x ) << ( y ) )
#define OUTPUT_SET(x)			( ( 0x1 << ( x ) ) )
#define INPUT_MASK(x)			( ( 0x1 << ( x ) ) )

void gpio_cfg_pin(struct gpio_bank *bank, int port, int pin, int cfg)
{
	unsigned int value;
	bank->cnf[port] &= ~CON_LOCK(pin);
	bank->cnf[port] |= CON_GPIO(pin);
	/*
	value &= ~CON_LOCK(pin);
	value |= CON_GPIO(pin);
	writegpio(&bank->cnf[port], value);
	*/

	value = readgpio(&bank->oe[port]);
	value |= OE_DRIVEN(cfg, pin);
	writegpio(&bank->oe[port], value);

	gpio_set_value(bank, port, pin, LOW);
}

void gpio_msk_pin(struct gpio_bank *bank, int port, int pin, int cfg)
{
	unsigned int value;
	value = readgpio(&bank->msk_cnf[port]);
	value |= CON_MASK_ENABLE(pin);
	value |= CON_GPIO(pin);
	writegpio(&bank->msk_cnf[port], value);

	value = readgpio(&bank->msk_oe[port]);
	value |= CON_MASK_ENABLE(pin);
	value |= CON_GPIO(pin);
	writegpio(&bank->msk_oe[port], value);

	value = readgpio(&bank->msk_out[port]);
	value |= CON_MASK_ENABLE(pin);
	//value &= ~CON_MASK_ENABLE(pin);
	value |= CON_GPIO(pin);
	writegpio(&bank->msk_out[port], value);
}

void gpio_set_value(struct gpio_bank *bank, int port, int pin, int en)
{
	unsigned int value;
	value = readgpio(&bank->out[port]);
	value &= ~OUTPUT_MASK(pin);
	if(en)
		value |= OUTPUT_SET(pin);
	writegpio(&bank->out[port], value);
}

unsigned int gpio_get_value(struct gpio_bank *bank, int port, int pin)
{
	unsigned int result = 0;

	result = readgpio(&bank->in[port]);
	result &= INPUT_MASK(pin);
	return !!result;
}

void qph_gpio_set_value(struct gpio_bank *bank, int port, int pin, int en)
{
	unsigned int value;
	bank->cnf[port] &= ~CON_LOCK(pin);
	bank->cnf[port] |= CON_GPIO(pin);
	bank->oe[port] |= OE_DRIVEN(GPIO_OUTPUT, pin);

	value = readgpio(&bank->out[port]);
	value &= ~OUTPUT_MASK(pin);
	if(en)
		value |= OUTPUT_SET(pin);
	writegpio(&bank->out[port], value);
}
