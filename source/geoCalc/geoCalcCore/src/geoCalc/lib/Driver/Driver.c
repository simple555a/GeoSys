#include <stddef.h>

#include "k_functionblock.h"
#include "k_util.h"

#include "D_AS_VAL"
#include "D_DS_VAL"
#include "D_DT_VAL"
#include "MTR_LGE"
#include "MTR_LTE"
#include "S_E_VAL"

static void *fb_table[]={
	&fb_D_AS_VAL,
	&fb_D_DS_VAL,
	&fb_D_DT_VAL,
	&fb_MTR_LGE,
	&fb_MTR_LTE,
	&fb_S_E_VAL,
	};

static int cursor;

void *driver_first()
{
	cursor = 0;
	return fb_table[0];
}

void *driver_next()
{
	cursor++;
	if(cursor < sizeof(fb_table) / sizeof(void *)){
		return fb_table[cursor];
	}else{
		return ((void*) 0);	//NULL
	}
}
