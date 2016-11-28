#include <string.h>
#include <stdio.h>
#include "new.h"
#include "int.h"

#define S_BUF   128

typedef struct
{
  Class base;
  int	i;
  char  buf[S_BUF];
} IntClass;

static void     int_ctor(Object *self, va_list *args)
{
  ((IntClass *)self)->i = va_arg(*args, int);
  memset(((IntClass *)self)->buf, 0, S_BUF);
  snprintf(((IntClass *)self)->buf, S_BUF, "<%s (%d)>",
           ((Class *)self)->__name__, ((IntClass *)self)->i);
}

static void     int_dtor(Object *self)
{
  ((IntClass *)self)->i = 0;
}

static const char       *int_to_string(Object *self)
{
  return ((IntClass *)self)->buf;
}

static Object           *int_add(const Object *self, const Object *other)
{
  return new(Int, ((IntClass *)self)->i + ((IntClass *)other)->i);
}

static Object           *int_sub(const Object *self, const Object *other)
{
  return new(Int, ((IntClass *)self)->i - ((IntClass *)other)->i);
}

static Object           *int_mul(const Object *self, const Object *other)
{
  return new(Int, ((IntClass *)self)->i * ((IntClass *)other)->i);
}

static Object           *int_div(const Object *self, const Object *other)
{
  return ((IntClass *)other)->i != 0 ?
    new(Int, ((IntClass *)self)->i / ((IntClass *)other)->i) :
    NULL;
}

static bool             int_eq(const Object *self, const Object *other)
{
  return ((IntClass *)self)->i == ((IntClass *)other)->i;
}

static bool             int_gt(const Object *self, const Object *other)
{
  return ((IntClass *)self)->i > ((IntClass *)other)->i;
}

static bool             int_it(const Object *self, const Object *other)
{
  return ((IntClass *)self)->i < ((IntClass *)other)->i;
}

static IntClass       _description =
  {
    { sizeof(_description), "Int",
      &int_ctor, &int_dtor, &int_to_string,
      &int_add, &int_sub, &int_mul, &int_div,
      &int_eq, &int_gt, &int_it },
    0, ""
  };

Class   *Int = (Class *)&_description;
