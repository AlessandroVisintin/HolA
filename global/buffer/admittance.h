#ifndef _ADMITTANCE_H
#define _ADMITTANCE_H

#include "../const.h"

// RSA-blocks
#define ad_ofsRsab 0
#define ad_getRsab(x,y) (&((x)[(ad_ofsRsab)+(y*rsaBK)]))
#define ad_setRsab(x,y,z) (memcpy(ad_getRsab(x,y), &((z)[0]), rsaBK))

// Total length
#define admSZ (oveNU*rsaBK)

#endif