
all:
	cd src && $(MAKE)
test:
	cd src && $(MAKE) && $(MAKE) test
	cd test && $(MAKE)
	test/testmonit

.PHONY: all test clean
