/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Linker script for Leaf Maple (STM32F103RBT6, 128K flash, 20K RAM,
 * minus 20k flash and 3k RAM for the DFU bootloader). */

/* Define memory regions. */
/*MEMORY
{
	rom (rx) : ORIGIN = 0x08005000, LENGTH = 108K
	ram (rwx) : ORIGIN = 0x20000c00, LENGTH = 17K
}*/

MEMORY
{
	rom (rx) : ORIGIN = 0x08002000, LENGTH = 120K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}
/* Include the common ld script. */
INCLUDE libopencm3_stm32f1.ld

