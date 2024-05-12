#pragma once
/*===========================================================
 * File: core_errors.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

//========================== Begin Types ==========================
typedef enum ErrorCode {
    SUCCESS = 0x1,
    INVALID_ARGUMENT_VALUE = 0x2,
    INVALID_ARGUMENT_VALUE = 0x4,
    INVALID_ARGUMENT_VALUE = 0x8,
    INVALID_ARGUMENT_VALUE = 0x10,
    INVALID_ARGUMENT_VALUE = 0x20,
    INVALID_ARGUMENT_VALUE = 0x40,
    INVALID_ARGUMENT_VALUE = 0x80,
    INVALID_ARGUMENT_VALUE = 0x100,
    INVALID_ARGUMENT_VALUE = 0x200,
} ErrorCode;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    const char* error_code_readout(ErrorCode error_code);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++




/**
 * @brief Powers of Two for quick lookups
 * Anything that can error has to be able to tell why
 * so I might need macros macro helpers for functions that can error
 */

/**
 * @brief Error Code Function Standard
 * 
 * ErrorCode function(..., file, line, function)
 * Macro Helper(...) function(..., __FILE__, __LINE__, __func__)
 * 
 */


