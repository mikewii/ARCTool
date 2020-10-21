.SUFFIXES:

CXX		:= x86_64-w64-mingw32-g++

TOPDIR 		?= $(CURDIR)

TARGET		:= arctool

BUILD		:= Build
INCLUDES	:= include
SOURCES		:= src
LIBS		:= -lz -lstdc++fs -static-libstdc++ -static-libgcc

CFLAGS		:= $(INCLUDE) -std=gnu++17
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions 
LDFLAGS		:= -Os -static -Wl,--gc-sections,--strip-discarded,--strip-debug

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

export LD 	:= 	$(CXX)
export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I $(CURDIR)/$(dir) ) \
					$(foreach dir,$(LIBDIRS),-I $(dir)/include) \
					-I $(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L $(dir)/Lib)

.PHONY: $(BUILD) clean all


all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ... 
	@rm -fr $(BUILD) $(OUTPUT).exe

re: clean all

else

DEPENDS	:=	$(OFILES:.o=.d)

$(OUTPUT).exe : $(OFILES)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.exe:
	$(CXX) $(OFILES) $(LIBS) -o $(OUTPUT)
	@echo made $(notdir $@)

-include $(DEPENDS)

endif