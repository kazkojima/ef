/*
 * ST32F446 memory setup.
 */

MEMORY
{
    flash0 : org = 0x08000000, len = 4k
    flash  : org = 0x08000000+0x1000, len = 508k
    ram : org = 0x20000000, len = 128k
}

__ram_start__           = ORIGIN(ram);
__ram_size__            = 128k;
__ram_end__             = __ram_start__ + __ram_size__;

SECTIONS
{
    . = 0;

    .startup : ALIGN(4) SUBALIGN(4)
    {
	_startup = .;
        KEEP(*(.startup.vectors))
	. = ALIGN(1024);
	KEEP(*(.startup.entry))
	. = ALIGN(1024);
    } > flash0 =0xffffffff

    _text = .;
    .text : ALIGN(16) SUBALIGN(16)
    {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
	
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
	. = ALIGN(8);
    } > flash

    .ARM.extab : {*(.ARM.extab* .gnu.linkonce.armextab.*)} > flash

    .ARM.exidx : {
        PROVIDE(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE(__exidx_end = .);
     } > flash

    .eh_frame_hdr : {*(.eh_frame_hdr)} > flash

    .eh_frame : ONLY_IF_RO {*(.eh_frame)} > flash

    .init_array     :
    {   
        __init_array_start = .;
    	KEEP (*(SORT(.init_array.*)))
	KEEP (*(.init_array ))
        __init_array_end = .;
    } > flash

    .textalign : ONLY_IF_RO { . = ALIGN(8); } > flash

    _etext = .;
    _textdata = _etext;

    .vectors :
    {
        . = ALIGN(8);
        __vector_ram_addr__ = .;
	KEEP(*(.vectors))
    } > ram

    .data :
    {
        . = ALIGN(4);
        PROVIDE(_data = .);
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(_edata = .);
    } > ram AT > flash

    .bss :
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } > ram

    PROVIDE(end = .);
    _end            = .;
}

__initial_sp__  = __ram_end__ - 256;
__heap_base__   = _end;
__heap_end__    = __ram_end__ - 0x1000;
