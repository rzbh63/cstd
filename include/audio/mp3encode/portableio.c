
#include	<stdio.h>
#include	<string.h>
#include	<math.h>
#include	"portableio.h"

/****************************************************************
 * Big/little-endian independent I/O routines.
 ****************************************************************/


int 
ReadByte(fp)
FILE *fp;
{
	int	result;

	result = getc(fp) & 0xff;        //取低八位
	if (result & 0x80)               //如果result为负数，则取取其补数
		result = result - 0x100;
	return result;
}


int 
Read16BitsLowHigh(fp)
FILE *fp;
{
	int	first, second, result;

	first = 0xff & getc(fp);
	second = 0xff & getc(fp);

	result = (second << 8) + first;   //second先左移8位再与first相加
#ifndef	THINK_C42
	if (result & 0x8000)
		result = result - 0x10000;
#endif	/* THINK_C */
	return(result);
}


int
Read16BitsHighLow(fp)
FILE *fp;
{
	int	first, second, result;

	first = 0xff & getc(fp);
	second = 0xff & getc(fp);

	result = (first << 8) + second;
#ifndef	THINK_C42
	if (result & 0x8000)
		result = result - 0x10000;
#endif	/* THINK_C */
	return(result);
}


void
Write8Bits(fp, i)
FILE *fp;
int i;
{
	putc(i&0xff,fp);
}


void
Write16BitsLowHigh(fp, i)
FILE *fp;
int i;
{
	putc(i&0xff,fp);
	putc((i>>8)&0xff,fp);
}


void
Write16BitsHighLow(fp, i)
FILE *fp;
int i;
{
	putc((i>>8)&0xff,fp);
	putc(i&0xff,fp);
}


int
Read24BitsHighLow(fp)
FILE *fp;
{
	int	first, second, third;
	int	result;

	first = 0xff & getc(fp);
	second = 0xff & getc(fp);
	third = 0xff & getc(fp);

	result = (first << 16) + (second << 8) + third;
	if (result & 0x800000)
		result = result - 0x1000000;
	return(result);
}

#define	Read32BitsLowHigh(f)	Read32Bits(f)


int
Read32Bits(fp)
FILE *fp;
{
	int	first, second, result;

	first = 0xffff & Read16BitsLowHigh(fp);
	second = 0xffff & Read16BitsLowHigh(fp);

	result = (second << 16) + first;
#ifdef	CRAY
	if (result & 0x80000000)
		result = result - 0x100000000;
#endif	/* CRAY */
	return(result);
}


int
Read32BitsHighLow(fp)
FILE *fp;
{
	int	first, second, result;

	first = 0xffff & Read16BitsHighLow(fp);
	second = 0xffff & Read16BitsHighLow(fp);

	result = (first << 16) + second;
#ifdef	CRAY
	if (result & 0x80000000)
		result = result - 0x100000000;
#endif
	return(result);
}


void
Write32Bits(fp, i)
FILE *fp;
int i;
{
	Write16BitsLowHigh(fp,(int)(i&0xffffL));
	Write16BitsLowHigh(fp,(int)((i>>16)&0xffffL));
}


void
Write32BitsLowHigh(fp, i)
FILE *fp;
int i;
{
	Write16BitsLowHigh(fp,(int)(i&0xffffL));
	Write16BitsLowHigh(fp,(int)((i>>16)&0xffffL));
}


void
Write32BitsHighLow(fp, i)
FILE *fp;
int i;
{
	Write16BitsHighLow(fp,(int)((i>>16)&0xffffL));
	Write16BitsHighLow(fp,(int)(i&0xffffL));
}

void ReadBytes(fp, p, n)
FILE	*fp;
char	*p;
int	n;
{
	while (!feof(fp) && n-- > 0)
		*p++ = getc(fp);
}

void ReadBytesSwapped(fp, p, n)
FILE	*fp;
char	*p;
int	n;
{
	register char	*q = p;

	while (!feof(fp) && n-- > 0)
		*q++ = getc(fp);

	for (q--; p < q; p++, q--){
		n = *p;
		*p = *q;
		*q = n;
	}
}

