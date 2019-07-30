TARGET = ref_gl

SOURCE_DIRS := \
	. \
	../public

INCLUDES := \
	../engine \
	../engine/common \
	../engine/server \
	../engine/client \
	../public \
	../common \
	../pm_shared \
	../engine/platform/psp

CFILES := $(foreach dir,$(SOURCE_DIRS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.c))))
OBJS = $(CFILES:.c=.o)
INCDIR = $(INCLUDES)
LIBS = -lc -lm -lgl -lpspvfpu -lpsprtc -lpspuser
CFLAGS = -DXASH_GL_STATIC -DREF_DLL -D__PSP__ -g -lm -G0 -Os
LDFLAGS = -nostartfiles

BUILD_PRX=1
PSP_FW_VERSION=660
PRX_EXPORTS = exports.exp

release: all
	psp-build-exports -k exports.exp
	psp-build-exports -s -v exports.exp

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak