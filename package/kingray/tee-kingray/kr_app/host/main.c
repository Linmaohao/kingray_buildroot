// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Kingray Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ta_anti_copy_board.h>
#include <tee_client_api.h>

#if 1
static const char* keyword[] = {"otp_read", "otp_write", "otp_verify", "otp_size", NULL};
#else
static const char* keyword[] = {"otp_verify", "otp_size", NULL};
#endif

static TEEC_Result invoke_otp_read(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	res = TEEC_InvokeCommand(session, TA_CMD_OEM_OTP_READ,
		operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("invoke_otp_read failed with code 0x%x origin 0x%x\n",
			res, error_origin);
	}
	return res;
};

static TEEC_Result invoke_otp_write(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	res = TEEC_InvokeCommand(session, TA_CMD_OEM_OTP_WRITE,
		operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("invoke_otp_write failed with code 0x%x origin 0x%x\n",
			res, error_origin);
	}
	return res;
};

static TEEC_Result invoke_otp_verify(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	res = TEEC_InvokeCommand(session, TA_CMD_OEM_OTP_VERIFY,
				  operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("invoke_otp_verify failed with code 0x%x origin 0x%x\n",
			res, error_origin);
	}
	return res;
};

static TEEC_Result invoke_otp_size(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	res = TEEC_InvokeCommand(session, TA_CMD_OEM_OTP_SIZE,
				  operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("invoke_otp_verify failed with code 0x%x origin 0x%x\n",
			res, error_origin);
	}
	return res;
};

static void printf_main_cmd(void)
{
	printf("Please entry one correct parameter when excuting the app!\n");
	printf("And no parameter is right, will excute otp_verify.\n");
	printf("The correct parameters list:\n");
	for (int i = 0; keyword[i] != NULL; i++)
		printf("	%s\n", keyword[i]);
}

static int verify_argv(const char *cp)
{
	for (int i = 0; keyword[i] != NULL; i++)
		if (strcasecmp(cp, keyword[i]) == 0)
			return 0;

	printf_main_cmd();
	return -1;
}

int main(int argc, char *argv[])
{
	if (argc != 1 && argc != 2) {
		printf_main_cmd();
		return -1;
	} else if (argc == 2) {
		if (verify_argv(argv[1]) != 0) {
			return -1;
		}

	}

	TEEC_Result res = TEEC_SUCCESS;
	uint32_t error_origin = 0;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;

	const TEEC_UUID anti_copy_board_uuid = TA_ANTI_COPY_BOARD_UUID;

	//[1] Connect to TEE
	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return res;
	}

	//[2] Open session with TEE application
	res = TEEC_OpenSession(&contex, &session, &anti_copy_board_uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
		       res, error_origin);
		goto out;
	}

	//[3] Perform operation initialization
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);

	if (argc > 1) {
		if (strcmp(argv[1], "otp_read") == 0) {
			res = invoke_otp_read(&session, &operation, &error_origin);

		} else if (strcmp(argv[1], "otp_write") == 0) {
			res = invoke_otp_write(&session, &operation, &error_origin);

		} else if (strcmp(argv[1], "otp_verify") == 0) {
			res = invoke_otp_verify(&session, &operation, &error_origin);

		} else if (strcmp(argv[1], "otp_size") == 0) {
			res = invoke_otp_size(&session, &operation, &error_origin);

		} else {
			printf("Unknown command\n");
		}
	} else {
#if 1
		res = invoke_otp_write(&session, &operation, &error_origin);
#else
		res = invoke_otp_verify(&session, &operation, &error_origin);
#endif
	}

	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}