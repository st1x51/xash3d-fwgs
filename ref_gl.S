	.set noreorder

#include "pspstub.s"

	STUB_START "ref_gl",0x00090000,0x00010005
	STUB_FUNC  0xF4C1FB06,get_ref_exports
	STUB_END
