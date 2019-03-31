all: SBEMVIEWER MOVIEMAKER

clean:
	+rm -rf build buildmm

SBEMVIEWER: PREP
	+make -C build

debug: PREP
	+make -C build debug

PREP:
	mkdir -p build
	( cd build; qmake ../src/sbemviewer.pro )

MOVIEMAKER: PREPMM
	+make -C buildmm
PREPMM:
	mkdir -p buildmm
	( cd buildmm; qmake ../moviemaker/moviemaker.pro )

.PHONY: PREP SBEMVIEWER PREPMM MOVIEMAKER clean
