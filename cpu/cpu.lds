/*
 * Linker script for Freetribe.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

HEAP_SIZE = 0x20000;
STACK_SIZE = 0x20000;
VECTOR_TABLE_SIZE = 0x80;

MEMORY
{
    OC_RAM (rwx)    : ORIGIN = 0x80000000, LENGTH = 0x20000
    EXT_RAM (rwx)   : ORIGIN = 0xC0000000, LENGTH = 0x4000000
    ARM_RAM (rwx)   : ORIGIN = 0xFFFF0000, LENGTH = 0x2000
}

SECTIONS
{
    . = 0xC0000000;
	. = ALIGN(4);

    .rsthand :
    {
        *init.S.o    (.text)
    } > EXT_RAM
 
	. = ALIGN(4);
	.text :
	{
	    *(.text)
	} > EXT_RAM

	. = ALIGN(4);

	.data :
	{
	    *(.data)
	} > EXT_RAM
	
	. = ALIGN(4);

	_bss_start = .;
	.bss :
	{
	    *(.bss)
	} > EXT_RAM

    . = ALIGN(8);

	_bss_end = .;
    end = .;
	
	.heap ALIGN(8) :
	{
        _heap_start = .;

	     . += HEAP_SIZE;

        _heap_end = .;
	} > EXT_RAM

	.stack :
	{
        . = ALIGN(8); 
        _stack_low = .;


	     . += STACK_SIZE; 

        _stack = .;
        stack = .;
	} > EXT_RAM

    /*
     * Vector table is moved to 
     * ARM RAM by startup code.
     */
	.vector_table :
	{
        . = ALIGN(8); 
        _vector_table = .;


	    . += VECTOR_TABLE_SIZE; 

	} > ARM_RAM


}
