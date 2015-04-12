#include "mmal/util/mmal_default_components.h"
#include "mmal/util/mmal_util.h"
#include "mmal/util/mmal_util_params.h"
#include "mmal/mmal.h"
#include "mmal/util/mmal_connection.h"
 MMAL_BUFFER_HEADER_T *mmal_queue_get(MMAL_QUEUE_T *queue){}
MMAL_STATUS_T mmal_buffer_header_mem_lock(MMAL_BUFFER_HEADER_T *header){}

void mmal_buffer_header_mem_unlock(MMAL_BUFFER_HEADER_T *header){}
 void mmal_port_pool_destroy(MMAL_PORT_T *port, MMAL_POOL_T *pool){}

/** Create an instance of a component.
 * The newly created component will expose ports to the client. All the exposed ports are
 * disabled by default.
 * Note that components are reference counted and creating a component automatically
 * acquires a reference to it (released when \ref mmal_component_destroy is called).
 *
 * @param name name of the component to create, e.g. "video_decode"
 * @param component returned component
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_component_create(const char *name,
                                    MMAL_COMPONENT_T **component){}

/** Acquire a reference on a component.
 * Acquiring a reference on a component will prevent a component from being destroyed until
 * the acquired reference is released (by a call to \ref mmal_component_destroy).
 * References are internally counted so all acquired references need a matching call to
 * release them.
 *
 * @param component component to acquire
 */
void mmal_component_acquire(MMAL_COMPONENT_T *component){}

/** Release a reference on a component
 * Release an acquired reference on a component. Triggers the destruction of the component when
 * the last reference is being released.
 * \note This is in fact an alias of \ref mmal_component_destroy which is added to make client
 * code clearer.
 *
 * @param component component to release
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_component_release(MMAL_COMPONENT_T *component){}

/** Destroy a previously created component
 * Release an acquired reference on a component. Only actually destroys the component when
 * the last reference is being released.
 *
 * @param component component to destroy
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_component_destroy(MMAL_COMPONENT_T *component){}

/** Enable processing on a component
 * @param component component to enable
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_component_enable(MMAL_COMPONENT_T *component){}

/** Disable processing on a component
 * @param component component to disable
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_component_disable(MMAL_COMPONENT_T *component){}




/** Commit format changes on a port.
 *
 * @param port The port for which format changes are to be committed.
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_format_commit(MMAL_PORT_T *port){}


/** Enable processing on a port
 *
 * If this port is connected to another, the given callback must be NULL, while for a
 * disconnected port, the callback must be non-NULL.
 *
 * If this is a connected output port and is successfully enabled:
 * <ul>
 * <li>The port shall be populated with a pool of buffers, allocated as required, according
 * to the buffer_num and buffer_size values.
 * <li>The input port to which it is connected shall be set to the same buffer
 * configuration and then be enabled. Should that fail, the original port shall be
 * disabled.
 * </ul>
 *
 * @param port port to enable
 * @param cb callback use by the port to send a \ref MMAL_BUFFER_HEADER_T back
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_enable(MMAL_PORT_T *port, MMAL_PORT_BH_CB_T cb){}

/** Disable processing on a port
 *
 * Disabling a port will stop all processing on this port and return all (non-processed)
 * buffer headers to the client.
 *
 * If this is a connected output port, the input port to which it is connected shall
 * also be disabled. Any buffer pool shall be released.
 *
 * @param port port to disable
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_disable(MMAL_PORT_T *port){}

/** Ask a port to release all the buffer headers it currently has.
 *
 * Flushing a port will ask the port to send all the buffer headers it currently has
 * to the client. Flushing is an asynchronous request and the flush call will
 * return before all the buffer headers are returned to the client.
 * It is up to the client to keep a count on the buffer headers to know when the
 * flush operation has completed.
 * It is also important to note that flushing will also reset the state of the port
 * and any processing which was buffered by the port will be lost.
 *
 * \attention Flushing a connected port behaviour TBD.
 *
 * @param port The port to flush.
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_flush(MMAL_PORT_T *port){}

/** Set a parameter on a port.
 *
 * @param port The port to which the request is sent.
 * @param param The pointer to the header of the parameter to set.
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_parameter_set(MMAL_PORT_T *port,
   const MMAL_PARAMETER_HEADER_T *param){}

/** Get a parameter from a port.
 * The size field must be set on input to the maximum size of the parameter
 * (including the header) and will be set on output to the actual size of the
 * parameter retrieved.
 *
 * \note If MMAL_ENOSPC is returned, the parameter is larger than the size
 * given. The given parameter will have been filled up to its size and then
 * the size field set to the full parameter's size. This can be used to
 * resize the parameter buffer so that a second call should succeed.
 *
 * @param port The port to which the request is sent.
 * @param param The pointer to the header of the parameter to get.
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_parameter_get(MMAL_PORT_T *port,
   MMAL_PARAMETER_HEADER_T *param){}

/** Send a buffer header to a port.
 *
 * @param port The port to which the buffer header is to be sent.
 * @param buffer The buffer header to send.
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_send_buffer(MMAL_PORT_T *port,
   MMAL_BUFFER_HEADER_T *buffer){}

/** Connect an output port to an input port.
 *
 * When connected and enabled, buffers will automatically progress from the
 * output port to the input port when they become available, and released back
 * to the output port when no longer required by the input port.
 *
 * Ports can be given either way around, but one must be an output port and
 * the other must be an input port. Neither can be connected or enabled
 * already. The format of the output port will be applied to the input port
 * on connection.
 *
 * @param port One of the ports to connect.
 * @param other_port The other port to connect.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_port_connect(MMAL_PORT_T *port, MMAL_PORT_T *other_port){}

/** Disconnect a connected port.
 *
 * If the port is not connected, an error will be returned. Otherwise, if the
 * ports are enabled, they will be disabled and any buffer pool created will be
 * freed.
 *
 * @param port The ports to disconnect.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_port_disconnect(MMAL_PORT_T *port){}

/** Allocate a payload buffer.
 * This allows a client to allocate memory for a payload buffer based on the preferences
 * of a port. This for instance will allow the port to allocate memory which can be shared
 * between the host processor and videocore.
 *
 * See \ref mmal_pool_create_with_allocator().
 *
 * @param port         Port responsible for allocating the memory.
 * @param payload_size Size of the payload buffer which will be allocated.
 *
 * @return Pointer to the allocated memory.
 */
