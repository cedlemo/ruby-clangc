
/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015  cedlemo <cedlemo@gmx.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*CompletionString ruby class*/
#include "class_CompletionString.h"
#include "macros.h"

static void
c_CompletionString_struct_free(CompletionString_t *s)
{
  if(s)
  {
    ruby_xfree(s);
  }
}  

static void
c_CompletionString_mark(void *s)
{
  if(s)
  {
    CompletionString_t *t =(CompletionString_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_CompletionString_struct_alloc( VALUE klass)
{
  
  CompletionString_t * ptr;
  ptr = (CompletionString_t *) ruby_xmalloc(sizeof(CompletionString_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_CompletionString_struct_free, (void *) ptr);
}

/**
 * Clangc::CompletionString#availability => Clangc::AvailabilityKind
 *
 * Determine the availability of the entity that this code-completion
 * string refers to.
 *
 * The availability of the completion string which is one of the 
 * Clangc::AvailabilityKind constants.
 */
VALUE
c_CompletionString_get_availability(VALUE self)
{
  CompletionString_t *c;
  Data_Get_Struct(self, CompletionString_t, c);

  return CUINT_2_NUM(clang_getCompletionAvailability(c->data));
}
