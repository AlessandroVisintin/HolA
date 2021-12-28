#ifndef _ROUTINE_H
#define _ROUTINE_H

#include "../const.h"

// Overlay
#define ro_ofsOver 0
#define ro_getOver(x) ((x)[ro_ofsOver])
#define ro_setOver(x,y) ((x)[ro_ofsOver] = (y))

// Opcode
#define ro_ofsOpco (ro_ofsOver + 1)
#define ro_getOpco(x) ((x)[ro_ofsOpco])
#define ro_setOpco(x,y) ((x)[ro_ofsOpco] = (y))

// Process
#define ro_ofsProc (ro_ofsOpco + 1)
#define ro_getProc(x) (&((x)[ro_ofsProc]))
#define ro_setProc(x,y) (memcpy(ro_getProc(x), &((y)[0]), 4))

// Sequence
#define ro_ofsSequ (ro_ofsProc + 4)
#define ro_getSequ(x) ((x)[ro_ofsSequ])
#define ro_setSequ(x,y) (((x)[ro_ofsSequ]) = (y))

// Source
#define ro_ofsSour (ro_ofsSequ + 1)
#define ro_getSour(x) (&((x)[ro_ofsSour]))
#define ro_setSour(x,y) (memcpy(ro_getSour(x), &((y)[0]), plaSZ))

// Sender
#define ro_ofsSend (ro_ofsSour + plaSZ)
#define ro_getSend(x) (&((x)[ro_ofsSend]))
#define ro_setSend(x,y) (memcpy(ro_getSend(x), &((y)[0]), plaSZ))

// Recipient
#define ro_ofsReci (ro_ofsSend + plaSZ)
#define ro_getReci(x) (&((x)[ro_ofsReci]))
#define ro_setReci(x,y) (memcpy(ro_getReci(x), &((y)[0]), plaSZ))

// Header total length
#define ro_heaLE (1 + 1 + 4 + 1 + plaSZ + plaSZ + plaSZ)
#define ro_heaPA (aesBK  - (ro_heaLE % aesBK))
#define ro_heaSZ (ro_heaLE + ro_heaPA)

// Body
#define ro_ofsBody ro_heaSZ

// Body plates
#define ro_getPlat(x,y) (&((x)[ro_ofsBody+((y)*plaSZ)]))
#define ro_setPlat(x,y,z) (memcpy(ro_getPlat(x,y), &((z)[0]), plaSZ))

// Body proofs
#define ro_getProo(x,y) (&((x)[ro_ofsBody+((y)*proSZ)]))
#define ro_setProo(x,y,z) (memcpy(ro_getProo(x,y), &((z)[0]), proSZ))

// Body total length
#define ro_bodLE (plaSZ + (plaSZ*sucNU) + plaSZ + plaSZ)
#define ro_bodPA (aesBK  - (ro_bodLE % aesBK))
#define ro_bodSZ (ro_bodLE + ro_bodPA)

// Body capacity
#define ro_maxPL (ro_bodSZ / plaSZ)
#define ro_maxPR (ro_bodSZ / proSZ)

// Decryption check
#define ro_ofsChec (ro_heaSZ + ro_bodSZ)
#define ro_getChec(x) (&((x)[ro_ofsChec]))
#define ro_setChec(x,y) (memcpy(ro_getChec(x), &((y)[0]), aesBK))

// Initialization vector
#define ro_ofsVect (ro_ofsChec + aesBK)
#define ro_getVect(x) (&((x)[ro_ofsVect]))
#define ro_setVect(x,y) (memcpy(ro_getVect(x), &((y)[0]), aesBK))

// Main data total length
#define maiSZ (ro_heaSZ + ro_bodSZ + aesBK + aesBK)

// Monitor flag
#define ro_ofsMoni maiSZ
#define ro_getMoni(x) ((x)[ro_ofsMoni])
#define ro_setMoni(x,y) ((x)[ro_ofsMoni] = (y))

// Drop flag
#define ro_ofsDrop (ro_ofsMoni + 1)
#define ro_getDrop(x) ((x)[ro_ofsDrop])
#define ro_setDrop(x,y) ((x)[ro_ofsDrop] = (y))

// Attestation flag
#define ro_ofsFlag (ro_ofsDrop + 1)
#define ro_getFlag(x) ((x)[ro_ofsFlag])
#define ro_setFlag(x,y) ((x)[ro_ofsFlag] = (y))

// Proof number
#define ro_ofsPcou (ro_ofsFlag + 1)
#define ro_getPcou(x) ((x)[ro_ofsPcou])
#define ro_setPcou(x,y) ((x)[ro_ofsPcou] = (y))

// Finger number
#define ro_ofsFcou (ro_ofsPcou + 1)
#define ro_getFcou(x) ((x)[ro_ofsFcou])
#define ro_setFcou(x,y) ((x)[ro_ofsFcou] = (y))

// Start time
#define ro_ofsStar (ro_ofsFcou + 1)
#define ro_getStar(x) (&((x)[ro_ofsStar]))
#define ro_setStar(x,y) (memcpy(ro_getStar(x), &((y)[0]), timSZ))

// Decrypt time
#define ro_ofsDecr (ro_ofsStar + timSZ)
#define ro_getDecr(x) (&((x)[ro_ofsDecr]))
#define ro_setDecr(x,y) (memcpy(ro_getDecr(x), &((y)[0]), timSZ))

// Encrypt time
#define ro_ofsEncr (ro_ofsDecr + timSZ)
#define ro_getEncr(x) (&((x)[ro_ofsEncr]))
#define ro_setEncr(x,y) (memcpy(ro_getEncr(x), &((y)[0]), timSZ))

// End time
#define ro_ofsEndt (ro_ofsEncr + timSZ)
#define ro_getEndt(x) (&((x)[ro_ofsEndt]))
#define ro_setEndt(x,y) (memcpy(ro_getEndt(x), &((y)[0]), timSZ))

// Main data copy
#define ro_ofsCopy (ro_ofsEndt + timSZ)
#define ro_getCopy(x) (&((x)[ro_ofsCopy]))
#define ro_setCopy(x,y) (memcpy(ro_getCopy(x), &((y)[0]), (ro_heaSZ+ro_bodSZ)))

// Monitor data total length
#define monSZ (1 + 1 + 1 + 1 + 1 + timSZ + timSZ + timSZ + timSZ + ro_heaSZ + ro_bodSZ)

// Total length
#define rouSZ (maiSZ + monSZ)

#endif