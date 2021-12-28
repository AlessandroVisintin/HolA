#ifndef _CERTIFICATION_H
#define _CERTIFICATION_H

#include "../../ta.h"

/*

Create message with certificate

*/
void ce_create(void);

/*

Verify certificate and send back this device plates

*/
void ce_verify(void);

/*

Save entry plates

*/
void ce_save(void);

#endif