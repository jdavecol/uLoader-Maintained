/*
 dir.c - legacy newlib devoptab directory wrappers for libFAT.

 uLoader relies on the old devkitPro newlib directory API
 (diropen/dirnext/dirclose).  Modern newlib removed those public wrappers in
 favour of POSIX opendir/readdir/closedir and no longer provides them, so
 the link fails with undefined references.  Reimplement them here, directly
 on top of libFAT's internal _FAT_dir*_r routines, matching the memory
 contract those routines expect.
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/dir.h>

#include "fatdir.h"

DIR_ITER* diropen (const char *path) {
	DIR_STATE_STRUCT *state = (DIR_STATE_STRUCT*) malloc (sizeof (DIR_STATE_STRUCT));
	if (state == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	memset (state, 0, sizeof (DIR_STATE_STRUCT));

	DIR_ITER container;
	container.dirStruct = (void*) state;

	if (_FAT_diropen_r (_REENT, &container, path) == NULL) {
		free (state);
		return NULL;
	}

	return (DIR_ITER*) state;
}

int dirreset (DIR_ITER *dirState) {
	DIR_ITER container;
	container.dirStruct = (void*) dirState;
	return _FAT_dirreset_r (_REENT, &container);
}

int dirnext (DIR_ITER *dirState, char *filename, struct stat *filestat) {
	DIR_ITER container;
	container.dirStruct = (void*) dirState;
	return _FAT_dirnext_r (_REENT, &container, filename, filestat);
}

int dirclose (DIR_ITER *dirState) {
	DIR_ITER container;
	container.dirStruct = (void*) dirState;
	int ret = _FAT_dirclose_r (_REENT, &container);
	free (dirState);
	return ret;
}