BINS := wheel blackbox
CPPFLAGS := -std=c++11
CXXFLAGS := -pedantic -Wall -Wextra -Weffc++

OBJS := $(shell if [ -e objs.mk ] ; then echo exists ; fi)
ifdef OBJS
	include objs.mk
endif

objs.mk: cxxs := $(filter-out $(foreach bin,${BINS},${bin}.cpp),$(wildcard *.cpp))
objs.mk:
	$(eval OBJS := $(subst .cpp,.o,${cxxs}))
	$(shell echo "all: ${BINS}" > objs.mk)
	$(foreach bin,${BINS},$(shell echo "${bin}: ${OBJS}" >> objs.mk))
	${CXX} ${CPPFLAGS} -MM ${cxxs} >> objs.mk
	echo "OBJS := ${OBJS}" >> objs.mk
	${MAKE}

clean:
	rm -f ${OBJS}
wipe: clean
	rm -f ${BINS} objs.mk
