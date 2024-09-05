// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024 Rockchip Electronics Co. Ltd.
 */

#include <mbedtls/md.h>
#include <string.h>
#include "anti_copy_board_otp.h"

void ta_dump_hex(const char *name, const uint8_t *array, uint32_t len)
{
	uint32_t i;
	char buffer[256];
	char *p;

	p = buffer;
	for (i = 0; i < len; i++) {
		if (i % 16 == 0 && i != 0) {
			IMSG("%s", buffer);
			p = buffer;
			TEE_MemFill(buffer, 0x00, sizeof(buffer));
		}
		p += snprintf(p, 256, "%02x ", array[i]);
	}

	IMSG("=================== %s [%u] ================", name, len);
	IMSG("%s\n", buffer);
}

void calculate_hmac(const unsigned char *data, size_t data_len, unsigned char *output, const unsigned char *key, size_t key_len) {
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;

    mbedtls_md_init(&ctx);
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_setup(&ctx, info, 1);
    mbedtls_md_hmac_starts(&ctx, key, key_len);
    mbedtls_md_hmac_update(&ctx, data, data_len);
    mbedtls_md_hmac_finish(&ctx, output);

    mbedtls_md_free(&ctx);
}

TEE_Result handle_read_oem_otp(void)
{
	TEE_Result res = TEE_SUCCESS;

	uint32_t read_len = OTP_KINGRAY_DATA_LENGTH + KEY_LENGTH;
	uint32_t read_offset = OTP_OFFSET;
	uint8_t *read_data;

	//The memory used to invoke internal TA MUST BE secure memory, instead of CA memory.
	read_data = TEE_Malloc(read_len, 0);
	if (!read_data) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	res = rk_otp_read(read_offset, read_data, read_len);
	if (res != TEE_SUCCESS)
		EMSG("rk_otp_read failed with code 0x%x", res);
	else
		ta_dump_hex("read oem otp", read_data, read_len);

	TEE_Free(read_data);
	return res;
}

TEE_Result handle_write_oem_otp(void)
{
	TEE_Result res = TEE_SUCCESS;

	// Write 16 bytes data to OTP [0-15], write 32 bytes key to OTP [16-47].
	uint8_t *read_data;
	uint8_t write_data[] = OTP_DATA;
	uint8_t kingray_key[] = KINGRAY_KEY;
	uint32_t write_len = OTP_KINGRAY_DATA_LENGTH;
	uint32_t write_offset = OTP_OFFSET;
	uint32_t key_len = KEY_LENGTH;
	uint32_t key_offset = KEY_OFFSET;

	// The memory used to invoke internal TA MUST BE secure memory, instead of CA memory.
	read_data = TEE_Malloc(write_len, 0);
	if (!read_data) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	// Read before write.
	res = rk_otp_read(write_offset, read_data, write_len + key_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_read failed with code 0x%x", res);
		return res;
	} else {
		for(uint32_t i = 0; i < write_len; i++) {
			if (read_data[i] != 0x0) {
				EMSG("oem otp has already been write.");
				return res;
			}
		}
	}

	res = rk_otp_write(write_offset, write_data, write_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_write data failed with code 0x%x", res);
		return res;
	} else {
		ta_dump_hex("Write oem otp", write_data, write_len);
	}

	res = rk_otp_write(key_offset, kingray_key, key_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_write key failed with code 0x%x", res);
		return res;
	} else {
		ta_dump_hex("Write oem otp (key)", kingray_key, key_len);
	}

//	TEE_Free(read_data);
	return res;
}

/* Unused now */
TEE_Result handle_otp_size(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t otp_size = 0;

	res = rk_otp_size(&otp_size);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_size failed with code 0x%x", res);
		return res;
	}
	IMSG("The oem otp size is %d byte.", otp_size);
	return res;
}

TEE_Result handle_verify_oem_otp(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t read_len = OTP_KINGRAY_DATA_LENGTH;
	uint32_t key_len = KEY_LENGTH;
	uint32_t read_offset = OTP_OFFSET;
	uint32_t key_offset = KEY_OFFSET;
	uint8_t *read_data;
	uint8_t *kingray_key;

	uint8_t calculated_mac[KEY_LENGTH];
	uint8_t expected_mac[KEY_LENGTH] = EXPECTED_MAC;

	//The memory used to invoke internal TA MUST BE secure memory, instead of CA memory.
	read_data = TEE_Malloc(read_len, 0);
	if (!read_data) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	kingray_key = TEE_Malloc(key_len, 0);
	if (!kingray_key) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	res = rk_otp_read(read_offset, read_data, read_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_read failed with code 0x%x", res);
		goto verify_out;
	}

	res = rk_otp_read(key_offset, kingray_key, key_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_read failed with code 0x%x", res);
		goto verify_out;
	}
	// use HMAC to calculate MAC
	calculate_hmac(read_data, read_len, calculated_mac, kingray_key, sizeof(kingray_key));

	/* test */
	/*
	printf("Generated MAC:\n");
	for (int i = 0; i < 32; i++) {
		printf("%02x", calculated_mac[i]);
	}
	printf("\n");
	*/

	// verify MAC
	if (memcmp(calculated_mac, expected_mac, sizeof(expected_mac)) != 0) {
		EMSG("Verification failed");
		res = -2;
		goto verify_out;
	}
	IMSG("Verification successfully.");

verify_out:
	TEE_Free(read_data);
	return res;
}
