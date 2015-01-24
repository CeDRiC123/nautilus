/* nautilus-notification-delete.c
 *
 * Copyright (C) 2015 Carlos Soriano <csoriano@gnome.org>
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

#include <glib/gi18n.h>

#include "nautilus-notification-delete.h"
#include "nautilus-notification-manager.h"
#include "nautilus-file-undo-manager.h"

struct _NautilusNotificationDeletePrivate
{
  guint n_items;
  guint timeout_id;
  NautilusWindow *window;
};

G_DEFINE_TYPE_WITH_PRIVATE (NautilusNotificationDelete, nautilus_notification_delete, GTK_TYPE_GRID)

enum {
  PROP_0,
  PROP_N_ITEMS,
  PROP_WINDOW,
  LAST_PROP
};

enum
{
  NOTIFICATION_TIMEOUT = 10
};

static void
nautilus_notification_delete_remove_timeout (NautilusNotificationDelete *self)
{
  if (self->priv->timeout_id != 0)
    {
      g_source_remove (self->priv->timeout_id);
      self->priv->timeout_id = 0;
    }
}

static void
nautilus_notification_delete_destroy (NautilusNotificationDelete *self)
{
  nautilus_notification_delete_remove_timeout (self);
  gtk_widget_destroy (GTK_WIDGET (self));
}

static gboolean
nautilus_notification_delete_on_timeout (gpointer user_data)
{
  NautilusNotificationDelete *self = NAUTILUS_NOTIFICATION_DELETE (user_data);

  self->priv->timeout_id = 0;
  nautilus_notification_delete_destroy (self);

  return G_SOURCE_REMOVE;
}

static void
nautilus_notification_delete_undo_clicked (NautilusNotificationDelete *self)
{
  // nautilus_file_undo_manager_undo (GTK_WINDOW (self->priv->window));
  nautilus_notification_delete_destroy (self);
}

static void
nautilus_notification_delete_constructed (GObject *object)
{
  NautilusNotificationDelete *self = NAUTILUS_NOTIFICATION_DELETE (object);
  GtkWidget *close;
  gchar *label;
  GtkWidget *label_widget;
  GtkWidget *undo;

  G_OBJECT_CLASS (nautilus_notification_delete_parent_class)->constructed (object);

  if (self->priv->n_items == 1)
	  label = g_strdup (_("Selected item has been deleted"));
  else
    label = g_strdup_printf (_("%d items have been deleted"),
                                self->priv->n_items);
  label_widget = gtk_label_new (label);
  gtk_widget_set_halign (label_widget, GTK_ALIGN_START);
  gtk_container_add (GTK_CONTAINER (self), label_widget);

  undo = gtk_button_new_with_label (_("Undo"));
  gtk_widget_set_valign (undo, GTK_ALIGN_CENTER);
  gtk_container_add (GTK_CONTAINER (self), undo);
  g_signal_connect_swapped (undo, "clicked", G_CALLBACK (nautilus_notification_delete_undo_clicked), self);

  close = gtk_button_new_from_icon_name ("window-close-symbolic", GTK_ICON_SIZE_BUTTON);
  gtk_widget_set_valign (close, GTK_ALIGN_CENTER);
  gtk_button_set_focus_on_click (GTK_BUTTON (close), FALSE);
  gtk_button_set_relief (GTK_BUTTON (close), GTK_RELIEF_NONE);
  gtk_container_add (GTK_CONTAINER (self), close);
  g_signal_connect_swapped (close, "clicked", G_CALLBACK (nautilus_notification_delete_destroy), self);

  self->priv->timeout_id = g_timeout_add_seconds_full (G_PRIORITY_DEFAULT,
                                                       NOTIFICATION_TIMEOUT,
                                                       nautilus_notification_delete_on_timeout,
                                                       g_object_ref (self),
                                                       g_object_unref);
}

NautilusNotificationDelete *
nautilus_notification_delete_new (NautilusWindow *window,
                                  guint           n_items)
{
  g_assert (NAUTILUS_IS_WINDOW (window));

  return g_object_new (NAUTILUS_TYPE_NOTIFICATION_DELETE,
                       "window", window,
                       "n-items", n_items,
                       NULL);
}

static void
nautilus_notification_delete_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
  NautilusNotificationDelete *self = NAUTILUS_NOTIFICATION_DELETE (object);

  switch (prop_id)
    {
    case PROP_N_ITEMS:
      self->priv->n_items = g_value_get_int (value);
      break;

    case PROP_WINDOW:
      self->priv->window = NAUTILUS_WINDOW (g_value_get_pointer (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nautilus_notification_delete_class_init (NautilusNotificationDeleteClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = nautilus_notification_delete_set_property;
  object_class->constructed = nautilus_notification_delete_constructed;
  g_object_class_install_property (object_class,
                                   PROP_N_ITEMS,
                                   g_param_spec_int ("n-items",
                                                     "Number of deleted items",
                                                     "Number of items that are being deleted",
                                                     1,
                                                     G_MAXINT,
                                                     1,
                                                     G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE));
  g_object_class_install_property (object_class,
                                   PROP_WINDOW,
                                   g_param_spec_pointer ("window",
                                                         "Window associated",
                                                         "The window that contains the notification",
                                                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE));
}

static void
nautilus_notification_delete_init (NautilusNotificationDelete *self)
{
  self->priv = nautilus_notification_delete_get_instance_private (self);
}

