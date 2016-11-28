#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "raise.h"
#include "list.h"
#include "new.h"

typedef struct Node	Node;

struct Node
{
  Object	*_obj;
  Node		*_next;
};

typedef struct
{
  Container	base;
  Class		*_type;

  struct
  {
    size_t	_size;
    Node	*_begin;
    Node	*_end;
  } _control_list;

} ListClass;

typedef struct
{
  Iterator	base;
  ListClass	*_list;
  Node		*_node;
  size_t	_idx;
} ListIteratorClass;

void	ListIterator_ctor(ListIteratorClass *self, va_list *args)
{
  self->_list = va_arg(*args, ListClass *);
  self->_node = self->_list->_control_list._begin;
  self->_idx = 0;
}

bool	ListIterator_eq(const ListIteratorClass *self, const ListIteratorClass *other)
{
  return self->_idx == other->_idx;
}

bool	ListIterator_gt(const ListIteratorClass *self, const ListIteratorClass *other)
{
  return self->_idx > other->_idx;
}

bool	ListIterator_lt(const ListIteratorClass *self, const ListIteratorClass *other)
{
  return self->_idx < other->_idx;
}

void	ListIterator_incr(ListIteratorClass *self)
{
  self->_node = self->_node->_next;
  ++self->_idx;
}

Object	*ListIterator_getval(ListIteratorClass *self)
{
  return self->_node->_obj;
}

void	ListIterator_setval(ListIteratorClass *self, ...)
{
  if (self->_idx < self->_list->_control_list._size)
    {
      va_list	ap;

      va_start(ap, self);

      delete(self->_node->_obj);
      self->_node->_obj = va_new(self->_list->_type, &ap);

      va_end(ap);
    }
}

static ListIteratorClass	ListIteratorDescr =
  {
    {
      {
	sizeof(ListIteratorClass), "ListIterator",
	(ctor_t)&ListIterator_ctor,
	NULL,
	NULL,
	NULL, NULL, NULL, NULL,
	(binary_comparator_t)&ListIterator_eq,
	(binary_comparator_t)&ListIterator_gt,
	(binary_comparator_t)&ListIterator_lt
      },
      (incr_t)&ListIterator_incr,
      (getval_t)&ListIterator_getval,
      (setval_t)&ListIterator_setval,
    },
    NULL,
    NULL,
    0
  };

static Class	*ListIterator = (Class *)&ListIteratorDescr;

static void	add_to_the_end_of_list(ListClass *, va_list *, bool);
static void	fill_self_with_other(ListClass *, const ListClass *);
static void	prepare_to_call_add_end_list(ListClass *, ...);

static void	add_to_the_end_of_list(ListClass *self, va_list *ap, bool opt)
{
  Node	*node = malloc(sizeof(*node));

  if (!node)
    raise("Out of memory");
  if (!opt)
    node->_obj = va_new(self->_type, ap);
  else
    {
      node->_obj = new(self->_type, 0);
      memcpy(node->_obj, va_arg(*ap, Class *), self->_type->__size__);
    }
  node->_next = NULL;
  if (!self->_control_list._end)
    {
      self->_control_list._begin = node;
      self->_control_list._end = node;
    }
  else
    {
      self->_control_list._end->_next = node;
      self->_control_list._end = self->_control_list._end->_next;
    }
}

void	List_ctor(ListClass *self, va_list *args)
{
  self->_control_list._size = va_arg(*args, size_t);
  self->_type = va_arg(*args, Class *);
  
  size_t	i = 0;

  while (i < self->_control_list._size)
    {
      va_list	cpy;

      va_copy(cpy, *args);
      add_to_the_end_of_list(self, &cpy, 0);
      ++i;
    }
}

void	List_dtor(ListClass *self)
{
  if (self->_control_list._begin)
    {
      while (self->_control_list._begin)
	{
	  Node	*tmp = self->_control_list._begin;

	  self->_control_list._begin = self->_control_list._begin->_next;
	  delete(tmp->_obj);
	  delete(tmp);
	}
    }
}

static void	prepare_to_call_add_end_list(ListClass *self, ...)
{
  va_list	ap;

  va_start(ap, self);
  add_to_the_end_of_list(self, &ap, 1);
  va_end(ap);
}

static void	fill_self_with_other(ListClass *self, const ListClass *other)
{
  if (other->_control_list._begin)
    {
      const Node	*node = other->_control_list._begin;

      while (node)
	{
	  prepare_to_call_add_end_list(self, node->_obj);
	  node = node->_next;
	}
    }
}

ListClass	*List_add(const ListClass *self, const ListClass *other)
{
  if (self->_type != other->_type)
    return NULL;  

  ListClass	*list = new(List, 0, self->_type);

  fill_self_with_other(list, self);
  fill_self_with_other(list, other);
  return list;
}

size_t	List_len(ListClass *self)
{
  return self->_control_list._size;
}

Iterator	*List_begin(ListClass *self)
{
  return new(ListIterator, self);
}

Iterator	*List_end(ListClass *self)
{
  ListIteratorClass	*it = new(ListIterator, self);

  it->_idx = self->_control_list._size;
  it->_node = self->_control_list._end;
  return (Iterator *)it;
}

Object	*List_getitem(ListClass *self, ...)
{
  if (!self->_control_list._begin)
    return NULL;

  va_list	ap;

  va_start(ap, self);

  size_t	index = va_arg(ap, size_t);

  va_end(ap);

  if (index >= self->_control_list._size)
    return NULL;

  Node		*node = self->_control_list._begin;
  size_t	pos = 0;
  
  while (node && pos < index)
    {
      node = node->_next;
      ++pos;
    }
  return node;
}

void	List_setitem(ListClass *self, ...)
{
  va_list	ap;

  va_start(ap, self);

  size_t	index = va_arg(ap, size_t);

  if (index < self->_control_list._size)
    {
      Node	*node = self->_control_list._begin;
      size_t	pos = 0;

      while (node && pos < index)
	{
	  node = node->_next;
	  ++pos;
	}
      delete(node->_obj);
      node->_obj = va_new(self->_type, &ap);
    }
  va_end(ap);
}

static ListClass	_descr =
  {
    {
      {
	sizeof(ListClass), "List",
	(ctor_t)&List_ctor, (dtor_t)&List_dtor,
	NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL,
      },
      (len_t)&List_len,
      (iter_t)&List_begin,
      (iter_t)&List_end,
      (getitem_t)&List_getitem,
      (setitem_t)&List_setitem,
      (binary_cont_comparator_t)&List_add,
    },
    NULL,
    {
      0, 0, NULL
    }
  };

Class	*List = (Class *)&_descr;
