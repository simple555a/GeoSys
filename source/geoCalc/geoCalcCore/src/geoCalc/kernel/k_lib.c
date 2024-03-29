#include <stddef.h>
#include <string.h>
#include "k_lib.h"

#include "lib/Arithmetic.h"
#include "lib/Bistable.h"
#include "lib/Comm.h"
#include "lib/Comparison.h"
#include "lib/Converter.h"
#include "lib/Control.h"
#include "lib/Counter.h"
#include "lib/Driver.h"
#include "lib/IO.h"
#include "lib/Logic.h"
#include "lib/Maths.h"
#include "lib/Selection.h"
#include "lib/System.h"
#include "lib/Trigger.h"
#include "lib/Timer.h"

#define NLIBCOUNT 15
static int cursor;
static lib_t lib_table[NLIBCOUNT]; 

static void lib_set_table(unsigned int id, char *name, fbget first, fbget next)
{
	if(id >= NLIBCOUNT){
		return;
	}
	
	strncpy(lib_table[id].libname, name, LIBNAMESIZE);
	lib_table[id].first = first;
	lib_table[id].next = next;
}

void lib_init()
{
	lib_set_table(0, "Arithmetic", arithmetic_first, arithmetic_next);
	lib_set_table(1, "Bistable", bistable_first, bistable_next);
	lib_set_table(2, "Comm", comm_first, comm_next);
	lib_set_table(3, "Comparison", comparison_first, comparison_next);
	lib_set_table(4, "Converter", converter_first, converter_next);
	lib_set_table(5, "Control", control_first, control_next);
	lib_set_table(6, "Counter", counter_first, counter_next);
	lib_set_table(7, "Driver", driver_first, driver_next);
	lib_set_table(8, "IO", io_first, io_next);
	lib_set_table(9, "Logic", logic_first, logic_next);
	lib_set_table(10, "Maths", maths_first, maths_next);
	lib_set_table(11, "Selection", selection_first, selection_next);
	lib_set_table(12, "System", system_first, system_next);
	lib_set_table(13, "Trigger", trigger_first, trigger_next);
	lib_set_table(14, "Timer", timer_first, timer_next);

	cursor = 0;
}

lib_t *lib_first()
{
	cursor = 0;
	return &lib_table[0];
}

lib_t *lib_next()
{
	cursor++;
	if(cursor < NLIBCOUNT){
		return &lib_table[cursor];
	}else{
		return ((void*) 0);	//NULL
	}
}

fb_t *lib_find(char *libname, char *fcname)
{
	lib_t *p_lib;
	fb_t *p_fb;
	
	p_lib = lib_first();
	while(p_lib != ((void*) 0)){
		if(strncmp(libname, p_lib->libname, LIBNAMESIZE) == 0){
			p_fb = p_lib->first();
			while(p_fb != ((void*) 0)){
				if(strncmp(fcname, p_fb->h.fcname, FCNAMESIZE) == 0){
					return p_fb;
				}
				p_fb = p_lib->next();
			}

			return ((void*) 0);
		}

		p_lib = lib_next();
	}
	
	return ((void*) 0);
}
