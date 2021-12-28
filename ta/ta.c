#include "ta.h"
#include "task/task.h"
#include <operation/operation.h>

bool membership[oveNU]; // part of the ring
UC curOV; // overlay
UC curOP; // opcode
UC curME; // message
UC *curBU; // buffer
UC *curDA; // data
UC rcvBU[maiSZ]; // decrypted buffer
UC sndBU[maiSZ]; // decrypted result
UC *curPO; // attest binary
int curLE; // attest length
UC curMAC[macSZ];

#if (TEST == 0)
#if (ENV == 0)
	uint32_t curCO;
#else
	int curCO;
#endif
#else
	UI curCO;
#endif

bool check_membership(void)
{
	for (int i = 0; i < oveNU; i++)
	{
		if (!membership[i])
			return false;
	}
	return true;
}

void gen_random(UC *result, int len)
{
	if (len > 0)
	{
		#if (ENV == 1 || ENV == 2 || TEST == 1)
			for (int i = 0; i < len; i++)
				result[i] = (UC) (rand() % 255);
		#else
			TEE_GenerateRandom(result, len);

			UC hashed[shaSZ], tmp[shaSZ];
			memcpy((UC*) (&(hashed[0])), (UC*) (&(curMAC[0])), macSZ);
			if (len < (shaSZ-macSZ))
			{
				memcpy((UC*) (&(hashed[macSZ])), result, len);
				op_sha256(hashed, shaSZ, (UC*) (&(tmp[0])));
				memcpy(result, (UC*) (&(tmp[0])), len);
			}
			else
			{
				memcpy((UC*) (&(hashed[macSZ])), result, shaSZ-macSZ);
				op_sha256(hashed, shaSZ, (UC*) (&(tmp[0])));
				memcpy(result, (UC*) (&(tmp[0])), shaSZ-macSZ);
			}
			
		#endif
	}
}

#if (TEST == 0)
	void clock_set(Clock* t) {
		#if (ENV == 0)
			TEE_GetSystemTime(t);
		#else
			gettimeofday(t, NULL);
		#endif
	}

	millis clock_delta(Clock* t)
	{
		Clock n;

		#if (ENV == 0)
			TEE_GetSystemTime(&n);

			millis r = 0;
			if (n.seconds > t->seconds)
				r = (n.seconds - t->seconds)*1000L;
			if (n.millis > t->millis)
				r = r + n.millis - t->millis;
		#else
			gettimeofday(&n, NULL);

			millis r = 0;
			if (n.tv_sec > t->tv_sec)
				r = (n.tv_sec - t->tv_sec) * 1000L;
			if (n.tv_usec > t->tv_usec)
				r = r + ((n.tv_usec - t->tv_usec) / 1000L);
		#endif

		return r;
	}

	void clock_store(UC time[timSZ])
	{
		#if (ENV == 0)
			TEE_Time n;
			TEE_GetSystemTime(&n);

			memcpy(
				(UC*) (&(time[0])), 
				(UC*) (&n.seconds), 4);
			
			memcpy(
				(UC*) (&(time[secSZ])), 
				(UC*) (&n.millis), 4);
		#else
			struct timeval n;
			gettimeofday(&n, NULL);
			n.tv_usec = n.tv_usec / 1000L;

			memcpy(
				(UC*) (&(time[0])),
				(UC*) (&n.tv_sec), 8);
			
			memcpy(
				(UC*) (&(time[secSZ])),
				(UC*) (&n.tv_usec), 8);
		#endif
	}

	#if (ENV == 0)
		
		TEE_Result TA_CreateEntryPoint(void)
		{ return TEE_SUCCESS; }

		void TA_DestroyEntryPoint(void)
		{ return; }

		TEE_Result TA_OpenSessionEntryPoint(
			uint32_t pi, 
			TEE_Param __maybe_unused pars[4], 
			void __maybe_unused **ctx
		)
		{
			uint32_t pr = TEE_PARAM_TYPES(
				TEE_PARAM_TYPE_NONE, 
				TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE, 
				TEE_PARAM_TYPE_NONE
			);

			if (pi != pr) 
				return TEE_ERROR_BAD_PARAMETERS;

			// Unused parameters
			(void) &pars;
			(void) &ctx;

			return TEE_SUCCESS;
		}

		void TA_CloseSessionEntryPoint(void __maybe_unused *ctx)
		{ (void) &ctx; }
	#endif

	#if (ENV == 0)
		TEE_Result TA_InvokeCommandEntryPoint(
			void __maybe_unused *ctx, uint32_t cmd,
			uint32_t pi, TEE_Param pars[4])
	#else
		errCO TEEC_InvokeCommand(
			UC *buf, int len, UC *bin, int blen, UI code)
	#endif
	{
		#if (ENV == 0)
			(void) &ctx; // Unused parameter

			uint32_t pr = TEE_PARAM_TYPES(
				TEE_PARAM_TYPE_MEMREF_INOUT, // msg_buffer
				TEE_PARAM_TYPE_MEMREF_INPUT, // bin
				TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE
			);

			if (pi != pr) 
				return TEE_ERROR_BAD_PARAMETERS;

			curBU = (UC*) pars[0].memref.buffer;
			curDA = bu_getData(curBU);
			curPO = (UC*) pars[1].memref.buffer;
			curLE = pars[1].memref.size;
			curCO = cmd;

			if (curCO == TA_HIDRA_OUTGOING)
				outgoing();
			
			else if (curCO == TA_HIDRA_INCOMING)
				incoming();

			else
				return TEE_ERROR_BAD_PARAMETERS;

			return TEE_SUCCESS;
		#else
			curBU = buf;
			curDA = bu_getData(curBU);
			curPO = bin;
			curLE = blen;
			curCO = code;

			if (curCO == TA_HIDRA_OUTGOING)
				outgoing();
			
			else if (curCO == TA_HIDRA_INCOMING)
				incoming();

			else
				return BAD_PARAMETERS;

			return SUCCESS;
		#endif
	}
#endif