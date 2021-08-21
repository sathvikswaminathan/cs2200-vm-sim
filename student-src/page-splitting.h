/* STEP - 1 */

#ifndef _STUDENT_PAGE_SPLITTING_H_
#define _STUDENT_PAGE_SPLITTING_H_

#define VADDR_PAGENUM(addr) ((addr) / (page_size))

#define VADDR_OFFSET(addr) ((addr) % (page_size))

#endif/*_STUDENT_PAGE_SPLITTING_H_*/
