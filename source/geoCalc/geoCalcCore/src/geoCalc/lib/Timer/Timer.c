#include <stddef.h>
#include "k_functionblock.h"
#include "k_util.h"

#include "DELAY"
#include "SPO"
#include "TOFF"
#include "TON"
#include "TP"

static void *fb_table[]={
	&fb_DELAY,
	&fb_SPO,
	&fb_TOFF,
	&fb_TON,
	&fb_TP,
	};

static int cursor;

void *timer_first()
{
	cursor = 0;
	return fb_table[0];
}

void *timer_next()
{
	cursor++;
	if(cursor < sizeof(fb_table) / sizeof(void *)){
		return fb_table[cursor];
	}else{
		return ((void*) 0);	//NULL
	}
}
