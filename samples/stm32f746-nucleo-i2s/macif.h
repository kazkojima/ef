// Interface for MAC descriptors

#include <string.h>

extern int get_free_tdes_index (void);
extern void *get_tdes_buffer (int idx);
extern void release_tdes (int idx, uint32_t ofs);
extern int get_rcvd_rdes_index (void);
extern uint16_t get_rcvd_rdes_len (int idx);
extern void *get_rdes_buffer (int idx);
extern void release_rdes (int idx);
