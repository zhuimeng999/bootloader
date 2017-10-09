OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Vectors)

INTERNAL_SRAM_START = 0xFFFF0000;

SECTIONS {
	. = INTERNAL_SRAM_START;
	__image_start = .;
	.text : {
		./src/start.o(.text*)
		*(.text*)
	}
	
	. = ALIGN(4);
	.rodata : {
		*(.rodata*)
	}
	
	. = ALIGN(4);
	.data : {
		*(.data*)
	}
	__image_end = .;
	
	. = ALIGN(4);
	__bss_start = .;
	.bss : {
		*(.bss*)
	}
	__bss_end = .;
	
	. = ALIGN(4);
	COMMON : {
		*(COMMON)
	}
	
	. = INTERNAL_SRAM_START + 32K;
	__stack_start = .;
	.stack : {
		. += 16K;
	}
	__stack_end = .;
	
	. = INTERNAL_SRAM_START + 48K;
	.ttb : {
		. = 16K;
	}
}

__image_size = __image_end - __image_start;

__bss_end = ALIGN(__bss_end, 4);
__image_size = ALIGN(__image_size, 16);

/* newlib req */
end = __bss_end;
_Heap_Limit = __stack_start;

