#ifndef NAUTILUS_DELETE_NOTIFICATION_H
#define NAUTILUS_DELETE_NOTIFICATION_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_DELETE_NOTIFICATION            (nautilus_delete_notification_get_type())
#define NAUTILUS_DELETE_NOTIFICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAUTILUS_TYPE_DELETE_NOTIFICATION, NautilusDeleteNotification))
#define NAUTILUS_DELETE_NOTIFICATION_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAUTILUS_TYPE_DELETE_NOTIFICATION, NautilusDeleteNotification const))
#define NAUTILUS_DELETE_NOTIFICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  NAUTILUS_TYPE_DELETE_NOTIFICATION, NautilusDeleteNotificationClass))
#define NAUTILUS_IS_DELETE_NOTIFICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAUTILUS_TYPE_DELETE_NOTIFICATION))
#define NAUTILUS_IS_DELETE_NOTIFICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  NAUTILUS_TYPE_DELETE_NOTIFICATION))
#define NAUTILUS_DELETE_NOTIFICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  NAUTILUS_TYPE_DELETE_NOTIFICATION, NautilusDeleteNotificationClass))

typedef struct _NautilusDeleteNotification        NautilusDeleteNotification;
typedef struct _NautilusDeleteNotificationClass   NautilusDeleteNotificationClass;
typedef struct _NautilusDeleteNotificationPrivate NautilusDeleteNotificationPrivate;

struct _NautilusDeleteNotification
{
  GtkGrid parent;

  /*< private >*/
  NautilusDeleteNotificationPrivate *priv;
};

struct _NautilusDeleteNotificationClass
{
  GtkGridClass parent;
};

GType                           nautilus_delete_notification_get_type (void);
NautilusDeleteNotification     *nautilus_delete_notification_new      (void);

G_END_DECLS

#endif /* NAUTILUS_DELETE_NOTIFICATION_H */
