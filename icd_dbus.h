#ifndef ICD_DBUS_H
#define ICD_DBUS_H

/**
 * Copyright (C) 2004-2007, 2010, Nokia. All rights reserved.
 *
 * @author Patrik Flykt <patrik.flykt@nokia.com>
 * @author Aapo Mäkelä <aapo.makela@nokia.com>
 * @author Luciano Coelho <luciano.coelho@nokia.com>
 * @author Jukka Rissanen <jukka.rissanen@nokia.com>
 *
 * @file icd_dbus.h
 */

#include <glib.h>

#include <dbus/dbus.h>
/* while dbus is under construction, do this */
#include <dbus/dbus-glib-lowlevel.h>

/** @addtogroup icd_dbus D-Bus helper functions
 * @ingroup support_libraries
 * @{ */

/** Method call handler data */
struct icd_dbus_mcall_table {
  /** method call name */
  const gchar *name;
  /** method call signature */
  const gchar *mcall_sig;
  /** method call reply signature */
  const gchar *reply_sig;
  /** handler function */
  DBusObjectPathMessageFunction handler_fn;
};

/** Signals sent from the interface */
struct icd_dbus_sig_table {
  /** signal name */
  const gchar *name;
  /** signal signature */
  const gchar *mcall_sig;
};

/** Property data */
struct icd_dbus_prop_table {
  /** property name */
  const gchar *name;
};

/**
 * Helper function for connecting D-Bus system bus paths to callbacks.
 * Unregister with dbus_connection_unregister_object_path().
 * @param path the D-Bus signal path to connect
 * @param cb the callback function to call when any signal is received
 * @param user_data user data passed to the callback function
 * @return TRUE on success, FALSE on failure
 */
gboolean icd_dbus_connect_system_path (const char *path,
				       DBusObjectPathMessageFunction cb,
				       void *user_data);
/** Helper function for disconnecting a system D-Bus path
 * @param path path
 * @return TRUE on success, FALSE on failure
 */
gboolean icd_dbus_disconnect_system_path (const char* path);

/** Function for connecting a callback to receive broadcast signals.
 * @note Since the broadcast reception is implemented as a filter for
 *       the specified interface, all sorts of D-Bus actions related to that
 *       interface will be captured. It is safe to return
 *       DBUS_HANDLER_RESULT_NOT_YET_HANDLED for unknown actions involving the
 *       callback.
 * @param interface the interface name of the broadcasted signal
 * @param cb the DBusHandleMessageFunction callback for receiving messages
 * @param user_data user data to be supplied to the callback function
 * @param extra_filters other filters to be included in the match or NULL
 * @return TRUE on success, FALSE on failure
 */
gboolean icd_dbus_connect_system_bcast_signal (const char *interface,
					       DBusHandleMessageFunction cb,
					       void *user_data,
					       const char *extra_filters);

/** Function for disconnecting a broadcast signal callback function
 * @note In the D-Bus documentation it is noted that it is a programming error
 *       to try to remove a callback function and user data that has not been
 *       added previously, in this case with
 *       'icd_dbus_connect_system_bcast_signal()'
 * @param interface the interface name of the broadcasted signal
 * @param cb the previously added DBusHandleMessageFunction callback
 * @param user_data the previously supplied user data
 * @param extra_filters other filters to be included in the match or NULL
 * @return TRUE on successful diconnection, FALSE otherwise
 */
gboolean icd_dbus_disconnect_system_bcast_signal (const char *interface,
						  DBusHandleMessageFunction cb,
						  void *user_data,
						  const char *extra_filters);

/** Helper function for registering a D-Bus system bus service.
 * @param path the D-Bus signal path to connect.
 * @param service the service to provide
 * @param service_flags D-Bus service flags
 * @param cb the callback function to call when any signal is received
 * @param user_data user data passed to the callback function
 * @return TRUE on success, FALSE on failure
 */
gboolean icd_dbus_register_system_service (const char *path,
					   const char *service,
					   guint service_flags,
					   DBusObjectPathMessageFunction cb,
					   void *user_data);

/** Helper function for unregistering a D-Bus system bus service
 * @param path object path
 * @param service registered service name
 */
void icd_dbus_unregister_system_service (const char *path,
					 const char *service);

/** Function for sending a system D-Bus method call message
 * @param message the D-Bus method call message to send
 * @param timeout reply timeout in milliseconds or -1 for D-Bus default value
 * @param cb callback function for the pending call, if NULL no callback
 *        will be set
 * @param user_data user data to pass to the callback
 * @return a D-Bus pending call if a callback function is given and the method
 *         call is successfully sent, NULL otherwise; the pending call returned
 *         is referenced, the caller must unreference it after use
 */
DBusPendingCall *icd_dbus_send_system_mcall (DBusMessage *message,
					     gint timeout,
					     DBusPendingCallNotifyFunction cb,
					     void *user_data);

/** Sends a D-Bus message to the system bus. Does not unref() the message.
 * @param message the D-Bus signal to send
 * @return TRUE if the message was a signal, method call return or error; FALSE
 *         otherwise
 */
gboolean icd_dbus_send_system_msg (DBusMessage *message);


/** Callback function for receiving the unique D-Bus id
 * @param name D-Bus service name
 * @param id D-Bus id
 * @param user_data user data
 */
typedef void
(*icd_dbus_get_unique_name_cb_fn) (const gchar *name,
				   const gchar *id,
				   gpointer user_data);

/** Cancel the unique name pending call
 * @param pending the pending call or NULL to cancel all
 */
void icd_dbus_cancel_unique_name (DBusPendingCall *pending);

/** Get the unique D-Bus bus name for a service
 * @param name D-Bus service name
 * @param cb callback
 * @param user_data user data
 * @return TRUE on success, FALSE on failure
 */
gboolean icd_dbus_get_unique_name (const gchar *name,
				   icd_dbus_get_unique_name_cb_fn cb,
				   gpointer user_data);

/** Close D-Bus system bus.
 */
void icd_dbus_close (void);

/** @} */

#endif
