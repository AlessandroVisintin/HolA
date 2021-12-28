#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include "../const.h"

// Entry point
#define pa_ofsEntr 0
#define pa_getEntr(x) (&((x)[pa_ofsEntr]))
#define pa_setEntr(x,y) (memcpy(pa_getEntr(x), &((y)[0]), ipaSZ))

// IP address
#define pa_ofsIpad (pa_ofsEntr + ipaSZ)
#define pa_getIpad(x) (&((x)[pa_ofsIpad]))
#define pa_setIpad(x,y) (memcpy(pa_getIpad(x), &((y)[0]), ipaSZ))

// MAC address
#define pa_ofsMaca (pa_ofsIpad + ipaSZ)
#define pa_getMaca(x) (&((x)[pa_ofsMaca]))
#define pa_setMaca(x,y) (memcpy(pa_getMaca(x), &((y)[0]), macSZ))

// Total length
#define parSZ (ipaSZ + ipaSZ + macSZ)


#endif