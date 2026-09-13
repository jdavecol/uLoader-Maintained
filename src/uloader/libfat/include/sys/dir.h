/*
 sys/dir.h - legacy devkitPro newlib directory API, local copy.

 Modern devkitPro newlib no longer ships the old devoptab directory
 interface (DIR_ITER/diropen/dirnext/dirclose) that uLoader (and this
 libFAT tree) was written against.  Keep a compatible definition here so
 libFAT can still be built from source.
*/

#ifndef _SYS_DIR_H_
#define _SYS_DIR_H_

#include <sys/stat.h>

struct _reent;

typedef struct {
	void *dirStruct;
} DIR_ITER;

DIR_ITER* diropen (const char *path);
int dirreset (DIR_ITER *dirState);
int dirnext (DIR_ITER *dirState, char *filename, struct stat *filestat);
int dirclose (DIR_ITER *dirState);

#endif /* _SYS_DIR_H_ */