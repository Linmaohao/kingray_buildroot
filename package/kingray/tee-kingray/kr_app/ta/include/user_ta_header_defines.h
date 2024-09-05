/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2024 kingray Electronics Co. Ltd.
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_anti_copy_board.h> /* To get the TA_UUID define */

#define TA_UUID TA_ANTI_COPY_BOARD_UUID

#define TA_FLAGS			(TA_FLAG_USER_MODE | TA_FLAG_EXEC_DDR | TA_FLAG_MULTI_SESSION)
#define TA_STACK_SIZE		(2 * 1024)
#define TA_DATA_SIZE		(64 * 1024)

#endif
