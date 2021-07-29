#ifndef __LIB_COPROC_H__
#define __LIB_CORPOC_H__

/* double indirection for stringify                                           */
/*=> can be used in #define                                                   */
/*=> check http://stackoverflow.com/questions/12844364/stringify-c-preprocess */

#include "gic-types.h"

#define _stringify(s)           #s
#define stringify(s)            _stringify(s)

#define MRC_32(name,opc1,CRn,CRm,opc2)                                  \
        static inline u32 get_##name()                                          \
        {                                                               \
               u32 val;                                                 \
               asm volatile("mrc p15, " stringify(opc1)                 \
                            ", %0, c"   stringify(CRn)                  \
                            ", c" stringify(CRm)                        \
                            ", "  stringify(opc2) "\n" : "=r" (val));   \
               return val;                                              \
        }

#define MCR_32(name,opc1,CRn,CRm,opc2)                                  \
        static inline void set_##name(u32 val)                                  \
        {                                                               \
                asm volatile("mcr p15, " stringify(opc1)                 \
                            ", %0, c"   stringify(CRn)                  \
                            ", c" stringify(CRm)                        \
                            ", "  stringify(opc2) "\n" : : "r" (val));  \
        }


#define em_MRC_32(name,opc1,CRn,CRm,opc2)                                  \
        static inline u32 em_get_##name()                                          \
        {                                                               \
               u32 val;                                                 \
               asm volatile("mrc p15, " stringify(opc1)                 \
                            ", %0, c"   stringify(CRn)                  \
                            ", c" stringify(CRm)                        \
                            ", "  stringify(opc2) "\n" : "=r" (val));   \
               return val;                                              \
		}

#define em_MCR_32(name,opc1,CRn,CRm,opc2)                                  \
        static inline void em_set_##name(u32 val)                                  \
        {                                                               \
                asm volatile("mcr p15, " stringify(opc1)                 \
                            ", %0, c"   stringify(CRn)                  \
                            ", c" stringify(CRm)                        \
                            ", "  stringify(opc2) "\n" : : "r" (val));  \
        }



/*TODO 64bit access */

#define MRRC_64(name,opc1,CRn)                                          \
        static inline u64 get_##name()                                          \
        {                                                               \
                u32 val1, val2;                                         \
                asm volatile("mrrc p15, " stringify(opc1)               \
                        ", %0, %1, c" stringify(CRn) "\n"               \
                        : "=r" (val1), "=r" (val2) : :);                \
                return ((u64) val1 + ((u64) val2 << 32));               \
        }


#define MCRR_64(name,opc1,CRn)                                          \
        static inline void set_##name(u64 val)                                  \
        {                                                               \
                asm volatile("mcrr p15, " stringify(opc1)               \
                        ", %0, %1, c" stringify(CRn)                    \
                        "\n" : : "r" ((val) & 0xffffffff), "r" ((val) >> 32)); \
        }

//MRC_32(MPIDR,0,0,0,5)


#endif
