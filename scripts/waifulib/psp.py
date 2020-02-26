from waflib import TaskGen, Task

import os
import sys

PSP_SDK_ENVVARS = ['PSPSDK']

class PSP:
	ctx = None

	def __init__(self, ctx):
		self.ctx = ctx

		for i in PSP_SDK_ENVVARS:
			self.sdk_home = os.getenv(i)
			if self.sdk_home != None:
				self.sdk_bin = os.path.join(self.sdk_home, 'bin')
				self.sdk_psp = os.path.join(self.sdk_home, 'psp', 'sdk')
				break
		else:
			ctx.fatal('Set %s environment variable pointing to the root of PSP SDK!' %
				' or '.join(PSP_SDK_ENVVARS))
	
	def cflags(self):
		cflags = ['-G0', '-D_PSP_FW_VERSION=660', '-I.']
		cflags += ['-I%s' % os.path.join(self.sdk_psp, 'include')]
		return cflags
	
	def ldflags(self):
		ldflags = ['-specs=%s' % os.path.join(self.sdk_psp, 'lib', 'prxspecs')]
		ldflags += ['-Wl,-q,-T%s' % os.path.join(self.sdk_psp, 'lib', 'linkfile.prx')]
		ldflags += ['-L.', '-L%s' % os.path.join(self.sdk_psp, 'lib')]
		ldflags += ['-lstdc++', '-lc', '-lm', '-lpspdebug', '-lpspgu', '-lpspdisplay', '-lpspge', '-lpspctrl', '-lpspsdk']
		ldflags += ['-lpspnet', '-lpspnet_inet', '-lpspnet_apctl', '-lpspnet_resolver', '-lpsputility', '-lpspuser', '-lpspkernel']
		return ldflags

def options(opt):
	grp = opt.add_option_group('PSP options')
	grp.add_option('--enable-psp', action='store_true', dest='PSP', default=False, help='enable building for psp')

def configure(conf):
	if not conf.options.PSP:
		return

	conf.psp = psp = PSP(conf)
	conf.find_program('psp-gcc', var='CC', path_list=psp.sdk_bin)
	conf.find_program('psp-gcc', var='CXX', path_list=psp.sdk_bin)
	conf.find_program('psp-gdc', var='GDC', path_list=psp.sdk_bin)
	conf.find_program('psp-gcc', var='AS', path_list=psp.sdk_bin)
	conf.find_program('psp-gcc', var='LD', path_list=psp.sdk_bin)
	conf.find_program('psp-ar', var='AR', path_list=psp.sdk_bin)
	conf.find_program('psp-ranlib', var='RANLIB', path_list=psp.sdk_bin)
	conf.find_program('psp-strip', var='STRIP', path_list=psp.sdk_bin)
	conf.find_program('psp-objcopy', var='OBJCOPY', path_list=psp.sdk_bin)

	conf.find_program('psp-prxgen', var='PRXGEN', path_list=psp.sdk_bin)
	conf.find_program('mksfoex', var='MKSFO', path_list=psp.sdk_bin)
	conf.find_program('pack-pbp', var='PACK_PBP', path_list=psp.sdk_bin)
	conf.find_program('psp-fixup-imports', var='FIXUP', path_list=psp.sdk_bin)

	conf.env.CFLAGS += psp.cflags()
	conf.env.CXXFLAGS += psp.cflags()
	conf.env.LDFLAGS += psp.ldflags()

def build(bld):
	if not bld.env.DEST_OS == 'psp':
		return
	
	bld(rule='${FIXUP} -o ${TGT} ${SRC}', source='engine/xash', target='xash.elf')
	bld(rule='${PRXGEN} ${SRC} ${TGT}', source='xash.elf', target='xash.prx')
	bld(rule='${MKSFO} -d MEMSIZE=1 Xash3D ${TGT}', source='xash.prx', target='PARAM.SFO')
	bld(rule='${PACK_PBP} EBOOT.PBP PARAM.SFO NULL NULL NULL NULL NULL xash.prx NULL', source='xash.prx PARAM.SFO', target='EBOOT.PBP')
