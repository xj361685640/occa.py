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
  occa::memory *memory;
} Memory;

static PyObject* Memory_new(PyTypeObject *type,
                            PyObject *args,
                            PyObject *kwargs) {
  Memory *self = (Memory*) type->tp_alloc(type, 0);

  self->memory = NULL;

  PyObject *memoryPtr = NULL;
  if (!PyArg_ParseTuple(args, "|O", &memoryPtr)) {
    return NULL;
  }
  if (memoryPtr) {
    OCCA_TRY(
      self->memory = new occa::memory(
        (occa::modeMemory_t*) occa::py::ptr(memoryPtr)
      );
    );
  }

  return (PyObject*) self;
}

static void Memory_dealloc(Memory *self) {
  delete self->memory;
  Py_TYPE(self)->tp_free((PyObject*) self);
}
//======================================


//---[ Class Methods ]------------------
static PyObject* Memory_is_initialized(Memory *self) {
  return occa::py::toPy(
    (bool) (self->memory &&
            self->memory->isInitialized())
  );
}

static PyObject* Memory_get_device(Memory *self) {
  if (!self->memory) {
    return occa::py::None();
  }
  return occa::py::toPy(self->memory->getDevice());
}

static PyObject* Memory_properties(Memory *self) {
  if (!self->memory) {
    return occa::py::None();
  }
  return occa::py::toPy(self->memory->properties());
}

static PyObject* Memory_size(Memory *self) {
  if (!self->memory) {
    return occa::py::None();
  }
  return occa::py::toPy((long long) self->memory->size());
}

static PyObject* Memory_slice(Memory *self,
                              PyObject *args,
                              PyObject *kwargs) {
  static const char *kwargNames[] = {"offset", "bytes", NULL};
  if (!self->memory) {
    return occa::py::None();
  }

  long long offset = -1;
  long long bytes = -1;
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|$ii", (char**) kwargNames,
                                   &offset, &bytes)) {
    return NULL;
  }

  return occa::py::toPy(self->memory->slice(offset, bytes));
}

//---[ UVA ]----------------------------
static PyObject* Memory_is_managed(Memory *self) {
  return occa::py::toPy(
    (bool) (self->memory &&
            self->memory->isManaged())
  );
}

static PyObject* Memory_in_device(Memory *self) {
  return occa::py::toPy(
    (bool) (self->memory &&
            self->memory->inDevice())
  );
}

static PyObject* Memory_is_stale(Memory *self) {
  return occa::py::toPy(
    (bool) (self->memory &&
            self->memory->isStale())
  );
}

static PyObject* Memory_start_managing(Memory *self) {
  if (self->memory) {
    self->memory->startManaging();
  }
  return occa::py::None();
}

static PyObject* Memory_stop_managing(Memory *self) {
  if (self->memory) {
    self->memory->stopManaging();
  }
  return occa::py::None();
}

static PyObject* Memory_sync_to_device(Memory *self,
                                       PyObject *args,
                                       PyObject *kwargs) {
  static const char *kwargNames[] = {"bytes", "offset", NULL};

  if (self->memory) {
    long long bytes = -1;
    long long offset = -1;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|$ii", (char**) kwargNames,
                                     &bytes, &offset)) {
      return NULL;
    }
    self->memory->syncToDevice(bytes, offset);
  }

  return occa::py::None();
}

static PyObject* Memory_sync_to_host(Memory *self,
                                     PyObject *args,
                                     PyObject *kwargs) {
  static const char *kwargNames[] = {"bytes", "offset", NULL};

  if (self->memory) {
    long long bytes = -1;
    long long offset = -1;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|$ii", (char**) kwargNames,
                                     &bytes, &offset)) {
      return NULL;
    }
    self->memory->syncToHost(bytes, offset);
  }

  return occa::py::None();
}
//======================================

static PyObject* Memory_memcpy(Memory *self,
                               PyObject *args,
                               PyObject *kwargs) {
  return occa::py::None();
}

static PyObject* Memory_copy_mem_to_mem(Memory *self,
                                        PyObject *args,
                                        PyObject *kwargs) {
  return occa::py::None();
}

static PyObject* Memory_copy_ptr_to_mem(Memory *self,
                                        PyObject *args,
                                        PyObject *kwargs) {
  return occa::py::None();
}

static PyObject* Memory_copy_mem_to_ptr(Memory *self,
                                        PyObject *args,
                                        PyObject *kwargs) {
  return occa::py::None();
}

static PyObject* Memory_clone(Memory *self) {
  if (!self->memory) {
    return occa::py::None();
  }
  return occa::py::toPy(self->memory->clone());
}
//======================================


//---[ Module ]-------------------------
#define MEMORY_METHOD_NO_ARGS(FUNC)             \
  OCCA_PY_METHOD_NO_ARGS(#FUNC, Memory_##FUNC)

#define MEMORY_METHOD_WITH_KWARGS(FUNC)             \
  OCCA_PY_METHOD_WITH_KWARGS(#FUNC, Memory_##FUNC)

OCCA_PY_METHODS(
  Memory_methods,
  MEMORY_METHOD_NO_ARGS(is_initialized),
  MEMORY_METHOD_NO_ARGS(get_device),
  MEMORY_METHOD_NO_ARGS(properties),
  MEMORY_METHOD_NO_ARGS(size),
  MEMORY_METHOD_WITH_KWARGS(slice),
  MEMORY_METHOD_NO_ARGS(is_managed),
  MEMORY_METHOD_NO_ARGS(in_device),
  MEMORY_METHOD_NO_ARGS(is_stale),
  MEMORY_METHOD_NO_ARGS(start_managing),
  MEMORY_METHOD_NO_ARGS(stop_managing),
  MEMORY_METHOD_WITH_KWARGS(sync_to_device),
  MEMORY_METHOD_WITH_KWARGS(sync_to_host),
  MEMORY_METHOD_WITH_KWARGS(memcpy),
  MEMORY_METHOD_WITH_KWARGS(copy_mem_to_mem),
  MEMORY_METHOD_WITH_KWARGS(copy_ptr_to_mem),
  MEMORY_METHOD_WITH_KWARGS(copy_mem_to_ptr),
  MEMORY_METHOD_NO_ARGS(clone)
);

static PyTypeObject MemoryType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "occa.c.Memory",             // tp_name
  sizeof(Memory),              // tp_basicsize
  0,                           // tp_itemsize
  (destructor) Memory_dealloc, // tp_dealloc
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
  "Wrapper for occa::memory",  // tp_doc
  0,                           // tp_traverse
  0,                           // tp_clear
  0,                           // tp_richcompare
  0,                           // tp_weaklistoffset
  0,                           // tp_iter
  0,                           // tp_iternext
  Memory_methods,              // tp_methods
  0,                           // tp_members
  0,                           // tp_getset
  0,                           // tp_base
  0,                           // tp_dict
  0,                           // tp_descr_get
  0,                           // tp_descr_set
  0,                           // tp_dictoffset
  0,                           // tp_init
  0,                           // tp_alloc
  Memory_new,                  // tp_new
};

static bool memory_has_valid_module() {
  return PyType_Ready(&MemoryType) >= 0;
}

static void memory_init_module(PyObject *module) {
  Py_INCREF(&MemoryType);
  PyModule_AddObject(module,
                     "Memory",
                     (PyObject*) &MemoryType);
}

OCCA_PY_MODULE(memory, OCCA_PY_NO_METHODS)
//======================================
