#include "gateway.h"
#include "../../protected/my_function.h"
#include <pthread.h>

#if (TEST == 0)
#if (ENV == 0)
	#include <tee_client_api.h>
	typedef TEEC_SharedMemory Sh;
	typedef TEEC_Context Co;
	typedef TEEC_Session Se;
	typedef TEEC_UUID Uu;
#else
	typedef int Sh;
	typedef int Co;
	typedef int Se;
	typedef UI uint32_t;

	typedef struct s_id {
		int n1;
		int n2;
		int n3;
		int n4[8];
	} Uu;
#endif

volatile bool opened = false;
Uu ID = TA_HIDRA_UUID;
pthread_mutex_t m_ta;
Co CTX;
Se SES;

static void ga_openCtx(Co *ctx)
{
	if (opened)
		return;
	
	#if (ENV == 0)
		TEEC_Result r = TEEC_InitializeContext(NULL, ctx);
		if (r != TEEC_SUCCESS)
			errx(1, "ga_openCtx: failed with code 0x%x", r);
	#else
		(void) ctx;
	#endif
}

static void ga_closeCtx(Co *ctx)
{
	if (!opened)
		return;
	
	#if (ENV == 0)
		TEEC_FinalizeContext(ctx);
	#else
		(void) ctx;
	#endif
}

static void ga_openSes(Co *ctx, Se *ses, const Uu *id)
{
	if (opened)
		return;
	
	#if (ENV == 0)
		TEEC_Result r;
		uint32_t e;
		r = TEEC_OpenSession(ctx, ses, id, TEEC_LOGIN_PUBLIC, NULL, NULL, &e);
		if (r != TEEC_SUCCESS)
			errx(1, "ga_openSes: failed with code 0x%x origin 0x%x", r, e);
	#else
		(void) ctx;
		(void) ses;
		(void) id;
	#endif 

	if (pthread_mutex_init(&m_ta, NULL) != 0)
		errx(1, "ga_openSes: mutex init failed");
}

static void ga_closeSes(Se *ses)
{
	if (!opened)
		return;

	#if (ENV == 0)
		TEEC_CloseSession(ses);
	#else
		(void) ses;
		return;
	#endif
}

static void ga_open(void)
{
	ga_openCtx(&CTX);
	ga_openSes(&CTX, &SES, &ID);
	opened = true;
}

static void ga_close(void)
{
	ga_closeSes(&SES);
	ga_closeCtx(&CTX);
	opened = false;
}

static void ga_shareMemory(Sh *sm, UC *ptr, size_t s, uint32_t input)
{
	#if (ENV == 0)
	 	memset(sm, 0, sizeof(Sh));
	 	sm->buffer = ptr;
	 	sm->size = s;

	 	if (input == 0)
	 		sm->flags = TEEC_MEM_INPUT;
	 	else if (input == 1)
	 		sm->flags = TEEC_MEM_OUTPUT;
	 	else
	 		sm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	 	TEEC_Result r = TEEC_RegisterSharedMemory(&CTX, sm);
	 	if (r != TEEC_SUCCESS)
	 		errx(1, "ga_shareMemory: failed with code 0x%x", r);
	 #else
	 	(void) sm;
	 	(void) ptr;
	 	(void) s;
	 	(void) input;
	 #endif
}

static void ga_msg(UC *buf, int len, uint32_t code)
{
	Sh mbuf, mbin;

	UC *bin = (UC*) my_function; 
	int blen = ((UC*) dummy_function) - bin;

	ga_shareMemory(&mbuf, buf, len, 2);
	ga_shareMemory(&mbin, bin, blen, 0);

	#if (ENV == 0)
		TEEC_Operation op;
		memset(&op, 0, sizeof(op));
		op.paramTypes = TEEC_PARAM_TYPES(
		 	TEEC_MEMREF_WHOLE, 
		 	TEEC_MEMREF_WHOLE, 
		 	TEEC_NONE,
		 	TEEC_NONE
		 );
			
		op.params[0].memref.parent = &mbuf;
		op.params[1].memref.parent = &mbin;

		pthread_mutex_lock(&m_ta);
		uint32_t e;
		TEEC_Result r = TEEC_InvokeCommand(&SES, code, &op, &e);
		pthread_mutex_unlock(&m_ta);

		if (r != TEEC_SUCCESS)
		{
			ga_close();
			errx(1, "ga_msg: failed with code 0x%x", r);
		}

		TEEC_ReleaseSharedMemory(&mbuf);
		TEEC_ReleaseSharedMemory(&mbin);
	#else
		pthread_mutex_lock(&m_ta);
		errCO r = TEEC_InvokeCommand(buf, len, bin, blen, code);
		pthread_mutex_unlock(&m_ta);

		if (r != SUCCESS)
		{
			ga_close();
			errx(1, "ga_msg: failed with code 0x%x", r);
		}
	#endif
}

void ga_outgoing(UC buf[bufSZ])
{
	if (!opened)
		ga_open();

	ga_msg((UC*) (&(buf[0])), bufSZ, TA_HIDRA_OUTGOING); 
}

void ga_incoming(UC buf[bufSZ])
{
	if (!opened)
		ga_open(); 
	
	ga_msg((UC*) (&(buf[0])), bufSZ, TA_HIDRA_INCOMING); 
}

#endif