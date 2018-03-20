all: SBEMVIEWER

clean:
	+rm -rf build

SBEMVIEWER: PREP
	+make -C build

PREP:
	mkdir -p build
	( cd build; qmake ../src/sbemviewer.pro )

.PHONY: PREP SBEMVIEWER clean
