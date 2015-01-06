CFLAGS=-Wall -Wextra -std=c++11 -DSFML_STATIC

FILES= $(wildcard src/Common/*.cpp)
FILES+=$(wildcard src/Common/*/*.cpp)
FILES+=$(wildcard src/Common/*/*/*.cpp)

TARGET_CFRT_VIEW=cfrt_view
 FILES_CFRT_VIEW=$(FILES) src/cfrt_view.cpp
  OBJS_CFRT_VIEW=$(patsubst %,build/%.o,$(basename $(FILES_CFRT_VIEW:src/%=%)))
LFLAGS_CFRT_VIEW=-static -mwindows \
                 -lsfml-graphics-s -lsfml-window-s -lsfml-system-s \
                 -ljpeg -lglew32 -lfreetype -lzlibstatic \
                 -lgdi32 -lopengl32 -lwinmm

TARGET_CFRT_CONVERT=cfrt_convert
 FILES_CFRT_CONVERT=$(FILES) src/cfrt_convert.cpp
  OBJS_CFRT_CONVERT=$(patsubst %,build/%.o,$(basename $(FILES_CFRT_CONVERT:src/%=%)))
LFLAGS_CFRT_CONVERT=-static -lFreeImage

TARGET_OBJ_CONVERT=obj_convert
 FILES_OBJ_CONVERT=$(FILES) src/obj_convert.cpp
  OBJS_OBJ_CONVERT=$(patsubst %,build/%.o,$(basename $(FILES_OBJ_CONVERT:src/%=%)))
LFLAGS_OBJ_CONVERT=-static

TARGETS=$(TARGET_CFRT_VIEW) $(TARGET_CFRT_CONVERT) $(TARGET_OBJ_CONVERT)
OBJS=$(OBJS_CFRT_VIEW) $(OBJS_CFRT_CONVERT)

.PHONY: all clean
all: $(TARGETS)
$(TARGET_CFRT_VIEW): $(OBJS_CFRT_VIEW)
	@echo "Linking "$@
	@g++ $^ $(LFLAGS_CFRT_VIEW) -o $@
$(TARGET_CFRT_CONVERT): $(OBJS_CFRT_CONVERT)
	@echo "Linking "$@
	@g++ $^ $(LFLAGS_CFRT_CONVERT) -o $@
$(TARGET_OBJ_CONVERT): $(OBJS_OBJ_CONVERT)
	@echo "Linking "$@
	@g++ $^ $(LFLAGS_OBJ_CONVERT) -o $@
build/%.o: src/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@g++ -c $< $(CFLAGS) -o $@
	@g++ $< $(CFLAGS) -MT $@ -MM -MF build/$*.d
build/%.o: src/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@g++ -c $< $(CFLAGS) -o $@
	@g++ $< $(CFLAGS) -MT $@ -MM -MF build/$*.d
-include $(OBJS:.o=.d)
%.hpp %.h %.cpp %.c:
clean:
	@rm -rf *.o *.exe $(TARGETS) build/
	@echo "Cleaned."