void WriteBytes(fp, p, n)
FILE	*fp;
char	*p;
int	n;
{
	while (n-- > 0)
		putc(*p++, fp);
}

void WriteBytesSwapped(fp, p, n)
FILE	*fp;
char	*p;
int	n;
{
	p += n-1;
	while (n-- > 0)
		putc(*p--, fp);
}

defdouble
ReadIeeeFloatHighLow(fp)
FILE *fp;
{
	char	bits[kFloatLength];

	ReadBytes(fp, bits, kFloatLength);
	return ConvertFromIeeeSingle(bits);
}

defdouble
ReadIeeeFloatLowHigh(fp)
FILE *fp;
{
	char	bits[kFloatLength];

	ReadBytesSwapped(fp, bits, kFloatLength);
	return ConvertFromIeeeSingle(bits);
}

defdouble
ReadIeeeDoubleHighLow(fp)
FILE *fp;
{
	char	bits[kDoubleLength];

	ReadBytes(fp, bits, kDoubleLength);
	return ConvertFromIeeeDouble(bits);
}

defdouble
ReadIeeeDoubleLowHigh(fp)
FILE *fp;
{
	char	bits[kDoubleLength];

	ReadBytesSwapped(fp, bits, kDoubleLength);
	return ConvertFromIeeeDouble(bits);
}

defdouble
ReadIeeeExtendedHighLow(fp)
FILE *fp;
{
	char	bits[kExtendedLength];

	ReadBytes(fp, bits, kExtendedLength);
	return ConvertFromIeeeExtended(bits);
}

defdouble
ReadIeeeExtendedLowHigh(fp)
FILE *fp;
{
	char	bits[kExtendedLength];

	ReadBytesSwapped(fp, bits, kExtendedLength);
	return ConvertFromIeeeExtended(bits);
}

void
WriteIeeeFloatLowHigh(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kFloatLength];

	ConvertToIeeeSingle(num,bits);
	WriteBytesSwapped(fp,bits,kFloatLength);
}

void
WriteIeeeFloatHighLow(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kFloatLength];

	ConvertToIeeeSingle(num,bits);
	WriteBytes(fp,bits,kFloatLength);
}

void
WriteIeeeDoubleLowHigh(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kDoubleLength];

	ConvertToIeeeDouble(num,bits);
	WriteBytesSwapped(fp,bits,kDoubleLength);
}

void
WriteIeeeDoubleHighLow(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kDoubleLength];

	ConvertToIeeeDouble(num,bits);
	WriteBytes(fp,bits,kDoubleLength);
}

void
WriteIeeeExtendedLowHigh(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kExtendedLength];

	ConvertToIeeeExtended(num,bits);
	WriteBytesSwapped(fp,bits,kExtendedLength);
}


void
WriteIeeeExtendedHighLow(fp, num)
FILE *fp;
defdouble num;
{
	char	bits[kExtendedLength];

	ConvertToIeeeExtended(num,bits);
	WriteBytes(fp,bits,kExtendedLength);
}


enum e_byte_order DetermineByteOrder()
{
    char s[ sizeof(long) + 1 ];
    union
    {
        long longval;
        char charval[ sizeof(long) ];
    } probe;
    probe.longval = 0x41424344L;  /* ABCD in ASCII */
    strncpy( s, probe.charval, sizeof(long) );
    s[ sizeof(long) ] = '\0';
    /* fprintf( stderr, "byte order is %s\n", s ); */
    if ( strcmp(s, "ABCD") == 0 )
        return order_bigEndian;
    else
        if ( strcmp(s, "DCBA") == 0 )
            return order_littleEndian;
        else
            return order_unknown;
}

void SwapBytesInWords( short *loc, int words )
{
    int i;
    short thisval;
    char *dst, *src;
    src = (char *) &thisval;
    for ( i = 0; i < words; i++ )
    {
        thisval = *loc;
        dst = (char *) loc++;
        dst[0] = src[1];
        dst[1] = src[0];
    }
}


