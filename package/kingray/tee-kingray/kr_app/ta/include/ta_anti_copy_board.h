/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2024, Kingray Electronics Co. Ltd.
 */

#ifndef __TA_ANTI_COPY_BOARD_H
#define __TA_ANTI_COPY_BOARD_H

/* This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define TA_ANTI_COPY_BOARD_UUID { 0xe9e28077, 0x1173, 0x4afb, \
		{ 0xbb, 0xa4, 0x25, 0x68, 0xb0, 0x4e, 0xef, 0x4a } }

#define TA_CMD_OEM_OTP_READ		200
#define TA_CMD_OEM_OTP_WRITE	201
#define TA_CMD_OEM_OTP_VERIFY	202
#define TA_CMD_OEM_OTP_SIZE		203

#endif /* __TA_ANTI_COPY_BOARD_H */
