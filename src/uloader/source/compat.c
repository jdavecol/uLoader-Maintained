/*
 * Compatibility shims for precompiled 2012 libraries (libscreen.a, ...)
 * against modern libogc, where legacy symbols became static inlines and no
 * longer exist in libogc.a.
 */

void DCFlushRange(void *startaddress, unsigned int len)
{
	unsigned char *p = (unsigned char *)startaddress;
	unsigned int i;

	for (i = 0; i < len; i += 32)
		__asm__ __volatile__ ("dcbst 0, %0" : : "r"(p + i));
	__asm__ __volatile__ ("sync" : : : "memory");
}