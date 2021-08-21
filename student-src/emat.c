/* STEP - 3 */

#include "statistics.h"

/* Time in nano seconds */

#define MEMORY_ACCESS_TIME      100 
#define DISK_ACCESS_TIME   10000000 

double compute_emat()
{
	/* Total number of memory references */
	long int total_accesses = count_writes + count_reads;
	/* Number of TLB misses */
	long int tlb_misses     = total_accesses - count_tlbhits;

	/* Total time taken */
	long int total_time = (total_accesses + tlb_misses) * MEMORY_ACCESS_TIME 
						+ (count_pagefaults) * (2*MEMORY_ACCESS_TIME + DISK_ACCESS_TIME);

	/* Return mean access time */
	return total_time / (double) total_accesses;
}
