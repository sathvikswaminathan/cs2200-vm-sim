/* STEP - 2 */

#include "swapfile.h"
#include "statistics.h"
#include "pagetable.h"
#include <stdio.h>

/* Performs page table lookup and returns physical frame number. 
 * Page fault handler is called in case of pagefault. 
 */

pfn_t pagetable_lookup(vpn_t vpn, int write)
{
	/* page frame number to be returned */
	pfn_t pfn = 0;

	/* index into the pagetable using vpn to get the corresponding pte*/
	pte_t entry = current_pagetable[vpn];

	if(entry.valid)
	{
		pfn = entry.pfn;
	}
	else
	{
		++count_pagefaults;
		pfn = pagefault_handler(vpn, write);
	}

	return pfn;
}