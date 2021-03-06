/*
 *      This file is part of GPaste.
 *
 *      Copyright 2011-2012 Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
 *
 *      GPaste is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      GPaste is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with GPaste.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpaste-text-item-private.h"

G_DEFINE_TYPE (GPasteTextItem, g_paste_text_item, G_PASTE_TYPE_ITEM)

static gboolean
g_paste_text_item_equals (const GPasteItem *self,
                          const GPasteItem *other)
{
    g_return_val_if_fail (G_PASTE_IS_TEXT_ITEM (self), FALSE);

    return (G_PASTE_IS_TEXT_ITEM (other) &&
            G_PASTE_ITEM_CLASS (g_paste_text_item_parent_class)->equals (self, other));
}

static const gchar *
g_paste_text_item_get_kind (const GPasteItem *self)
{
    g_return_val_if_fail (G_PASTE_IS_TEXT_ITEM (self), NULL);

    return "Text";
}

static void
g_paste_text_item_class_init (GPasteTextItemClass *klass)
{
    GPasteItemClass *item_class = G_PASTE_ITEM_CLASS (klass);

    item_class->equals = g_paste_text_item_equals;
    item_class->get_kind = g_paste_text_item_get_kind;
}

static void
g_paste_text_item_init (GPasteTextItem *self G_GNUC_UNUSED)
{
}

/**
 * g_paste_text_item_new:
 * @text: the content of the desired #GPasteTextItem
 *
 * Create a new instance of #GPasteTextItem
 *
 * Returns: a newly allocated #GPasteTextItem
 *          free it with g_object_unref
 */
G_PASTE_VISIBLE GPasteTextItem *
g_paste_text_item_new (const gchar *text)
{
    g_return_val_if_fail (text != NULL, NULL);
    g_return_val_if_fail (g_utf8_validate (text, -1, NULL), NULL);

    return G_PASTE_TEXT_ITEM (g_paste_item_new (G_PASTE_TYPE_TEXT_ITEM, text));
}
