BIN := wheel
CPPFLAGS := -std=c++11
CXXFLAGS := -pedantic -Wall -Wextra -Weffc++

OBJS := $(shell if [ -e objs.mk ] ; then echo exists ; fi)
ifdef OBJS
	include objs.mk
endif

objs.mk: cxxs := $(wildcard *.cpp)
objs.mk:
	$(eval OBJS := $(subst .cpp,.o,${cxxs}))
	echo "OBJS := ${OBJS}" > objs.mk
	echo "${BIN}: CC := ${CXX}" >> objs.mk
	echo "${BIN}: ${OBJS}" >> objs.mk
	${CXX} ${CPPFLAGS} -MM ${cxxs} >> objs.mk
	${MAKE}

clean:
	rm -f ${OBJS}
wipe: clean
	rm -f ${BIN} objs.mk
