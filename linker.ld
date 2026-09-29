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
	
	.data : {
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_edata = .;
	} > SRAM AT > FLASH

	.bss : {
		. = ALIGN(4);
		_sbss = .;
		__bss_start__ = .;
		*(.bss) 
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = .;
	} > SRAM

	. = ALIGN(4);
	end = .;

}
