BIN := wheel
CXXFLAGS := -std=c++11 -Wall -Wextra

OBJS := $(shell if [ -e objs.mk ] ; then echo exists ; fi)
ifdef OBJS
	include objs.mk
endif

objs: cxxs := $(wildcard *.cpp)
objs:
	$(eval OBJS := $(subst .cpp,.o,${cxxs}))
	echo "OBJS := ${OBJS}" > objs.mk
	echo "${BIN}: CC := ${CXX}" >> objs.mk
	echo "${BIN}: ${OBJS}" >> objs.mk
	${CXX} -MM ${cxxs} >> objs.mk
	${MAKE}

clean:
	rm -f ${OBJS}
wipe: clean
	rm -f ${BIN} objs.mk
