/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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
/*Cursor ruby class*/
#include "class_OverriddenCursor.h"
#include "macros.h"

static void c_OverriddenCursor_struct_free(OverriddenCursor_t *s)
{
    if (s)
    {
        if (s->ptr) clang_disposeOverriddenCursors(s->ptr);

        ruby_xfree(s);
    }
}

static void c_OverriddenCursor_mark(void *s)
{
    if (s)
    {
        OverriddenCursor_t *t = (OverriddenCursor_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_OverriddenCursor_struct_alloc(VALUE klass)
{

    OverriddenCursor_t *ptr;
    ptr = (OverriddenCursor_t *) ruby_xmalloc(sizeof(OverriddenCursor_t));
    ptr->parent = Qnil;

    return Data_Wrap_Struct(
        klass, NULL, c_OverriddenCursor_struct_free, (void *) ptr);
}