uint8_t *mmal_port_payload_alloc(MMAL_PORT_T *port, uint32_t payload_size){}

/** Free a payload buffer.
 * This allows a client to free memory allocated by a previous call to \ref mmal_port_payload_alloc.
 *
 * See \ref mmal_pool_create_with_allocator().
 *
 * @param port         Port responsible for allocating the memory.
 * @param payload      Pointer to the memory to free.
 */
void mmal_port_payload_free(MMAL_PORT_T *port, uint8_t *payload){}

/** Get an empty event buffer header from a port
 *
 * @param port The port from which to get the event buffer header.
 * @param buffer The address of a buffer header pointer, which will be set on return.
 * @param event The specific event FourCC required. See the \ref MmalEvents "pre-defined events".
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_port_event_get(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T **buffer, uint32_t event){}
/** Shallow copy a format structure.
 * It is worth noting that the extradata buffer will not be copied in the new format.
 *
 * @param format_dest destination \ref MMAL_ES_FORMAT_T for the copy
 * @param format_src source \ref MMAL_ES_FORMAT_T for the copy
 */
void mmal_format_copy(MMAL_ES_FORMAT_T *format_dest, MMAL_ES_FORMAT_T *format_src){}



/** Create a connection between two ports.
 * The connection shall include a pool of buffer headers suitable for the current format of
 * the output port. The format of the input port shall have been set to the same as that of
 * the input port.
 * Note that connections are reference counted and creating a connection automatically
 * acquires a reference to it (released when \ref mmal_connection_destroy is called).
 *
 * @param connection The address of a connection pointer that will be set to point to the created
 * connection.
 * @param out        The output port to use for the connection.
 * @param in         The input port to use for the connection.
 * @param flags      The flags specifying which type of connection should be created.
 *    A bitwise combination of \ref connectionflags "Connection flags" values.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_connection_create(MMAL_CONNECTION_T **connection,
   MMAL_PORT_T *out, MMAL_PORT_T *in, uint32_t flags){}

/** Acquire a reference on a connection.
 * Acquiring a reference on a connection will prevent a connection from being destroyed until
 * the acquired reference is released (by a call to \ref mmal_connection_destroy).
 * References are internally counted so all acquired references need a matching call to
 * release them.
 *
 * @param connection connection to acquire
 */
void mmal_connection_acquire(MMAL_CONNECTION_T *connection){}

/** Release a reference on a connection
 * Release an acquired reference on a connection. Triggers the destruction of the connection when
 * the last reference is being released.
 * \note This is in fact an alias of \ref mmal_connection_destroy which is added to make client
 * code clearer.
 *
 * @param connection connection to release
 * @return MMAL_SUCCESS on success
 */
MMAL_STATUS_T mmal_connection_release(MMAL_CONNECTION_T *connection){}

