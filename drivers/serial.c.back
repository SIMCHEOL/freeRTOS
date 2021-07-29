#include "includes.h"

/* UART Base Address determined by Hypervisor's Stage 2 Translation Table */
#define TEGRA_CLK_RESET_BASE        0x60006000

#define TEGRA_UARTD_BASE        0x70006300
#define TEGRA_UARTD_SIZE        0x100


static const struct {
	unsigned int base;
	unsigned int reset_reg;
	unsigned int clock_reg;
	unsigned int bit;
} uarts = {
	TEGRA_UARTD_BASE,
	TEGRA_CLK_RESET_BASE + 0x0c,
	TEGRA_CLK_RESET_BASE + 0x18,
	1,
};

//int uart_id = 3;
volatile unsigned char *uart;

#define DEBUG_UART_SHIFT    2
#define UART_LSR_THRE       0x20 /* Transmit-hold-register empty */
#define UART_LSR            5    /* In:  Line Status Register    */
#define UART_RX             0    /* In:  Receive buffer          */
#define UART_TX             0    /* Out: Transmit buffer         */


/* Exynos 5250 UART register macros */
#define UTXH        0x20
#define UFSTAT      0x18
#define UART_BASE   (UART0 + UTXH)

#define RX_FIFO_COUNT_MASK  0xff
#define RX_FIFO_FULL_MASK   (1 << 8)
#define TX_FIFO_FULL_MASK   (1 << 24)

#define readl(a)         (*(volatile unsigned int *)(a))
#define readb(a)         (*(volatile unsigned char *)(a))
#define writeb(v, a)         (*(volatile unsigned char *)(a) = (v))


static int serial_err_check(int op)
{
	/*
    struct s5p_uart *const uart = (struct s5p_uart *) UART0;
    struct s5p_uart *const uart = (struct s5p_uart *) UART0;
    unsigned int mask;

    if(op)
        mask = 0x8;
    else
        mask = 0xf;

    return readl(&uart->uerstat) & mask;
	*/
	return 0;
}

static inline int s5p_uart_divslot(void)
{
    return 0;
}


void uart_tx_char(char c)
{
	//modified
	uart = (volatile unsigned char *)uarts.base;
    while (!(uart[UART_LSR << DEBUG_UART_SHIFT] & UART_LSR_THRE))
	;
	uart[UART_TX << DEBUG_UART_SHIFT] = c;
	if (c == '\n')
		uart[UART_TX << DEBUG_UART_SHIFT] = '\r';
}

void uart_out_char_check_CR(const char c)
{
    uart_tx_char(c);
    if (c == '\n')
        uart_tx_char('\r');
}

int uart_out_str(char *str, int size)
{
    int lp;

    for (lp = 0; lp < size; lp++)
        uart_tx_char(str[lp]);

    return lp;
}

int uart_out_str_check_CR(char *str, int size)
{
    int lp;

    for (lp = 0; lp < size; lp++)
        uart_out_char_check_CR(str[lp]);

    return lp;
}

/*
int serial_getc_dev(void)
{
    struct s5p_uart * const uart = (struct s5p_uart *) UART0;

    // wait for character to arrive 
    while (!(readl(&uart->ufstat) & (RX_FIFO_COUNT_MASK | RX_FIFO_FULL_MASK))) {
        if (serial_err_check(0))
            return 0;
    }

    return (int) (readb(&uart->urxh) & 0xff);
}
*/

char uart_rx_char()
{
    return 0;
}

void uart_init()
{
    return;
}


