#include <string.h>
#include <stdio.h>
#include "new.h"
#include "float.h"

#define S_BUF	128

typedef struct
{
  Class base;
  float	f;
  char	buf[S_BUF];
} FloatClass;

static void	float_ctor(Object *self, va_list *args)
{
  ((FloatClass *)self)->f = (float)(va_arg(*args, double));
  memset(((FloatClass *)self)->buf, 0, S_BUF);
  snprintf(((FloatClass *)self)->buf, S_BUF, "<%s (%f)>",
	   ((Class *)self)->__name__, ((FloatClass *)self)->f);
}

static void	float_dtor(Object *self)
{
  ((FloatClass *)self)-> f = 0;
}

static const char	*float_to_string(Object *self)
{
  return ((FloatClass *)self)->buf;
}

static Object		*float_add(const Object *self, const Object *other)
{
  return new(Float, ((FloatClass *)self)->f + ((FloatClass *)other)->f);
}

static Object		*float_sub(const Object *self, const Object *other)
{
  return new(Float, ((FloatClass *)self)->f - ((FloatClass *)other)->f);
}

static Object		*float_mul(const Object *self, const Object *other)
{
  return new(Float, ((FloatClass *)self)->f * ((FloatClass *)other)->f);
}

static Object		*float_div(const Object *self, const Object *other)
{
  return ((FloatClass *)other)->f != 0 ?
    new(Float, ((FloatClass *)self)->f / ((FloatClass *)other)->f) :
    NULL;
}

static bool		float_eq(const Object *self, const Object *other)
{
  return ((FloatClass *)self)->f == ((FloatClass *)other)->f;
}

static bool		float_gt(const Object *self, const Object *other)
{
  return ((FloatClass *)self)->f > ((FloatClass *)other)->f;
}

static bool		float_it(const Object *self, const Object *other)
{
  return ((FloatClass *)self)->f < ((FloatClass *)other)->f;
}

static FloatClass	_description =
  {
    { sizeof(_description), "Float",
      &float_ctor, &float_dtor, &float_to_string,
      &float_add, &float_sub, &float_mul, &float_div,
      &float_eq, &float_gt, &float_it },
    0, ""
  };

Class	*Float = (Class *)&_description;
