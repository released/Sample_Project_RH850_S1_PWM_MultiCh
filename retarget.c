/*_____ I N C L U D E S ____________________________________________________*/
#include "retarget.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

//retarget printf
int putchar(int c)
{
    #if 1
    // if (c == '\r') 
    // {        
    //     SendChar('\n');
    // }
    SendChar((char)c);
    return (char)c;
    #else
    DISABLE_UART_INT_STATUS_FLAG = 1U;    /* disable INTST0 interrupt */
    UART_XFER = (unsigned char)c;
    while(CLEAR_UART_INT_STATUS_FLAG == 0)
    {

    }
    CLEAR_UART_INT_STATUS_FLAG = 0U;    /* clear INTST0 interrupt flag */
    return c;
    #endif
}

#if 0   // linux
/*
	https://github.com/torvalds/linux/blob/master/lib/string.c
	https://github.com/torvalds/linux/blob/master/arch/x86/boot/printf.c
*/

size_t strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

int fputc(int ch, FILE *f) 
{
    (void)f;
    SendChar(ch);
    return ch;
}

int fputs(const char *str, FILE *fp) 
{
    while (*str) 
    {                               // Continue while the current character is not '\0'
        fputc(*str, fp);            // Write the current character to the file
        str++;                      // Move the pointer to the next character
        return *str;
    }
    return EOF;
}

int puts(const char *s)
{
	if (fputs(s, stdout) == EOF)
		return EOF;
	return putchar('\n');
}

static int isdigit(int ch)
{
	return (ch >= '0') && (ch <= '9');
}

static int skip_atoi(const char **s)
{
	int i = 0;

	while (isdigit(**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* Must be 32 == 0x20 */
#define SPECIAL	64		/* 0x */

// #define __do_div(n, base)						\
// ({												\
// 	int __res;									\
// 												\
// 	__res = ((uint64_t) n) % (uint32_t) base;	\
// 	n = ((uint64_t) n) / (uint32_t) base;		\
// 	__res;										\
// })

int __do_div(long n,int base)
{
	int __res;
	__res = ((uint64_t) n) % (uint32_t) base;
	n = ((uint64_t) n) / (uint32_t) base;
	return __res;
}

static char *number(char *str, long num, int base, int size, int precision,
		    int type)
{
	/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
	static const char digits[16] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */

	char tmp[66];
	char c, sign, locase;
	int i;

	/* locase = 0 or 0x20. ORing digits or letters with 'locase'
	 * produces same digits or (maybe lowercased) letters */
	locase = (type & SMALL);
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 16)
		return NULL;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	else
		while (num != 0)
			tmp[i++] = (digits[__do_div(num, base)] | locase);
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type & (ZEROPAD + LEFT)))
		while (size-- > 0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type & SPECIAL) {
		if (base == 8)
			*str++ = '0';
		else if (base == 16) {
			*str++ = '0';
			*str++ = ('X' | locase);
		}
	}
	if (!(type & LEFT))
		while (size-- > 0)
			*str++ = c;
	while (i < precision--)
		*str++ = '0';
	while (i-- > 0)
		*str++ = tmp[i];
	while (size-- > 0)
		*str++ = ' ';
	return str;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	char *str;
	const char *s;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str = buf; *fmt; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}

		/* process flags */
		flags = 0;
	      repeat:
		++fmt;		/* this also skips first '%' */
		switch (*fmt) {
		case '-':
			flags |= LEFT;
			goto repeat;
		case '+':
			flags |= PLUS;
			goto repeat;
		case ' ':
			flags |= SPACE;
			goto repeat;
		case '#':
			flags |= SPECIAL;
			goto repeat;
		case '0':
			flags |= ZEROPAD;
			goto repeat;
		}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char)va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			continue;

		case 's':
			s = va_arg(args, char *);
			len = strnlen(s, precision);

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			continue;

		case 'p':
			if (field_width == -1) {
				field_width = 2 * sizeof(void *);
				flags |= ZEROPAD;
			}
			str = number(str,
				     (unsigned long)va_arg(args, void *), 16,
				     field_width, precision, flags);
			continue;

		case 'n':
			if (qualifier == 'l') {
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			} else {
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			continue;

		case '%':
			*str++ = '%';
			continue;

			/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'x':
			flags |= SMALL;
			// fallthrough;
		case 'X':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
			break;

		case 'u':
			break;

		default:
			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}
		if (qualifier == 'l')
			num = va_arg(args, unsigned long);
		else if (qualifier == 'h') {
			num = (unsigned short)va_arg(args, int);
			if (flags & SIGN)
				num = (short)num;
		} else if (flags & SIGN)
			num = va_arg(args, int);
		else
			num = va_arg(args, unsigned int);
		str = number(str, num, base, field_width, precision, flags);
	}
	*str = '\0';
	return str - buf;
}

int sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsprintf(buf, fmt, args);
	va_end(args);
	return i;
}

int tiny_printf(const char *fmt, ...)
{
	char printf_buf[1024];
	va_list args;
	int printed;

	va_start(args, fmt);
	printed = vsprintf(printf_buf, fmt, args);
	va_end(args);

	puts(printf_buf);

	return printed;
}


#else

// Print unsigned number with given base, width, padding, alignment
static void mini_print_unsigned(unsigned int num, unsigned int base, int width, int left_align, char fill, int uppercase)
{
    char buf[16];
    int i = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    // Convert number to string (reversed)
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num);

    int pad = width - i;
    if (pad < 0) pad = 0;

    if (!left_align) {
        while (pad--) putchar(fill);
    }

    while (i--) {
        putchar(buf[i]);
    }

    if (left_align) {
        while (pad--) putchar(' ');
    }
}

// Print signed number with width, padding, alignment
static void mini_print_signed(int num, int width, int left_align, char fill)
{
    if (num < 0) {
        putchar('-');
        num = -num;
        width--;
    }
    mini_print_unsigned((unsigned int)num, 10, width, left_align, fill, 0);
}

// Print float number (only 2 decimal places)
static void mini_print_float(double f, int width, int left_align, char fill)
{
    if (f < 0) {
        putchar('-');
        f = -f;
        width--;
    }

    int int_part = (int)f;
    int frac_part = (int)((f - int_part) * 100 + 0.5); // 2 decimal places

    // Adjust width (e.g., for ".xx")
    width -= 3;
    int tmp = int_part;
    while (tmp) {
        width--;
        tmp /= 10;
    }

    if (!left_align) {
        while (width-- > 0) putchar(fill);
    }

    // Print integer part
    mini_print_unsigned(int_part, 10, 0, 0, fill, 0);
    putchar('.');
    // Print fractional part
    if (frac_part < 10) {
        putchar('0'); // leading zero
    }
    mini_print_unsigned(frac_part, 10, 0, 0, fill, 0);

    if (left_align) {
        while (width-- > 0) putchar(' ');
    }
}

int tiny_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	int printed = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            int left_align = 0;
            char fill = ' ';
            int width = 0;
            // int uppercase = 0;

            // Check for left align
            if (*fmt == '-') {
                left_align = 1;
                fmt++;
            }

            // Check for zero padding
            if (*fmt == '0') {
                fill = '0';
                fmt++;
            }

            // Parse width
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }

            // Format specifiers
            switch (*fmt) {
                case 'd':
                    mini_print_signed(va_arg(args, int), width, left_align, fill);
                    break;
                case 'u':
                    mini_print_unsigned(va_arg(args, unsigned int), 10, width, left_align, fill, 0);
                    break;
                case 'x':
                    mini_print_unsigned(va_arg(args, unsigned int), 16, width, left_align, fill, 0);
                    break;
                case 'X':
                    mini_print_unsigned(va_arg(args, unsigned int), 16, width, left_align, fill, 1);
                    break;
                case 'c':
                    putchar((char)va_arg(args, int));
                    break;
                case 's': {
                    const char *str = va_arg(args, const char *);
                    int len = 0;
                    const char *p = str;
                    while (*p++) len++;

                    int pad = width - len;
                    if (pad < 0) pad = 0;

                    if (!left_align) {
                        while (pad--) putchar(fill);
                    }

                    while (*str) {
                        putchar(*str++);
                    }

                    if (left_align) {
                        while (pad--) putchar(' ');
                    }
                    break;
                }
                case 'f':
                    mini_print_float(va_arg(args, double), width, left_align, fill);
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    putchar('?');
                    break;
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);

	return printed;    
}

#endif
