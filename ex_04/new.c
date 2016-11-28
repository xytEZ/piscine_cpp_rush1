#include <stdlib.h>
#include <string.h>
#include "raise.h"
#include "new.h"

Object		*new(Class *class, ...)
{
  va_list	ap;

  va_start(ap, class);

  Object	*obj = va_new(class, &ap);

  va_end(ap);
  return obj;
}

Object		*va_new(Class *class, va_list *ap)
{
  Object	*obj = malloc(class->__size__);

  if (!obj)
    raise("Out of memory");
  memcpy(obj, class, class->__size__);
  class->__init__(obj, ap);
  return obj;
}

void		delete(Object *ptr)
{
  if (ptr)
    {
      ((Class *)ptr)->__del__(ptr);
      free(ptr);
    }
}
