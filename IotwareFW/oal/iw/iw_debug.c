/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/
/*
 * History
 *
 */
#include <stdarg.h>
#include "iw_common.h"
#include "iw_bsp.h"
#include "iw_dev.h"
#include "iw_if.h"

//#define DEBUG_UART_PORT		WDK_UART_PORT1

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define to_upper(c) ((c) & ~0x20)

#if (DEBUG_OFF == 0)
/*
 * print character	
 */
static int prt8(unsigned num, char *str)
{
	int len;
	char temp[20];
	char *p = temp;

	*p = 0;
	len = 0;
	do
	{
		*++p = (num & 7) + '0';
		num = num >> 3;
		len++;
	} while (num);

	do
	{
		*str++ = *p;
	} while (*p--);

	return len;
}

/*
 * print decimal
 */
static int prt10(unsigned int num, char *str)
{
	int len;
	char temp[20];
	char *p = temp;

	*p = 0;
	len = 0;
	do
	{
		*++p = num % 10 + '0';
		num /= 10;
		len++;
	} while (num);

	do
	{
		*str++ = *p;
	} while (*p--);

	return len;
}

/*
 * print hexa-decimal, lower case
 */
static int prt16(unsigned num, char *str)
{
	int len;
	char temp[20];
	char *p = temp;

	*p = 0;
	len = 0;
	do
	{
		*++p = "0123456789abcdef"[num & 0x0f];
		num = num >> 4;
		len++;
	} while (num);

	do
	{
		*str++ = *p;
	} while (*p--);

	return len;
}

/*
 * print hexa-decimal, upper case
 */
static int PRT16(unsigned num, char *str)
{
	int len;
	char temp[20];
	char *p = temp;

	*p = 0;
	len = 0;
	do
	{
		*++p = "0123456789ABCDEF"[num & 0x0f];
		num = num >> 4;
		len++;
	} while (num);

	do
	{
		*str++ = *p;
	} while (*p--);

	return len;
}

#if 0

#include <stdarg.h>

int _dputc(int ch)
{
	static int old_ch = -1;

	if( ch == '\n' && old_ch != '\r' )
	{
		wdk_uart_putc( DEBUG_UART_PORT, '\r' );
	}

	wdk_uart_putc( DEBUG_UART_PORT, (old_ch = ch) );

	return 0;
}

int _dgetc(int *pch)
{
	if( wdk_uart_getc( DEBUG_UART_PORT, pch ) < 0 )
	{
		return -1;
	}

	return 0;
}

void wdk_printf(char *format, ...)
{
	int		c, i, f;
	int		length, leftjust, leading;
	long	fmax, fmin;
	int		sign, fill;
	char	*str;
	char	string[64];

	va_list ap;
	int *args;
	
	va_start(ap, format);

	args = va_arg(ap, int *);

	for(;;)
	{
		/* Echo character until '%' or end of fmt string */
		while ((c = *format++) != '%' )
		{
			if (!c)
				return; //return (-1);
			_dputc(c);
		}

		/* Echo " ... %%..." as '%'  */
		if (*format == '%')
		{
			_dputc((*format++ & 0xff));
			continue;
		}

		/* Check for "%-...",== Left-justified output */
		if ((leftjust = (*format == '-')) != 0)
		{
			format++;
		}

		/* Allow for zero-filled numeric outputs ("%0...") */
		fill = (*format =='0') ? *format++ : ' ';

		/* Allow for minimum field width specifier for %d %u x,c,o,s */
		/* Also  allow %* for variable width (%0* as well)   */

		fmin = 0;

		if (*format == '*')
		{
			fmin = *args++;
			++format;
		}
		else
		{
			while (is_digit(*format))
			{
				fmin = fmin * 10 + *format++ - '0' ;
			}
		}

		/* Allow for maximum string width for %s */

		fmax = 0;

		if (*format == '.')
		{
			if (*(++format) == '*' )
			{
				fmax = *args++;
				++format;
			}
			else
			{
				while (is_digit(*format))
				{
					fmax = fmax * 10 + *format++ - '0';
				}
			}
		}

		if ((f = *format++) == 0)
		{
			_dputc('%');
			return; //return (-1);
		}

		sign = 0;

		switch (f)
		{
		case 'c' :
			string[0]	= (char) *args;
			string[1]	= '\0';
			str			= string;
			length		= 1;
			fmax		= 0;
			fill		= ' ';
			break;
		case 's' :
			str			= (char *) *args;
			for (length = 0; *str++; length ++);
			str			= (char *) *args;
			fill		= ' ';
			break;
		case 'd' :
			str			= string;
			i			= *(int *)args;
			if (i < 0)
			{
				sign	= '-';
				i		= -i;
			}
			length		= prt10(i, str);
			fmax		= 0;
			break;
		case 'o' :
			str			= string;
			length		= prt8(*(int *)args, str);
			fmax		= 0;
			break;
		case 'x' :
			str			= string;
			length		= prt16(*(int *)args, str);
			fmax		= 0;
			break;
		case 'X' :
			str			= string;
			length		= prt16X(*(int *)args, str);
			fmax		= 0;
			break;
		default:
			/* format is not supported */
			str			= (char *) *args;
			length		= 1;
			break;
		}

		leading = 0;

		if (fmax || fmin)
		{
			if (fmax)
			{
				if (length > fmax)
				{
					length	= fmax;
				}
			}
			if (fmin)
			{
				leading = fmin - length;
			}
			if (sign == '-')
			{
				leading --;
			}
		}
		if (sign == '-' && fill == '0')
		{
			_dputc(sign);
		}
		if (!leftjust)
		{
			for (i = 0; i < leading; i ++)
			{
				_dputc(fill);
			}
		}
		if (sign == '-' && fill == ' ')
		{
			_dputc(sign);
		}
		for (i = 0; i < length; i ++)
		{
			_dputc((*str++ & 0xff));
		}
		if (leftjust)
		{
			for (i = 0; i < leading; i ++)
			{
				_dputc(fill);
			}
		}

		args++;
	}
}

