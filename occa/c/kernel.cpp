/* The MIT License (MIT)
 *
 * Copyright (c) 2018 David Medina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 */
#include "header.hpp"


//---[ Class Python Methods ]-----------
typedef struct {
  PyObject_HEAD
  occa::kernel *kernel;
} Kernel;

static PyObject* Kernel_new(PyTypeObject *type,
                            PyObject *args,
                            PyObject *kwargs) {
  Kernel *self = (Kernel*) type->tp_alloc(type, 0);

  self->kernel = NULL;

  PyObject *kernelPtr = NULL;
  if (!PyArg_ParseTuple(args, "|O", &kernelPtr)) {
    return NULL;
  }
  if (kernelPtr) {
    OCCA_TRY(
      self->kernel = new occa::kernel(
        (occa::modeKernel_t*) occa::py::ptr(kernelPtr)
      );
    );
  }

  return (PyObject*) self;
}

static void Kernel_dealloc(Kernel *self) {
  delete self->kernel;
  Py_TYPE(self)->tp_free((PyObject*) self);
}
//======================================


//---[ Class Methods ]------------------
static PyObject* Kernel_is_initialized(Kernel *self) {
  return occa::py::toPy(
    (bool) (self->kernel &&
            self->kernel->isInitialized())
  );
}

static PyObject* Kernel_properties(Kernel *self) {
  if (!self->kernel) {
    return occa::py::None();
  }
  return occa::py::toPy(self->kernel->properties());
}

static PyObject* Kernel_get_device(Kernel *self) {
  if (!self->kernel) {
    return occa::py::None();
  }
  return occa::py::toPy(self->kernel->getDevice());
}

static PyObject* Kernel_name(Kernel *self) {
  if (!self->kernel) {
    return occa::py::None();
  }
  return occa::py::toPy(self->kernel->name());
}

static PyObject* Kernel_source_filename(Kernel *self) {
  if (!self->kernel) {
    return occa::py::None();
  }
  return occa::py::toPy(self->kernel->sourceFilename());
}

static PyObject* Kernel_binary_filename(Kernel *self) {
  if (!self->kernel) {
    return occa::py::None();
  }
  return occa::py::toPy(self->kernel->binaryFilename());
}

static PyObject* Kernel_max_dims(Kernel *self) {
  return occa::py::None();
}

static PyObject* Kernel_max_outer_dims(Kernel *self) {
  return occa::py::None();
}

static PyObject* Kernel_max_inner_dims(Kernel *self) {
  return occa::py::None();
}

static PyObject* Kernel_set_run_dims(Kernel *self,
                                     PyObject *args,
                                     PyObject *kwargs) {
  return occa::py::None();
}

static PyObject* Kernel_run(Kernel *self,
                            PyObject *args,
                            PyObject *kwargs) {
  return occa::py::None();
}
//======================================


//---[ Module ]-------------------------
#define KERNEL_METHOD_NO_ARGS(FUNC)             \
  OCCA_PY_METHOD_NO_ARGS(#FUNC, Kernel_##FUNC)

#define KERNEL_METHOD_WITH_KWARGS(FUNC)             \
  OCCA_PY_METHOD_WITH_KWARGS(#FUNC, Kernel_##FUNC)

OCCA_PY_METHODS(
  Kernel_methods,
    KERNEL_METHOD_NO_ARGS(is_initialized),
    KERNEL_METHOD_NO_ARGS(properties),
    KERNEL_METHOD_NO_ARGS(get_device),
    KERNEL_METHOD_NO_ARGS(name),
    KERNEL_METHOD_NO_ARGS(source_filename),
    KERNEL_METHOD_NO_ARGS(binary_filename),
    KERNEL_METHOD_NO_ARGS(max_dims),
    KERNEL_METHOD_NO_ARGS(max_outer_dims),
    KERNEL_METHOD_NO_ARGS(max_inner_dims),
    KERNEL_METHOD_WITH_KWARGS(set_run_dims),
    KERNEL_METHOD_WITH_KWARGS(run)
);

static PyTypeObject KernelType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "occa.c.Kernel",             // tp_name
  sizeof(Kernel),              // tp_basicsize
  0,                           // tp_itemsize
  (destructor) Kernel_dealloc, // tp_dealloc
  0,                           // tp_print
  0,                           // tp_getattr
  0,                           // tp_setattr
  0,                           // tp_reserved
  0,                           // tp_repr
  0,                           // tp_as_number
  0,                           // tp_as_sequence
  0,                           // tp_as_mapping
  0,                           // tp_hash
  0,                           // tp_call
  0,                           // tp_str
  0,                           // tp_getattro
  0,                           // tp_setattro
  0,                           // tp_as_buffer
  Py_TPFLAGS_DEFAULT,          // tp_flags
  "Wrapper for occa::kernel",  // tp_doc
  0,                           // tp_traverse
  0,                           // tp_clear
  0,                           // tp_richcompare
  0,                           // tp_weaklistoffset
  0,                           // tp_iter
  0,                           // tp_iternext
  Kernel_methods,              // tp_methods
  0,                           // tp_members
  0,                           // tp_getset
  0,                           // tp_base
  0,                           // tp_dict
  0,                           // tp_descr_get
  0,                           // tp_descr_set
  0,                           // tp_dictoffset
  0,                           // tp_init
  0,                           // tp_alloc
  Kernel_new,                  // tp_new
};

static bool kernel_has_valid_module() {
  return PyType_Ready(&KernelType) >= 0;
}

static void kernel_init_module(PyObject *module) {
  Py_INCREF(&KernelType);
  PyModule_AddObject(module,
                     "Kernel",
                     (PyObject*) &KernelType);
}

OCCA_PY_MODULE(kernel, OCCA_PY_NO_METHODS)
//======================================
