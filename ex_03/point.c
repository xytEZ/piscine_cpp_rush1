#include <string.h>
#include <stdio.h>
#include "new.h"
#include "point.h"

#define S_BUF	128

typedef struct
{
  Class	base;
  int	x, y;
  char	buf[S_BUF];
} PointClass;

static void	point_ctor(Object *self, va_list *args)
{
  ((PointClass *)self)->x = va_arg(*args, int);
  ((PointClass *)self)->y = va_arg(*args, int);
  memset(((PointClass *)self)->buf, 0, S_BUF);
  snprintf(((PointClass *)self)->buf, S_BUF, "<%s (%d, %d)>",
	   ((Class *)self)->__name__, ((PointClass *)self)->x,
	   ((PointClass *)self)->y);
}

static void	point_dtor(Object *self)
{
  ((PointClass *)self)->x = 0;
  ((PointClass *)self)->y = 0;
}

static const char	*point_to_string(Object *self)
{
  return ((PointClass *)self)->buf;
}

static Object		*point_add(const Object *self, const Object *other)
{
  return new(Point, ((PointClass *)self)->x + ((PointClass *)other)->x,
	     ((PointClass *)self)->y + ((PointClass *)other)->y);
}

static Object		*point_sub(const Object *self, const Object *other)
{
  return new(Point, ((PointClass *)self)->x - ((PointClass *)other)->x,
	     ((PointClass *)self)->y - ((PointClass *)other)->y);
}

static PointClass _description =
  {
    { sizeof(_description), "Point",
      &point_ctor, &point_dtor, &point_to_string,
      &point_add, &point_sub},
    0, 0, ""
  };

Class	*Point = (Class *)&_description;
