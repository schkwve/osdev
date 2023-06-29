/**
 * OSDev
 * Copyright (C) 2023 Jozef Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cpu/cpu.h>

#include <mm/virt.h>
#include <mm/phys.h>

#include <debug/log.h>
#include <utils/vector.h>

#include <stdint.h>
#include <stdbool.h>

static addrspace_t kaddrspace = { 0 };
vec_new_static(mem_map_t, mmap_list);

extern mem_info_t mem_info;

void virt_init(struct limine_memmap_response *mmap,
			   struct limine_kernel_address_response *kernel)
{
	kaddrspace.pml4 = kmalloc(PAGE_SIZE * 8);
	memset(kaddrspace.pml4, 0, PAGE_SIZE * 8);

	virt_map(NULL, MEM_VIRT_OFFSET, 0, NUM_PAGES(mem_info.phys_limit),
			 VIRT_FLAGS_DEFAULT | VIRT_FLAGS_USERMODE, true);
	klog("mapped %d bytes memory to 0x%x\n", mem_info.phys_limit,
		 MEM_VIRT_OFFSET);

	for (size_t i = 0; i < mmap->entry_count; i++) {
		struct limine_memmap_entry *entry = mmap->entries[i];

		switch (entry->type) {
		case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
			virt_map(NULL, entry->base, entry->base, NUM_PAGES(entry->length),
					 VIRT_FLAGS_DEFAULT | VIRT_FLAG_WRITECOMBINE |
						 VIRT_FLAGS_USERMODE,
					 false);
			klog("mapped 0x%x to 0x%x (len: %d)\n", entry->base, entry->base,
				 entry->length);
			break;
		case LIMINE_MEMMAP_FRAMEBUFFER:
			virt_map(NULL, PHYS_TO_VIRT(entry->base), entry->base,
					 NUM_PAGES(entry->length),
					 VIRT_FLAGS_DEFAULT | VIRT_FLAG_WRITECOMBINE |
						 VIRT_FLAGS_USERMODE,
					 true);
			klog("mapped 0x%x to 0x%x (len: %d)\n", entry->base,
				 PHYS_TO_VIRT(entry->base), entry->length);
			virt_map(NULL, entry->base, entry->base, NUM_PAGES(entry->length),
					 VIRT_FLAGS_DEFAULT | VIRT_FLAG_WRITECOMBINE |
						 VIRT_FLAGS_USERMODE,
					 false);
			klog("mapped 0x%x to 0x%x (len: %d)\n", entry->base, entry->base,
				 entry->length);
			break;
		case LIMINE_MEMMAP_KERNEL_AND_MODULES:
			uint64_t vaddr =
				kernel->virtual_base + entry->base - kernel->physical_base;
			virt_map(NULL, vaddr, entry->base, NUM_PAGES(entry->length),
					 VIRT_FLAGS_DEFAULT | VIRT_FLAGS_USERMODE, true);
			klog("mapped 0x%x to 0x%x (len: %d)\n", entry->base, vaddr,
				 entry->length);
			break;
		default:
			virt_map(NULL, PHYS_TO_VIRT(entry->base), entry->base,
					 NUM_PAGES(entry->length),
					 VIRT_FLAGS_DEFAULT | VIRT_FLAGS_USERMODE, true);
			klog("mapped 0x%x to 0x%x(len: %d)\n", entry->base,
				 PHYS_TO_VIRT(entry->base), entry->length);
			break;
		}
	}

	write_cr3(VIRT_TO_PHYS(kaddrspace.pml4));
	klog("done\n");
}

void virt_map_page(addrspace_t *addrspace, uint64_t vaddr, uint64_t paddr,
				   uint64_t flags)
{
	addrspace_t *as = (addrspace == NULL ? &kaddrspace : addrspace);

	uint16_t pte = (vaddr >> 12) & 0x1ff;
	uint16_t pde = (vaddr >> 21) & 0x1ff;
	uint16_t pdpe = (vaddr >> 30) & 0x1ff;
	uint16_t pml4e = (vaddr >> 39) & 0x1ff;

	uint64_t *pml4 = as->pml4;
	uint64_t *pdpt;
	uint64_t *pd;
	uint64_t *pt;

	pdpt = (uint64_t *)PHYS_TO_VIRT(pml4[pml4e] & ~(0xfff));
	if (!(pml4[pml4e] & VIRT_FLAG_PRESENT)) {
		pdpt = (uint64_t *)PHYS_TO_VIRT(phys_get(8, 0x0));
		memset(pdpt, 0, PAGE_SIZE * 8);
		pml4[pml4e] = MAKE_TABLE_ENTRY(VIRT_TO_PHYS(pdpt), VIRT_FLAGS_USERMODE);
	}

	pd = (uint64_t *)PHYS_TO_VIRT(pdpt[pdpe] & ~(0xfff));
	if (!(pdpt[pdpe] & VIRT_FLAG_PRESENT)) {
		pd = (uint64_t *)PHYS_TO_VIRT(phys_get(8, 0x0));
		memset(pd, 0, PAGE_SIZE * 8);
		pdpt[pdpe] = MAKE_TABLE_ENTRY(VIRT_TO_PHYS(pd), VIRT_FLAGS_USERMODE);
	}

	pt = (uint64_t *)PHYS_TO_VIRT(pd[pde] & ~(0xfff));
	if (!(pd[pde] & VIRT_FLAG_PRESENT)) {
		pt = (uint64_t *)PHYS_TO_VIRT(phys_get(8, 0x0));
		memset(pt, 0, PAGE_SIZE * 8);
		pd[pde] = MAKE_TABLE_ENTRY(VIRT_TO_PHYS(pt), VIRT_FLAGS_USERMODE);
	}

	pt[pte] = MAKE_TABLE_ENTRY(paddr & ~(0xfff), flags);

	if (read_cr3() == (uint64_t)(VIRT_TO_PHYS(as->pml4))) {
		invlpg(vaddr);
	}
}

void virt_unmap_page(addrspace_t *addrspace, uint64_t vaddr)
{
	addrspace_t *as = (addrspace == NULL ? &kaddrspace : addrspace);

	uint16_t pte = (vaddr >> 12) & 0x1ff;
	uint16_t pde = (vaddr >> 21) & 0x1ff;
	uint16_t pdpe = (vaddr >> 30) & 0x1ff;
	uint16_t pml4e = (vaddr >> 39) & 0x1ff;

	uint64_t *pml4 = as->pml4;
	if (!(pml4[pml4e] & VIRT_FLAG_PRESENT)) {
		return;
	}

	uint64_t *pdpt = (uint64_t *)PHYS_TO_VIRT(pml4[pml4e] & ~(0x1ff));
	if (!(pdpt[pdpe] & VIRT_FLAG_PRESENT)) {
		return;
	}

	uint64_t *pd = (uint64_t *)PHYS_TO_VIRT(pdpt[pdpe] & ~(0x1ff));
	if (!(pd[pde] & VIRT_FLAG_PRESENT)) {
		return;
	}

	uint64_t *pt = (uint64_t *)PHYS_TO_VIRT(pd[pde] & ~(0x1ff));
	if (!(pt[pte] & VIRT_FLAG_PRESENT)) {
		return;
	}

	pt[pte] = 0;

	if (read_cr3() == (uint64_t)(VIRT_TO_PHYS(as->pml4))) {
		invlpg(vaddr);
	}

	for (int i = 0; i < 512 * 8; i++)
		if (pt[i] != 0) {
			return;
		}

	pd[pde] = 0;
	phys_free(VIRT_TO_PHYS(pt), 8);

	for (int i = 0; i < 512 * 8; i++)
		if (pd[i] != 0) {
			return;
		}

	pdpt[pdpe] = 0;
	phys_free(VIRT_TO_PHYS(pd), 8);

	for (int i = 0; i < 512 * 8; i++)
		if (pdpt[i] != 0) {
			return;
		}

	pml4[pml4e] = 0;
	phys_free(VIRT_TO_PHYS(pdpt), 8);
}

uint64_t virt_get_paddr(addrspace_t *addrspace, uint64_t vaddr)
{
	addrspace_t *as = (addrspace == NULL ? &kaddrspace : addrspace);

	uint16_t pte = (vaddr >> 12) & 0x1ff;
	uint16_t pde = (vaddr >> 21) & 0x1ff;
	uint16_t pdpe = (vaddr >> 30) & 0x1ff;
	uint16_t pml4e = (vaddr >> 39) & 0x1ff;

	uint64_t *pml4 = as->pml4;
	if (!(pml4[pml4e] & VIRT_FLAG_PRESENT))
		return (uint64_t)NULL;

	uint64_t *pdpt = (uint64_t *)PHYS_TO_VIRT(pml4[pml4e] & ~(0x1ff));
	if (!(pdpt[pdpe] & VIRT_FLAG_PRESENT))
		return (uint64_t)NULL;

	uint64_t *pd = (uint64_t *)PHYS_TO_VIRT(pdpt[pdpe] & ~(0x1ff));
	if (!(pd[pde] & VIRT_FLAG_PRESENT))
		return (uint64_t)NULL;

	uint64_t *pt = (uint64_t *)PHYS_TO_VIRT(pd[pde] & ~(0x1ff));
	if (!(pt[pte] & VIRT_FLAG_PRESENT))
		return (uint64_t)NULL;

	return (pt[pte] & 0xFFFFFFFFFFFFF000);
}

void virt_map(addrspace_t *addrspace, uint64_t vaddr, uint64_t paddr,
			  uint64_t np, uint64_t flags, bool us)
{
	if (us && (addrspace == NULL)) {
		mem_map_t mm = {
			.vaddr = vaddr, .paddr = paddr, .flags = flags, .np = np
		};
		vec_push_back(&mmap_list, mm);
	}

	for (size_t i = 0; i < np * PAGE_SIZE; i += PAGE_SIZE) {
		virt_map_page(addrspace, vaddr + i, paddr + i, flags);
	}
}

void virt_unmap(addrspace_t *addrspace, uint64_t vaddr, uint64_t np, bool us)
{
	if (us && (addrspace == NULL)) {
		size_t len = vec_length(&mmap_list);
		for (size_t i = 0; i < len; i++) {
			mem_map_t m = vec_at(&mmap_list, i);
			if (m.vaddr != vaddr) {
				vec_erase(&mmap_list, i);
				break;
			}
		}
	}

	for (size_t i = 0; i < np * PAGE_SIZE; i += PAGE_SIZE) {
		virt_unmap_page(addrspace, vaddr + i);
	}
}