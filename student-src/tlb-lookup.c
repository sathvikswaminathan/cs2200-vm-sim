#include <stdlib.h>
#include <stdio.h>

#include "tlb.h"
#include "pagetable.h"
#include "global.h" 
#include "statistics.h"

pfn_t tlb_lookup(vpn_t vpn, int write) {
    pfn_t pfn;

    tlbe_t *entry = NULL;
    for (int i = 0; i < tlb_size; i++) {
        if (tlb[i].vpn == vpn && tlb[i].valid) {
            entry = &tlb[i];
            pfn = entry->pfn;
            count_tlbhits++;
            break;
        }
    }

    if (!entry) {
        pfn = pagetable_lookup(vpn, write);

        for (int i = 0; i < tlb_size; i++) {
            if (!tlb[i].valid) {
                entry = &tlb[i];
                break;
            }
        }

        unsigned char swept = 0;
        for (int i = 0; i < tlb_size; i++) {
            if (tlb[i].used) {
                tlb[i].used = 0;
            } else {
                entry = &tlb[i];
                break;
            }

            if (!swept && i == tlb_size - 1) {
                swept = 1;
                i = 0;
            }
        }
    }

    entry->vpn = vpn;
    entry->pfn = pfn;
    entry->valid = 1;
    entry->dirty = write;
    entry->used = 1;

    current_pagetable[vpn].pfn = pfn;
    current_pagetable[vpn].valid = 1;
    current_pagetable[vpn].dirty = write;
    current_pagetable[vpn].used = 1;

    return pfn;
}
