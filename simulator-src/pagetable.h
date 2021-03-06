#ifndef _PAGETABLE_H_
#define _PAGETABLE_H_

#include "types.h"

/*******************************************************************************
 * Looks up an address in the current page table. If the entry for the given
 * page is not valid, traps to the OS.
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t pagetable_lookup(vpn_t vpn, int write);

/*******************************************************************************
 * Finds a free physical frame. If none are available, uses a clock sweep
 * algorithm to find a used frame for eviction.
 *
 * @return The physical frame number of a free (or evictable) frame.
 */
pfn_t get_free_frame(void);

pfn_t clock_sweep(void);

pfn_t aging(void);

pfn_t fifo(void);

pfn_t nru(void);

pfn_t optimal(void);

pfn_t wsclock(void);  
/*******************************************************************************
 * Page table entry.
 */
typedef struct {
  pfn_t pfn;            /* Physical frame number */
  unsigned char valid;  /* Valid 'bit' */
  unsigned char dirty;  /* Dirty 'bit' */
  unsigned char used;   /* Used (aka accessed recently) 'bit' */
} pte_t;

/*******************************************************************************
 * Current page table register, used by the processor to find the running
 * processes page table upon a TLB miss.
 */
extern pte_t *current_pagetable;

/*******************************************************************************
 * Number of entries in a page table
 */
#define CPU_NUM_PTE ((1<<16) / page_size)

/*******************************************************************************
 * Number of frames in memory 
 */
#define CPU_NUM_FRAMES (mem_size / page_size)

#endif/*_PAGETABLE_H_*/