#else

/*
 *
 */
int d_printf(void (*dputc)(int), char *fmt, int *args)
{
	int c, i, f;
	int length, leftjust, leading;
	long fmax, fmin;
	int sign, fill;
	char *str;
	char string[64];

	for (;;)
	{
		/* Echo character until '%' or end of fmt string */
		while ((c = *fmt++) != '%')
		{
			if (!c)
				return (-1);
			(*dputc)(c);
		}
		/* Echo " ... %%..." as '%'  */
		if (*fmt == '%')
		{
			(*dputc)((*fmt++ & 0xff));
			continue;
		}
		/* Check for "%-...",== Left-justified output */
		if ((leftjust = (*fmt == '-')) != 0)
			fmt++;
		/* Allow for zero-filled numeric outputs ("%0...") */
		fill = (*fmt == '0') ? *fmt++ : ' ';
		/* Allow for minimum field width specifier for %d %u x,c,o,s */
		/* Also  allow %* for variable width (%0* as well)   */
		fmin = 0;
		if (*fmt == '*')
		{
			fmin = *args++;
			++fmt;
		}
		else
		{
			while (is_digit(*fmt))
				fmin = fmin * 10 + *fmt++ - '0';
		}
		/* Allow for maximum string width for %s */
		fmax = 0;
		if (*fmt == '.')
		{
			if (*(++fmt) == '*')
			{
				fmax = *args++;
				++fmt;
			}
			else
			{
				while (is_digit(*fmt))
					fmax = fmax * 10 + *fmt++ - '0';
			}
		}
		if ((f = *fmt++) == 0)
		{
			(*dputc)('%');
			return (-1);
		}
		sign = 0;
		switch (f)
		{
		case 'c':
			string[0] = (char)*args;
			string[1] = '\0';
			str = string;
			length = 1;
			fmax = 0;
			fill = ' ';
			break;
		case 's':
			str = (char *)*args;
			for (length = 0; *str++; length++)
				;
			str = (char *)*args;
			fill = ' ';
			break;
		case 'd':
			str = string;
			i = *(int *)args;
			if (i < 0)
			{
				sign = '-';
				i = -i;
			}
			length = prt10(i, str);
			fmax = 0;
			break;
		case 'u':
			str = string;
			length = prt10(*(unsigned int *)args, str);
			break;
		case 'o':
			str = string;
			length = prt8(*(int *)args, str);
			fmax = 0;
			break;
		case 'x':
			str = string;
			length = prt16(*(int *)args, str);
			fmax = 0;
			break;
		case 'X':
			str = string;
			length = PRT16(*(int *)args, str);
			fmax = 0;
			break;
		default:
			/* format is not supported */
			str = (char *)*args;
			length = 1;
			break;
		}

		//if (fmin > uDBG_txbuf_len)
		//	fmin = 0;
		//if (fmax > uDBG_txbuf_len)
		//	fmax = 0;

		leading = 0;
		if (fmax || fmin)
		{
			if (fmax)
				if (length > fmax)
					length = fmax;
			if (fmin)
				leading = fmin - length;
#if 1
			{
				if (sign == '-')
					leading--;
			}
#endif
		}
		{
			if (sign == '-' && fill == '0')
				(*dputc)(sign);
		}
		if (!leftjust)
			for (i = 0; i < leading; i++)
				(*dputc)(fill);
		{
			if (sign == '-' && fill == ' ')
				(*dputc)(sign);
		}
		for (i = 0; i < length; i++)
			(*dputc)((*str++ & 0xff));
		if (leftjust)
			for (i = 0; i < leading; i++)
				(*dputc)(fill);
		args++;
	}
}

