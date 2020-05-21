
#define Test 0  // 1 - тестовый режим без MVD


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\timeb.h>

#include "device.h"

#if defined(MMI_ICP)
#include "mmi.h"
#endif

#if defined(MMI_NEW)
#include "mmi_new.h"
#endif


#include "PROC.h"

#include "queue.h"
#include "icp.h"
#include "host.h"
#include "7017c.h"
#include "i7060.h"
#include "eee.h"
#include "rtu.h"
#include "sk.h"
#include "delta.h"
#include "MVD.h"
#include "tsd.h"
#include "arch.h"
#include "avt_ctrl.h"
#include "pmlan.h"
#include "reg.h"
#include "prop_but.h"
#include "lin.h"
#include "md5.h"



