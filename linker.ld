ENTRY(Reset_Handler)


MEMORY{
	FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 512K
	SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS{
	.text : {
		KEEP(*(.isr_vector))
		*(.text)
		*(.rodata)
		. = ALIGN(4);
		_etext = . ;
	} > FLASH

	.bss : {
		. = ALIGN(4);
		_sbss = .;
		*(.bss) 
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > SRAM
	
	.data : {
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_edata = .;
	} > SRAM AT > FLASH
}
