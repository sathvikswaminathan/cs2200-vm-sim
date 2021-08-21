/* STEP - 5 */

#include <stdlib.h>

#include "types.h"
#include "pagetable.h"
#include "global.h"
#include "process.h"

/* Returns a free physical frame if available. If not, uses a page 
 * replacement algorithm to evict and return a currently used 
 * physical frame. */

pfn_t get_free_frame(void)
{
	int ret;

	/* Check for free frames */
	for(int i = 0; i < CPU_NUM_FRAMES; ++i)
	{
		if(rlt[i].pcb == NULL)
		{
			return i;
		}
	}

	/* If no free physical frame exists, use a page replacement algorithm */

	ret = clock_sweep();

	if(ret == -1)
	{
		/* If all else fails, return a random frame */
    	return rand() % CPU_NUM_FRAMES;
	}
	else
	{
		return ret;
	}
}

/* Clock sweep algorithm */
pfn_t clock_sweep(void)
{
	unsigned char swept = 0;

	for(int i = 0; i < CPU_NUM_FRAMES; ++i)
	{
		pte_t* pt = rlt[i].pcb->pagetable;
		vpn_t vpn = rlt[i].vpn;

		if(pt[vpn].used)
		{
			pt[vpn].used = 0;
		}
		else
		{
			return i;
		}

		if(swept == 0 && i == CPU_NUM_FRAMES - 1)
		{
			swept = 1;
			i = 0;
		}
	}

	return -1;
}


/*
pfn_t aging(void)
{
	
}

pfn_t wsclock(void)
{

}
*/