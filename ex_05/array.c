
#include <stdlib.h>
#include <stdarg.h>

#include "raise.h"
#include "array.h"
#include "new.h"

typedef struct
{
    Container base;
    Class* _type;
    size_t _size;
    Object** _tab;
} ArrayClass;

typedef struct {
    Iterator base;
    ArrayClass* _array;
    size_t _idx;
} ArrayIteratorClass;

void ArrayIterator_ctor(ArrayIteratorClass* self, va_list* args)
{
  self->_array = va_arg(*args, ArrayClass *);
  self->_idx = 0;
}

bool ArrayIterator_eq(ArrayIteratorClass* self, ArrayIteratorClass* other)
{
  return self->_idx == other->_idx;
}

bool ArrayIterator_gt(ArrayIteratorClass* self, ArrayIteratorClass* other)
{
  return self->_idx > other->_idx;
}

bool ArrayIterator_lt(ArrayIteratorClass* self, ArrayIteratorClass* other)
{
  return self->_idx < other->_idx;
}

void ArrayIterator_incr(ArrayIteratorClass* self)
{
  ++self->_idx;
}

Object* ArrayIterator_getval(ArrayIteratorClass* self)
{
  return self->_array->_tab[self->_idx];
}

void ArrayIterator_setval(ArrayIteratorClass* self, ...)
{
  if (self->_idx < self->_array->_size)
    {
      va_list	ap;
      
      va_start(ap, self);

      delete(self->_array->_tab[self->_idx]);
      self->_array->_tab[self->_idx] = va_new(self->_array->_type, &ap);

      va_end(ap);
    }
}

static ArrayIteratorClass ArrayIteratorDescr = {
    {
        {
            sizeof(ArrayIteratorClass), "ArrayIterator",
            (ctor_t) &ArrayIterator_ctor,
            NULL, /* dtor */
            NULL, /* str */
            NULL, NULL, NULL, NULL, /* add, sub, mul, div */
            (binary_comparator_t) &ArrayIterator_eq,
            (binary_comparator_t) &ArrayIterator_gt,
            (binary_comparator_t) &ArrayIterator_lt,
        },
        (incr_t) &ArrayIterator_incr,
        (getval_t) &ArrayIterator_getval,
        (setval_t) &ArrayIterator_setval,
    },
    NULL,
    0
};

static Class* ArrayIterator = (Class*) &ArrayIteratorDescr;

void Array_ctor(ArrayClass* self, va_list* args)
{
  self->_size = va_arg(*args, size_t);
  self->_type = va_arg(*args, Class *);
  self->_tab = malloc(sizeof(*self->_tab) * (1 + self->_size));

  size_t	i = 0;

  while (i < self->_size)
    {
      va_list	cpy;

      va_copy(cpy, *args);
      self->_tab[i++] = va_new(self->_type, &cpy);
    }
  self->_tab[self->_size] = NULL;
}

void Array_dtor(ArrayClass* self)
{
  if (self->_tab)
    {
      while (*self->_tab)
	{
	  Object	*obj = *self->_tab;
	 
	  ++self->_tab;
	  delete(obj);
	}
      self->_tab = NULL;
    }
}

size_t Array_len(ArrayClass* self)
{
  if (!self->_tab)
    return 0;
  
  size_t	len = 0;

  while (self->_tab[len])
    ++len;
  return len;
}

Iterator* Array_begin(ArrayClass* self)
{
  return new(ArrayIterator, self);
}

Iterator* Array_end(ArrayClass* self)
{
  ArrayIteratorClass	*it = new(ArrayIterator, self);

  it->_idx = self->_size;
  return (Iterator *)it;
}

Object* Array_getitem(ArrayClass* self, ...)
{
  if (!self->_tab)
    return NULL;

  va_list	ap;

  va_start(ap, self);
  
  size_t	index = va_arg(ap, size_t);

  va_end(ap);

  return index < self->_size ? self->_tab[index] : NULL;
}


void Array_setitem(ArrayClass* self, ...)
{
  va_list	ap;

  va_start(ap, self);

  size_t	index = va_arg(ap, size_t);

  if (index < self->_size)
    {
      delete(self->_tab[index]);
      self->_tab[index] = va_new(self->_type, &ap);
    }
  va_end(ap);
}

static ArrayClass _descr = {
    { /* Container */
        { /* Class */
            sizeof(ArrayClass), "Array",
            (ctor_t) &Array_ctor, (dtor_t) &Array_dtor,
            NULL, /*str */
            NULL, NULL, NULL, NULL, /* add, sub, mul, div */
            NULL, NULL, NULL, /* eq, gt, lt */
        },
        (len_t) &Array_len,
        (iter_t) &Array_begin,
        (iter_t) &Array_end,
        (getitem_t) &Array_getitem,
        (setitem_t) &Array_setitem,
    },
    NULL, 0, NULL
};

Class* Array = (Class*) &_descr;