/** Destroy a connection.
 * Release an acquired reference on a connection. Only actually destroys the connection when
 * the last reference is being released.
 * The actual destruction of the connection will start by disabling it, if necessary.
 * Any pool, queue, and so on owned by the connection shall then be destroyed.
 *
 * @param connection The connection to be destroyed.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_connection_destroy(MMAL_CONNECTION_T *connection){}

/** Enable a connection.
 * The format of the two ports must have been committed before calling this function,
 * although note that on creation, the connection automatically copies and commits the
 * output port's format to the input port.
 *
 * The MMAL_CONNECTION_T::callback field must have been set if the \ref MMAL_CONNECTION_FLAG_TUNNELLING
 * flag was not specified on creation. The client may also set the MMAL_CONNECTION_T::user_data
 * in order to get a pointer passed, via the connection, to the callback.
 *
 * @param connection The connection to be enabled.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_connection_enable(MMAL_CONNECTION_T *connection){}

/** Disable a connection.
 *
 * @param connection The connection to be disabled.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_connection_disable(MMAL_CONNECTION_T *connection){}

/** Apply a format changed event to the connection.
 * This function can be used when the client is processing buffer headers and receives
 * a format changed event (\ref MMAL_EVENT_FORMAT_CHANGED). The connection is
 * reconfigured, changing the format of the ports, the number of buffer headers and
 * the size of the payload buffers as necessary.
 *
 * @param connection The connection to which the event shall be applied.
 * @param buffer The buffer containing a format changed event.
 * @return MMAL_SUCCESS on success.
 */
MMAL_STATUS_T mmal_connection_event_format_changed(MMAL_CONNECTION_T *connection,
   MMAL_BUFFER_HEADER_T *buffer){}


/** Helper function to set the value of a rational parameter.
 * @param port   port on which to set the parameter
 * @param id     parameter id
 * @param value  value to set the parameter to
 *
 * @return MMAL_SUCCESS or error
 */
MMAL_STATUS_T mmal_port_parameter_set_rational(MMAL_PORT_T *port, uint32_t id, MMAL_RATIONAL_T value){}

/** Helper function to set the value of a 32 bits signed integer parameter.
 * @param port   port on which to set the parameter
 * @param id     parameter id
 * @param value  value to set the parameter to
 *
 * @return MMAL_SUCCESS or error
 */
MMAL_STATUS_T mmal_port_parameter_set_int32(MMAL_PORT_T *port, uint32_t id, int32_t value){}


/** Helper function to set the value of a 32 bits unsigned integer parameter.
 * @param port   port on which to set the parameter
 * @param id     parameter id
 * @param value  value to set the parameter to
 *
 * @return MMAL_SUCCESS or error
 */
MMAL_STATUS_T mmal_port_parameter_set_uint32(MMAL_PORT_T *port, uint32_t id, uint32_t value){}


/** Helper function to set the value of a boolean parameter.
 * @param port   port on which to set the parameter
 * @param id     parameter id
 * @param value  value to set the parameter to
 *
 * @return MMAL_SUCCESS or error
 */
MMAL_STATUS_T mmal_port_parameter_set_boolean(MMAL_PORT_T *port, uint32_t id, MMAL_BOOL_T value){}



/** Create a pool of MMAL_BUFFER_HEADER_T associated with a specific port.
 * This allows a client to allocate memory for the payload buffers based on the preferences
 * of a port. This for instance will allow the port to allocate memory which can be shared
 * between the host processor and videocore.
 * After allocation, all allocated buffer headers will have been added to the queue.
 *
 * It is valid to create a pool with no buffer headers, or with zero size payload buffers.
 * The mmal_pool_resize() function can be used to increase or decrease the number of buffer
 * headers, or the size of the payload buffers, after creation of the pool.
 *
 * @param port         Port responsible for creating the pool.
 * @param headers      Number of buffers which will be allocated with the pool.
 * @param payload_size Size of the payload buffer which will be allocated in
 *                     each of the buffer headers.
 * @return Pointer to the newly created pool or NULL on failure.
 */
MMAL_POOL_T *mmal_port_pool_create(MMAL_PORT_T *port,
   unsigned int headers, uint32_t payload_size){}

/** Release a buffer header.
 * Releasing a buffer header will decrease its reference counter and when no more references
 * are left, the buffer header will be recycled by calling its 'release' callback function.
 *
 * If a pre-release callback is set (\ref MMAL_BH_PRE_RELEASE_CB_T), this will be invoked
 * before calling the buffer's release callback and potentially postpone buffer recycling.
 * Once pre-release is complete the buffer header is recycled with
 * \ref mmal_buffer_header_release_continue.
 *
 * @param header buffer header to release
 */
void mmal_buffer_header_release(MMAL_BUFFER_HEADER_T *header){}
/** Get the number of MMAL_BUFFER_HEADER_T currently in a queue.
 *
 * @param queue  Pointer to a queue
 *
 * @return length (in elements) of the queue.
 */
unsigned int mmal_queue_length(MMAL_QUEUE_T *queue){}
