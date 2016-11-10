CONFIG ?= config
include $(CONFIG).mk

PETSC_INC := ${PETSC}/include
PETSC_LIB := ${PETSC}/lib
SCOREC_INC := ${SCOREC}/include
SCOREC_LIB := ${SCOREC}/lib

PE_SRCS := $(wildcard *.cc)
PE_OBJS := ${PE_SRCS:.cc=.o}
PE_INCLUDE_DIRS := ${PETSC_INC} ${SCOREC_INC}
PE_LIBRARY_DIRS := ${PETSC_LIB} ${SCOREC_LIB}
PE_LIBRARIES := petsc pcu gmi mds apf apf_zoltan ma parma lion mth

CXXFLAGS += $(foreach includedir,$(PE_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(PE_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(PE_LIBRARIES),-l$(library))

.PHONY: all test clean

pe: $(PE_OBJS)
	$(CXX) $(CXXFLAGS) $(PE_OBJS) -o pe $(LDFLAGS)

clean:
	$(RM) pe
	$(RM) $(PE_OBJS)
