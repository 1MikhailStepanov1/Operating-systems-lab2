#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <linux/pid.h>

static struct page* get_page_by_mm_and_address(struct mm_struct* mm, long address)
{
	pgd_t* pgd;
	p4d_t* p4d;
	pud_t* pud;
	pmd_t* pmd;
	pte_t* pte;

	struct page* page = NULL;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd)){
		return NULL;
	}

	p4d = p4d_offset(pgd, address);
	if (!p4d_present(*p4d)){
		return NULL;
	}

	pud = pud_offset(p4d, address);
	if (!pud_present(*pud)){
		return NULL;
	}

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd)){
		return NULL;
	}

	pte = pte_offset_kernel(pmd, address);
	if (!pte_present(*pte)){
		return NULL;
	}

	page = pte_page(*pte);
	return page;
}

SYSCALL_DEFINE1(page_read, pid_t, pid_input)
{
	struct page* my_page;
	struct task_struct* input_task;
	struct pid* result_pid;

	result_pid = find_get_pid(pid_input);
	input_task = get_pid_task(result_pid, PIDTYPE_PID);
	struct mm_struct* mm = input_task->mm;

	if (mm == NULL){
		printk(KERN_INFO "Task struct has no mm\n");
		return 0;
	}
	printk(KERN_INFO "Syscall was called.\n");
	struct vm_area_struct* vm_current = mm->mmap;
	long start = vm_current->vm_start;
	long end = vm_current->vm_end;
	long address = start;
	while (address <= end){
		my_page = get_page_by_mm_and_address(mm, address);
		if (my_page != NULL){
			printk(KERN_INFO "Page flags: %d\n", my_page->flags);
			printk(KERN_INFO "Virtual_address: %x, Page address: %x\n", address, my_page->mapping);
			break;
		}
		address += PAGE_SIZE;
	}

	if (my_page == NULL){
		printk(KERN_INFO "Cannot get page\n");
		return 0;
	}

	return 0;
}