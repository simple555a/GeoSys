#include <stddef.h>
#include <math.h>
#include "k_functionblock.h"

#include "TWOSEL"
#include "THRSEL"
#include "SIX_SEL_N"

static void *fb_table[]={
	&fb_TWOSEL,
	&fb_THRSEL,
	&fb_SIX_SEL_N,
	};

static int cursor;

void *selection_first()
{
	cursor = 0;
	return fb_table[0];
}

void *selection_next()
{
	cursor++;
	if(cursor < sizeof(fb_table) / sizeof(void *)){
		return fb_table[cursor];
	}else{
		return ((void*) 0);	//NULL
	}
}
