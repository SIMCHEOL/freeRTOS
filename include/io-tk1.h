#define GPIO_BASE 0x6000D000

#define GPIO_CONTROLLER_1 ( GPIO_BASE + 0x000 )
#define GPIO_CONTROLLER_2 ( GPIO_BASE + 0x100 )
#define GPIO_CONTROLLER_3 ( GPIO_BASE + 0x200 )
#define GPIO_CONTROLLER_4 ( GPIO_BASE + 0x300 )
#define GPIO_CONTROLLER_5 ( GPIO_BASE + 0x400 )
#define GPIO_CONTROLLER_6 ( GPIO_BASE + 0x500 )
#define GPIO_CONTROLLER_7 ( GPIO_BASE + 0x600 )
#define GPIO_CONTROLLER_8 ( GPIO_BASE + 0x700 )

#define writegpio(a, v) ( *( volatile unsigned int * )(a) = (v) )
#define readgpio(a)		( *( volatile unsigned int * )(a) )

enum port_1 {PA, PB, PC, PD};
enum port_2 {PE, PF, PG, PH};
enum port_3 {PI, PJ, PK, PL};
enum port_4 {PM, PN, PO, PP};
enum port_5 {PQ, PR, PS, PT};
enum port_6 {PU, PV, PW, PX};
enum port_7 {PY, PZ, PAA, PBB};
enum port_8 {PCC, PDD, PEE, PFF};

struct gpio_bank {
	//Lower offsets
	unsigned int    cnf[4];
	unsigned int    oe[4];
	unsigned int    out[4];
	unsigned int    in[4];
	unsigned int    int_sta[4];
	unsigned int    int_enb[4];
	unsigned int	int_lvl[4];
	unsigned int	int_clr[4];
	//Upper offsets
	unsigned int    msk_cnf[4];
	unsigned int    msk_oe[4];
	unsigned int    msk_out[4];
	unsigned int    res1[4];
	unsigned int    msk_int_sta[4];
	unsigned int    msk_int_enb[4];
	unsigned int	msk_int_lvl[4];
	unsigned int    res2[4];
};


// Function
void gpio_cfg_pin(struct gpio_bank *bank, int port, int pin, int cfg);
void gpio_msk_pin(struct gpio_bank *bank, int port, int pin, int cfg);
void gpio_set_value(struct gpio_bank *bank, int port, int pin, int en);
void qph_gpio_set_value(struct gpio_bank *bank, int port, int pin, int en);
unsigned int gpio_get_value(struct gpio_bank *bank, int port, int pin);

// GPIO option
#define GPIO_INPUT 0x0
#define GPIO_OUTPUT 0x1
#define LOW 0x0
#define HIGH 0x1
/*
#define PIN_0 0x1
#define PIN_1 0x2
#define PIN_2 0x4
#define PIN_3 0x8
#define PIN_4 0x10
#define PIN_5 0x20
#define PIN_6 0x40
#define PIN_7 0x80
*/

