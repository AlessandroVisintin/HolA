#ifndef _BUFFER_H
#define _BUFFER_H

#include "parameters.h"
#include "certificate.h"
#include "admittance.h"
#include "routine.h"

// Address
#define bu_ofsAddr 0
#define bu_getAddr(x) (&((x)[bu_ofsAddr]))
#define bu_setAddr(x,y) (memcpy(bu_getAddr(x), &((y)[0]), ipaSZ))

// Data length
#define bu_ofsLeng (bu_ofsAddr + ipaSZ)
#define bu_getLeng(x) (&((x)[bu_ofsLeng]))
#define bu_setLeng(x,y) (memcpy(bu_getLeng(x), &((y)[0]), 4))

// Pre-data length
#define preSZ (ipaSZ + 4)

// Pre-data nullify
#define bu_nulPred(x) (memset(bu_getAddr(x), 0, preSZ))

// Buffer data
#define bu_ofsData (bu_ofsLeng + 4)
#define bu_getData(x) (&((x)[bu_ofsData]))

// Data length
#define bu_max(x,y) (((x)>(y))?(x):(y))
#define datSZ (bu_max(parSZ, bu_max(cerSZ, bu_max(admSZ, rouSZ))))

// Data nullify
#define bu_nulData(x) (memset(bu_getData(x), 0, datSZ))

// Total buffer length
#define bufSZ (preSZ + datSZ)

// Buffer nullify
#define bu_nulBuff(x) (memset(bu_getAddr(x), 0, bufSZ))

#endif