#include <string.h>
#include <stdio.h>
#include "new.h"
#include "vertex.h"

#define S_BUF	128

typedef struct
{
  Class	base;
  int	x, y, z;
  char	buf[S_BUF];
} VertexClass;

static void	vertex_ctor(Object *self, va_list *args)
{
  ((VertexClass *)self)->x = va_arg(*args, int);
  ((VertexClass *)self)->y = va_arg(*args, int);
  ((VertexClass *)self)->z = va_arg(*args, int);
  memset(((VertexClass *)self)->buf, 0, S_BUF);
  snprintf(((VertexClass *)self)->buf, S_BUF, "<%s (%d, %d, %d)>",
	   ((Class *)self)->__name__, ((VertexClass *)self)->x,
	   ((VertexClass *)self)->y, ((VertexClass *)self)->z);
}

static void	vertex_dtor(Object *self)
{
  ((VertexClass *)self)->x = 0;
  ((VertexClass *)self)->y = 0;
  ((VertexClass *)self)->z = 0;
}

static const char	*vertex_to_string(Object *self)
{
  return ((VertexClass *)self)->buf;
}

static Object		*vertex_add(const Object *self, const Object *other)
{
  return new(Vertex, ((VertexClass *)self)->x + ((VertexClass *)other)->x,
	     ((VertexClass *)self)->y + ((VertexClass *)other)->y,
	     ((VertexClass *)self)->z + ((VertexClass *)other)->z);
}

static Object		*vertex_sub(const Object *self, const Object *other)
{
  return new(Vertex, ((VertexClass *)self)->x - ((VertexClass *)other)->x,
	     ((VertexClass *)self)->y - ((VertexClass *)other)->y,
	     ((VertexClass *)self)->z - ((VertexClass *)other)->z);
}

static VertexClass	_description =
  {
    { sizeof(_description), "Vertex",
      &vertex_ctor, &vertex_dtor, &vertex_to_string,
      &vertex_add, &vertex_sub },
    0, 0, 0, ""
  };

Class	*Vertex = (Class *)&_description;
