/* STEP - 4 */

#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "process.h"
#include "global.h"
#include "swapfile.h"
#include "tlb.h"

/* Invoked when an invalid address is encountered. Returns a free 
 * physical frame if available. If not, uses a page replacement 
 * algorithm to evict and return a currently used physical frame */

pfn_t pagefault_handler(vpn_t request_vpn, int write)
{
	pfn_t victim_pfn;
	vpn_t victim_vpn;
	pcb_t* victim_pcb;

	/* Get a (free / evicted) frame */
	victim_pfn = get_free_frame();	
	victim_vpn = rlt[victim_pfn].vpn;
	victim_pcb = rlt[victim_pfn].pcb;

	if(victim_pcb)
	{
		/* Pagetable of victim process */
		pte_t* victim_pt = victim_pcb->pagetable;

		/* If page is dirty, write it back to disk */
		if(victim_pt[victim_vpn].dirty)
		{
			page_to_disk(victim_pfn, victim_vpn, victim_pcb->pid);
		} 

		/* Invalidate the victim vpn in the victim pagetable */
		victim_pt[victim_vpn].valid = 0;

		/* Remove corresponding entry in the victim TLB */
		tlb_clearone(victim_vpn);
	}

	/* Update reverse lookup table with the correct mapping */
	rlt[victim_pfn].vpn = request_vpn;
	rlt[victim_pfn].pcb = current;

	/* Update current process' page table with the correct mapping*/
	pte_t* current_pt = current->pagetable;
	current_pt[request_vpn].pfn = victim_pfn;
	current_pt[request_vpn].valid = 1;
	current_pt[request_vpn].dirty = write;
	current_pt[request_vpn].used = 1;

	/* Retrieve page from disk */
	page_from_disk(victim_pfn, request_vpn, current->pid);

	return victim_pfn;
}  
