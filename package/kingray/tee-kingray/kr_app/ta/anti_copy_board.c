// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024 Kinigray Electronics Co. Ltd.
 */

#include <string.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "ta_anti_copy_board.h"
#include "anti_copy_board_otp.h"

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
				    TEE_Param  params[4], void **sess_ctx)
{
	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;
	(void)&param_types;

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the parameters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
				      uint32_t param_types, TEE_Param params[4])
{
	/* Unused parameters */
	(void)&sess_ctx;
	(void)&param_types;
	(void)&params;

	switch (cmd_id) {
	case TA_CMD_OEM_OTP_READ:
		return handle_read_oem_otp();
	case TA_CMD_OEM_OTP_VERIFY:
		return handle_verify_oem_otp();
	case TA_CMD_OEM_OTP_WRITE:
		return handle_write_oem_otp();
	case TA_CMD_OEM_OTP_SIZE:
		return handle_otp_size();
	default:
		EMSG("InvokeCommandEntry: BAD PARAMETERS!");
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
