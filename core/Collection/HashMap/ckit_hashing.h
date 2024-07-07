#pragma once

#include "../../String/ckit_string.h"
//========================== Begin Types ==========================
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	u32 get_prime_by_index(u32 index);
	u64 hash(unsigned char *str);
	void test_hash_collisions();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

