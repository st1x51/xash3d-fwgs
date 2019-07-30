TARGET = xash

SOURCE_DIRS := \
	engine/client \
	engine/client/avi \
	engine/client/vgui \
	engine/common \
	engine/common/imagelib \
	engine/common/soundlib \
	engine/common/soundlib/libmpg \
	engine/platform/psp \
	engine/server \
	public \

INCLUDES := \
	pm_shared \
	public \
	common \
	engine \
	engine/client \
	engine/client/avi \
	engine/client/vgui \
	engine/common \
	engine/common/imagelib \
	engine/common/soundlib \
	engine/common/soundlib/libmpg \
	engine/platform \
	engine/platform/psp \
	engine/server

CFILES := $(foreach dir,$(SOURCE_DIRS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.c))))
SFILES := ref_gu/ref_gl.S
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o) 
INCDIR = $(INCLUDES)
CFLAGS = -DSINGLE_BINARY -DXASH_NO_ASYNC_NS_RESOLVE -DPATH_MAX=256 -D__PSP__ -finline-limit=10 \
	-g -G0 -Werror=implicit-function-declaration -Werror=return-type -Werror=parentheses -fvisibility=hidden -Os
LIBS = -lc -lstdc++ -lm -lpspaudio -lpspvram -lpspgum -lpspgu -lpsphprm -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk \
	-lgl -lpspvfpu -lpsprtc -lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsplibc -lpsputility -lpspuser -lpspkernel

BUILD_PRX = 1
PSP_FW_VERSION = 660
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Xash3D
PSP_LARGE_MEMORY = 1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak