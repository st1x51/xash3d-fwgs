	.set noreorder

#include "pspstub.s"

	STUB_START "ref_gl",0x00090000,0x00010005
	STUB_FUNC  0xC525A942,load_refgu_exports
	STUB_END
