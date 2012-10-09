#ifndef __KR_CFGPARSER_H__
#define __KR_CFGPARSER_H__

#include "kr_macros.h"

/*config file parsing functions*/
extern int kr_config_setfile(const char *vspFilename);
extern int kr_config_getstring(const char *vspSection, const char *vspEntry, char *vspValue);
extern int kr_config_getshort(const char *vspSection, const char *vspEntry, short *vnpValue);
extern int kr_config_getint(const char *vspSection, const char *vspEntry, int *vtpValue);
extern int kr_config_getlong(const char *vspSection, const char *vspEntry, long *vlpValue);
extern int kr_config_getdouble(const char *vspSection, const char *vspEntry, double *vdpValue);

#endif /* end of __KR_CFGPARSER_H__ */
