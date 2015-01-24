#include "nautilus-delete-notification.h"

struct _NautilusDeleteNotificationPrivate
{
  GList *items;
  guint timeout_id;
};

G_DEFINE_TYPE_WITH_PRIVATE (NautilusDeleteNotification, nautilus_delete_notification, GTK_TYPE_GRID)

enum {
  PROP_0,
  PROP_ITEMS,
  LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

NautilusDeleteNotification *
nautilus_delete_notification_new (void)
{
  return g_object_new (NAUTILUS_TYPE_DELETE_NOTIFICATION, NULL);
}

static void
nautilus_delete_notification_finalize (GObject *object)
{
  NautilusDeleteNotificationPrivate *priv = NAUTILUS_DELETE_NOTIFICATION (object)->priv;

  G_OBJECT_CLASS (nautilus_delete_notification_parent_class)->finalize (object);
}

static void
nautilus_delete_notification_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
  NautilusDeleteNotification *self = NAUTILUS_DELETE_NOTIFICATION (object);

  switch (prop_id)
    {
    case PROP_ITEMS:

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nautilus_delete_notification_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
  NautilusDeleteNotification *self = NAUTILUS_DELETE_NOTIFICATION (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nautilus_delete_notification_class_init (NautilusDeleteNotificationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = nautilus_delete_notification_finalize;
  object_class->get_property = nautilus_delete_notification_get_property;
  object_class->set_property = nautilus_delete_notification_set_property;
}

static void
nautilus_delete_notification_init (NautilusDeleteNotification *self)
{
  self->priv = nautilus_delete_notification_get_instance_private (self);
}

static void
nautilus_delete_notification_remove_timeout (NAutilusDeleteNotification *self)
{
  if (self->priv->timeout_id != 0)
    {
      g_source_remove (self->priv->timeout_id);
      self->priv->timeout_id = 0;
    }
}

static void
nautilus_delete_notification_destroy (PhotosDeleteNotification *self)
{
  nautilus_delete_notification_remove_timeout (self);
  gtk_widget_destroy (GTK_WIDGET (self));
}

