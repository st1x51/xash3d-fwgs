TARGET = libref_gl.a

SOURCE_DIRS := \
	. \
	../public

INCLUDES := \
	-I../engine \
	-I../engine/common \
	-I../engine/server \
	-I../engine/client \
	-I../public \
	-I../common \
	-I../pm_shared \
	-I../engine/platform/psp \
	\
	-I/usr/local/pspdev/psp/sdk/include/

CC := psp-gcc
AR := psp-ar cru
RANLIB := psp-ranlib
CFILES := $(foreach dir,$(SOURCE_DIRS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.c))))
OBJS = $(CFILES:.c=.o)
INCDIR = $(INCLUDES)
LIBS = -lc -lgl -lglu -lpspvfpu -lpsprtc -lm -lpspuser
CFLAGS = -DXASH_GL_STATIC -DREF_DLL -D__PSP__ -g -G0 -Os

all: $(TARGET)

libref_gl.a: ${OBJS}
	${AR} $@ ${OBJS}
	${RANLIB} $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCDIR} $< -o $@

clean:
	-rm -f ${OBJS} ${TARGET}