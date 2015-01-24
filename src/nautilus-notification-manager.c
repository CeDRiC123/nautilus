#include "nautilus-notification-manager.h"
#include "nautilus-file-undo-manager.h"

struct _NautilusNotificationManagerPrivate
{
  GtkWidget *grid;
};

G_DEFINE_TYPE_WITH_PRIVATE (NautilusNotificationManager, nautilus_notification_manager, GD_TYPE_NOTIFICATION)

NautilusNotificationManager *
nautilus_notification_manager_new (void)
{
  return g_object_new (NAUTILUS_TYPE_NOTIFICATION_MANAGER,
                       "show-close-button", FALSE,
                       "timeout", -1,
                       NULL);
}

void
nautilus_notification_manager_add_notification (NautilusNotificationManager *self,
                                                GtkWidget                   *notification)
{
  gtk_container_add (GTK_CONTAINER (self->priv->grid), notification);
  gtk_widget_show_all (GTK_WIDGET (self));
  g_print ("add notification\n");
}

static void
nautilus_notification_manager_remove (NautilusNotificationManager *self)
{
  gtk_widget_hide (GTK_WIDGET (self));
  gtk_container_foreach (GTK_CONTAINER (self->priv->grid),
                         (GtkCallback) gtk_widget_destroy,
                         NULL);
  g_print ("remove\n");
}

static void
nautilus_notification_manager_undo_manager_changed (NautilusFileUndoManager     *undo_manager,
                                                    NautilusNotificationManager *self)
{
  g_print ("CHANGEEED \n");
  nautilus_notification_manager_remove (self);
}

static void
nautilus_notification_manager_dispose (GObject *object)
{
  NautilusNotificationManager *self = NAUTILUS_NOTIFICATION_MANAGER (object);

  g_signal_handlers_disconnect_by_func (nautilus_file_undo_manager_get (),
                                        G_CALLBACK (nautilus_notification_manager_undo_manager_changed),
                                        self);

  G_OBJECT_CLASS (nautilus_notification_manager_parent_class)->dispose (object);
}

static void
nautilus_notification_manager_class_init (NautilusNotificationManagerClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = nautilus_notification_manager_dispose;
}

static void
nautilus_notification_manager_init (NautilusNotificationManager *self)
{
  self->priv = nautilus_notification_manager_get_instance_private (self);

  gtk_widget_set_halign (GTK_WIDGET (self), GTK_ALIGN_CENTER);
  gtk_widget_set_valign (GTK_WIDGET (self), GTK_ALIGN_START);

  self->priv->grid = gtk_grid_new ();
  gtk_orientable_set_orientation (GTK_ORIENTABLE (self->priv->grid),
                                  GTK_ORIENTATION_VERTICAL);
  gtk_grid_set_row_spacing (GTK_GRID (self->priv->grid), 6);
  gtk_container_add (GTK_CONTAINER (self), self->priv->grid);

  g_signal_connect_swapped (self->priv->grid, "remove",
                            G_CALLBACK (nautilus_notification_manager_remove), self);
  /* For now we only support one undo operation at once. So if undo changed,
   * remove all notifications and the new one will be added to the appropiate
   * notification manager
   */
	g_signal_connect_object (nautilus_file_undo_manager_get (), "undo-changed",
                           G_CALLBACK (nautilus_notification_manager_undo_manager_changed), self, 0);
}
