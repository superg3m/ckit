#include "../include/core_CKit.h"
#include "../include/core_types.h"
#include "../include/core_logger.h"
#include "../include/core_memory.h"
#include "../include/core_arena.h"
#include "../include/core_assert.h"
#include "../include/core_event_system.h"
#include "../include/core_vector.h"
#include "../include/core_string.h"
/*===========================================================
 * File: CKit.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

//========================== Begin Structs ==========================
//=========================== End Structs ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  void CKit_init() {
    assert_in_function(memory_init(), "memory_init failed\n");
  }
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
