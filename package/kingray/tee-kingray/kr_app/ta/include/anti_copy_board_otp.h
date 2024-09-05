// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024 Rockchip Electronics Co. Ltd.
 */

#include <stdio.h>
#include <tee_internal_api.h>
#include "../rk_public_api/rk_oem_otp_api.h"

#define OTP_KINGRAY_DATA_LENGTH	    16
#define OTP_OFFSET                  0
#define KEY_LENGTH                  32
#define KEY_OFFSET                  16
#define EXPECTED_MAC                {0x2e, 0xab, 0x31, 0xf3, 0x12, 0xe7, 0x46, 0x9c, 0x07, 0xff, 0x71, 0xec, 0xb0, 0x28, 0x55, 0x10, \
                                    0x9d, 0x1a, 0xcc, 0x0a, 0x75, 0x2a, 0xe5, 0x4c, 0xcc, 0x2d, 0x7d, 0x73, 0x2c, 0x66, 0x7a, 0x6b};
#if 1
#define OTP_DATA		            {0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0xee, 0xee, 0xff, 0xff, 0x11, 0x22, 0x33, 0x44};
#define KINGRAY_KEY                 { 'k', 'i', 'n', 'g', 'r', 'a', 'y', 's', 'u', 'p', 'e', 'r', 's', 'e', 'c', 'r', 'e', 't', 'k', 'e', 'y', 't', \
                                    'h', 'a', 't', 's', 'a', 'v', 'e', 'd', 'o', 'k' };
#else
#define OTP_DATA		            {};
#define KINGRAY_KEY                 {};
#endif

void ta_dump_hex(const char *name, const uint8_t *array, uint32_t len);
void calculate_hmac(const unsigned char *data, size_t data_len, unsigned char *output, const unsigned char *key, size_t key_len);
TEE_Result handle_read_oem_otp(void);
TEE_Result handle_write_oem_otp(void);
TEE_Result handle_verify_oem_otp(void);
TEE_Result handle_otp_size(void);