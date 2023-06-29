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

#include <dd/apic/pic.h>
#include <dd/apic/lapic.h>

#include <debug/log.h>
#include <utils/utils.h>

#include <stdint.h>

void lapic_init(void)
{
	pic_disable();
	uint32_t lapic_addr = PHYS_TO_VIRT(acpi_madt_get_lapic());
	lapic_out(LAPIC_SPURIOUS, LAPIC_ENABLE | LAPIC_SPURIOUS_INT);

	klog("done\n");
}

void lapic_eoi(void)
{
	lapic_out(LAPIC_EOI, 0);
}

void lapic_out(uint32_t reg, uint32_t data)
{
	mmio_outw(acpi_madt_get_lapic() + reg, data);
}

void lapic_in(uint32_t reg)
{
	mmio_inw(acpi_madt_get_lapic() + reg);
}