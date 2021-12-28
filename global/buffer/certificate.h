#ifndef _CERTIFICATE_H
#define _CERTIFICATE_H

#include "../const.h"

// Applicant
#define ce_ofsAppl 0
#define ce_getAppl(x) (&((x)[ce_ofsAppl]))
#define ce_setAppl(x,y) (memcpy(ce_getAppl(x), &((y)[0]), ipaSZ))

// Certificate
#define ce_ofsCert (ce_ofsAppl + ipaSZ)
#define ce_getCert(x) (&((x)[ce_ofsCert]))
#define ce_setCert(x,y) (memcpy(ce_getCert(x), &((y)[0]), rsaBK))

// Applicant public key
#define ce_ofsPubl (ce_ofsCert + rsaBK)
#define ce_getPubl(x) (&((x)[ce_ofsPubl]))
#define ce_setPubl(x,y) (memcpy(ce_getPubl(x), &((y)[0]), rsaPU))

// Total length
#define cerSZ (ipaSZ + rsaBK + rsaPU)

#endif