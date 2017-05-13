# The MIT License (MIT)
#
# Copyright (c) 2014-2017 David Medina and Tim Warburton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

rmSlash = $(patsubst %/,%,$1)

PROJ_DIR := $(call rmSlash,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
OCCA_DIR := $(PROJ_DIR)/submodules/occa

include $(OCCA_DIR)/scripts/Makefile

pyflags = -I${OCCA_PYTHON_DIR}/ -I${OCCA_NUMPY_DIR} -L${OCCA_LIBPYTHON_DIR} -l${OCCA_LIBPYTHON}

all: $(PROJ_DIR)/occa/c.so

$(PROJ_DIR)/occa/c.so: $(incPath)/_C_occa.h $(srcPath)/_C_occa.c
	$(compiler) $(compilerFlags) $(sharedFlag) $(srcPath)/_C_occa.c -o $@ -I$(incPath) -L$(OCCA_DIR)/lib/libocca.so -locca $(pyFlags)

#---[ CLEAN ]---------------------------
clean:
	rm -rf $(objPath)/*
	rm -rf $(binPath)/*
	rm  -f $(libPath)/libocca.so
	rm  -f $(libPath)/*.mod
	rm  -f $(libPath)/_C_occa.so
	rm  -f $(OCCA_DIR)/scripts/main