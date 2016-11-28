#include <string.h>
#include <stdio.h>
#include "new.h"
#include "char.h"

#define S_BUF   128

typedef struct
{
  Class base;
  char	c;
  char  buf[S_BUF];
} CharClass;

static void     char_ctor(Object *self, va_list *args)
{
  ((CharClass *)self)->c = va_arg(*args, bool);
  memset(((CharClass *)self)->buf, 0, S_BUF);
  snprintf(((CharClass *)self)->buf, S_BUF, "<%s (%d)>",
           ((Class *)self)->__name__, ((CharClass *)self)->c);
}

static void     char_dtor(Object *self)
{
  ((CharClass *)self)->c = 0;
}

static const char       *char_to_string(Object *self)
{
  return ((CharClass *)self)->buf;
}

static Object           *char_add(const Object *self, const Object *other)
{
  return new(Char, ((CharClass *)self)->c + ((CharClass *)other)->c);
}

static Object           *char_sub(const Object *self, const Object *other)
{
  return new(Char, ((CharClass *)self)->c - ((CharClass *)other)->c);
}

static Object           *char_mul(const Object *self, const Object *other)
{
  return new(Char, ((CharClass *)self)->c * ((CharClass *)other)->c);
}

static Object           *char_div(const Object *self, const Object *other)
{
  return ((CharClass *)other)->c != 0 ?
    new(Char, ((CharClass *)self)->c / ((CharClass *)other)->c) :
    NULL;
}

static bool             char_eq(const Object *self, const Object *other)
{
  return ((CharClass *)self)->c == ((CharClass *)other)->c;
}

static bool             char_gt(const Object *self, const Object *other)
{
  return ((CharClass *)self)->c > ((CharClass *)other)->c;
}

static bool             char_it(const Object *self, const Object *other)
{
  return ((CharClass *)self)->c < ((CharClass *)other)->c;
}

static CharClass       _description =
  {
    { sizeof(_description), "Char",
      &char_ctor, &char_dtor, char_to_string,
      &char_add, &char_sub, &char_mul, &char_div,
      &char_eq, &char_gt, &char_it },
    0, ""
  };

Class   *Char = (Class *)&_description;
