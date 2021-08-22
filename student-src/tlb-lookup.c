/* STEP - 5 */
#include <stdlib.h>
#include <stdio.h>

#include "tlb.h"
#include "pagetable.h"
#include "global.h" 
#include "statistics.h"

/* Performs a TLB lookup and returns the physical frame number.
 * Performs a pagetable lookup in case of TLB miss.
 */ 
pfn_t tlb_lookup(vpn_t vpn, int write)
{
    /* Physical frame to be returned */
    pfn_t pfn;

    /* Check if entry exists in TLB */
    tlbe_t* entry = NULL;

    for(int i = 0; i < tlb_size; ++i)
    {
        if(tlb[i].vpn == vpn && tlb[i].valid)
        {
            ++count_tlbhits;

            entry = &tlb[i];

            pfn = entry->pfn;

            break;
        }
    } 

    /* If entry doesn't exist in TLB, do a pagetable search */
    if(!entry)
    {
        pfn = pagetable_lookup(vpn, write);

        /* Add entry to TLB 
         * Find victim entry by simpy checking for an invalid entry.
         * If no invalid entry exists, perform clock sweep algorithm 
         */

        for(int i = 0; i < tlb_size; ++i)
        {
            if(!tlb[i].valid)
                entry = &tlb[i];

            break;
        }

        if(!entry)
        {
            unsigned char swept = 0;

            for (int i = 0; i < tlb_size; i++) 
            {
                if (tlb[i].used) 
                {
                    tlb[i].used = 0;
                } 
                else 
                {
                    entry = &tlb[i];
                    break;
                }

                if (!swept && i == tlb_size - 1) 
                {
                    swept = 1;
                    i = 0;
                }
            }
        }
    }


    /* In case all else fails, use first entry as the victim */
    if(!entry)
    {
        entry = &tlb[0];
    }

    /* Update TLB entry */
    entry->vpn = vpn;
    entry->pfn = pfn;
    entry->valid = 1;
    entry->dirty = write;
    entry->used = 1;

    /* Update corresponding bits in Page table */
    current_pagetable[vpn].pfn = pfn;
    current_pagetable[vpn].valid = 1;
    current_pagetable[vpn].dirty = write;
    current_pagetable[vpn].used = 1;

    return pfn;
}
