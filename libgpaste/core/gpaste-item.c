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

#include "gpaste-item-private.h"

#define G_PASTE_ITEM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), G_PASTE_TYPE_ITEM, GPasteItemPrivate))

G_DEFINE_ABSTRACT_TYPE (GPasteItem, g_paste_item, G_TYPE_OBJECT)

struct _GPasteItemPrivate
{
    gchar *value;
    gchar *display_string;
};

/**
 * g_paste_item_get_value:
 * @self: a #GPasteItem instance
 *
 * Get the value of the given item (text, uris or path to the image)
 *
 * Returns: read-only string containing the value
 */
G_PASTE_VISIBLE const gchar *
g_paste_item_get_value (const GPasteItem *self)
{
    g_return_val_if_fail (G_PASTE_IS_ITEM (self), NULL);

    return self->priv->value;
}

/**
 * g_paste_item_get_display_string:
 * @self: a #GPasteItem instance
 *
 * Get the string we should use to display the #GPasteItem
 *
 * Returns: read-only display string
 */
G_PASTE_VISIBLE const gchar *
g_paste_item_get_display_string (const GPasteItem *self)
{
    g_return_val_if_fail (G_PASTE_IS_ITEM (self), NULL);

    GPasteItemPrivate *priv = self->priv;
    const gchar *display_string = priv->display_string;

    return (display_string) ? display_string : priv->value;
}

/**
 * g_paste_item_equals:
 * @self: a #GPasteItem instance
 * @other: another #GPasteItem instance
 *
 * Compare the two instances
 *
 * Returns: true if equals, false otherwise
 */
G_PASTE_VISIBLE gboolean
g_paste_item_equals (const GPasteItem *self,
                     const GPasteItem *other)
{
    g_return_val_if_fail (G_PASTE_IS_ITEM (self), FALSE);
    g_return_val_if_fail (G_PASTE_IS_ITEM (other), FALSE);

    return G_PASTE_ITEM_GET_CLASS (self)->equals (self, other);
}

/**
 * g_paste_item_get_kind:
 * @self: a #GPasteItem instance
 *
 * Get the kind of #GPasteItem as string (for serialization)
 *
 * Returns: read-only string containing the kind of GPasteItem
 *          can be "Text", "Uris" or "Image"
 */
G_PASTE_VISIBLE const gchar *
g_paste_item_get_kind (const GPasteItem *self)
{
    g_return_val_if_fail (G_PASTE_IS_ITEM (self), NULL);

    return G_PASTE_ITEM_GET_CLASS (self)->get_kind (self);
}

/**
 * g_paste_item_set_display_string: (skip)
 */
void
g_paste_item_set_display_string (GPasteItem  *self,
                                 const gchar *display_string)
{
    g_return_if_fail (G_PASTE_IS_ITEM (self));

    GPasteItemPrivate *priv = self->priv;

    g_free (priv->display_string);
    priv->display_string = g_strdup (display_string);
}

/**
 * g_paste_item_set_state:
 * @self: a #GPasteItem instance
 * @state: a #GPasteItemState
 *
 * Set whether this item is Active or Idle
 *
 * Returns:
 */
G_PASTE_VISIBLE void
g_paste_item_set_state (GPasteItem     *self,
                        GPasteItemState state)
{
    g_return_if_fail (G_PASTE_IS_ITEM (self));

    G_PASTE_ITEM_GET_CLASS (self)->set_state (self, state);
}

static void
g_paste_item_finalize (GObject *object)
{
    GPasteItemPrivate *priv = G_PASTE_ITEM (object)->priv;

    g_free (priv->value);
    g_free (priv->display_string);

    G_OBJECT_CLASS (g_paste_item_parent_class)->finalize (object);
}

static gboolean
g_paste_item_default_equals (const GPasteItem *self,
                             const GPasteItem *other)
{
    return (g_strcmp0 (self->priv->value, other->priv->value) == 0);
}

static void
g_paste_item_default_set_state (GPasteItem     *self G_GNUC_UNUSED,
                                GPasteItemState state G_GNUC_UNUSED)
{
}

static void
g_paste_item_class_init (GPasteItemClass *klass)
{
    g_type_class_add_private (klass, sizeof (GPasteItemPrivate));

    klass->equals = g_paste_item_default_equals;
    klass->get_kind = NULL;
    klass->set_state = g_paste_item_default_set_state;

    G_OBJECT_CLASS (klass)->finalize = g_paste_item_finalize;
}

static void
g_paste_item_init (GPasteItem *self)
{
    self->priv = G_PASTE_ITEM_GET_PRIVATE (self);
}

/**
 * g_paste_item_new: (skip)
 */
GPasteItem *
g_paste_item_new (GType        type,
                  const gchar *value)
{
    GPasteItem *self = g_object_new (type, NULL);
    GPasteItemPrivate *priv = self->priv;

    priv->value = g_strdup (value);
    priv->display_string = NULL;

    return self;
}
