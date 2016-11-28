#include <stdlib.h>
#include <string.h>
#include "raise.h"
#include "new.h"

void		*new(Class *class)
{
  Object	*obj = malloc(class->__size__);

  if (!obj)
    raise("Out of memory");
  memcpy(obj, class, class->__size__);
  class->__init__(obj);
  return obj;
}

void		delete(Object *ptr)
{
  if (ptr)
    {
      ((Class *)(ptr))->__del__(ptr);
      free(ptr);
    }
}