int _dputc(int ch)
{
	static int old_ch = -1;

	if (ch == '\n' && old_ch != '\r')
	{
		iw_putc_uart(DEBUG_UART_PORT, '\r');
	}

	iw_putc_uart(DEBUG_UART_PORT, (old_ch = ch));

	return 0;
}

int _dgetc(int *pch)
{
	if (iw_getc_uart(DEBUG_UART_PORT, (char *)pch) < 0)
	{
		return -1;
	}

	return 0;
}

#define ASCII_CR (0x0Du)
void iw_get_cmd_str(char *buf)
{
	char ch;
	int i = 0;
	while (1)
	{
		if (iw_getc_uart(DEBUG_UART_PORT, &ch) == 0)
		{
			if (ch == ASCII_CR)
			{
				buf[i] = 0;
				iw_printf("\n");
				break;
			}
			else
			{
				buf[i++] = ch;
				iw_printf("%c", ch & 0xff);
			}
		}
		else
		{
			iw_sleep(1);
		}
	}
}

#if 0
#if defined(__GNUC__)
__attribute__((naked)) void iw_printf(char *format, ...)
{
	__asm volatile(
	    "stmdb	sp!, {r1-r3}	\n"
	    "mov	r1, r0			\n" /* r1 = format string */
	    "mov	r2, sp			\n" /* r2 = arguments */
	    "ldr	r0, =_dputc		\n" /* r0 = function pointer which will put character */
	    "str	lr, [sp, #-4]!	\n"		/* save r14 */
	    "bl		d_printf		\n"
	    "ldr	lr, [sp], #4	\n"		/* restore r14 */
	    "add	sp, sp, #12		\n" /* restore stack */
	    "mov	pc, lr			\n" ::
		: "r0", "r1", "r2", "r3");
}
#elif defined(__CC_ARM)
#error "ARM Compiler is not yet supported!"
#else
//#error "***** unknown compiler *****"
void iw_printf(char *format, ...)
{
	char str[256];
	va_list va;

	va_start(va, format);
	vsprintf(str, format, va);
	va_end(va);

	//Usart1_WriteBuff((u8 *)str, _strlen(str));
	for(int i = 0; i < strlen(str); i++) {
		_dputc(str[i]);
	}
}
#endif
#else
void iw_printf(char *format, ...)
{
	char str[256];
	va_list va;

	va_start(va, format);
	vsprintf(str, format, va);
	va_end(va);

	int len = strlen(str);
	for(int i = 0; i < len; i++) {
		_dputc(str[i]);
	}
}
#endif

#endif

#endif /* (DEBUG_OFF == 0) */

int stod(char *s)
{
	int t, n = 0;
	int sign;

	if (*s == '-')
	{
		sign = -1;
		do
		{
			s++;
			if (is_digit(*s) || *s == 0)
				break;
		} while (1);
	}
	else
		sign = 1;

	if (*s == '0' && to_upper(*(s + 1)) == 'X')
	{ // hexa-decimal conversion
		s += 2;
		while ((t = *s++) != 0)
		{
			if (t >= '0' && t <= '9')
				t = t - '0';
			else
			{
				t = to_upper(t);
				if (t >= 'A' && t <= 'F')
					t = t - 'A' + 10;
				else
					return n * sign;
			}
			n = (n << 4) + t;
		}
	}
	else
	{ // decimal conversion
		while ((t = *s) != 0)
		{
			if (t >= '0' && t <= '9')
				t = t - '0';
			else
				return n * sign;
			n = n * 10 + t;
			s++;
		}
	}
	return n * sign;
}
