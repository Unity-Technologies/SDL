/* 
 * Copyright © 2014      Jonas Ådahl
 * Copyright © 2015      Red Hat Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef POINTER_CONSTRAINTS_UNSTABLE_V1_CLIENT_PROTOCOL_H
#define POINTER_CONSTRAINTS_UNSTABLE_V1_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct wl_pointer;
struct wl_region;
struct wl_surface;
struct zwp_confined_pointer_v1;
struct zwp_locked_pointer_v1;
struct zwp_pointer_constraints_v1;

extern const struct wl_interface zwp_pointer_constraints_v1_interface;
extern const struct wl_interface zwp_locked_pointer_v1_interface;
extern const struct wl_interface zwp_confined_pointer_v1_interface;

#ifndef ZWP_POINTER_CONSTRAINTS_V1_ERROR_ENUM
#define ZWP_POINTER_CONSTRAINTS_V1_ERROR_ENUM
/**
 * zwp_pointer_constraints_v1_error - wp_pointer_constraints error values
 * @ZWP_POINTER_CONSTRAINTS_V1_ERROR_ALREADY_CONSTRAINED: pointer
 *	constraint already requested on that surface
 *
 * These errors can be emitted in response to wp_pointer_constraints
 * requests.
 */
enum zwp_pointer_constraints_v1_error {
	ZWP_POINTER_CONSTRAINTS_V1_ERROR_ALREADY_CONSTRAINED = 1,
};
#endif /* ZWP_POINTER_CONSTRAINTS_V1_ERROR_ENUM */

#ifndef ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_ENUM
#define ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_ENUM
/**
 * zwp_pointer_constraints_v1_lifetime - the pointer constraint is may
 *	reactivate
 * @ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_ONESHOT: (none)
 * @ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT: (none)
 *
 * A persistent pointer constraint may again reactivate once it has been
 * deactivated. See the corresponding deactivation event
 * (wp_locked_pointer.unlocked and wp_confined_pointer.unconfined) for
 * details.
 */
enum zwp_pointer_constraints_v1_lifetime {
	ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_ONESHOT = 1,
	ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT = 2,
};
#endif /* ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_ENUM */

#define ZWP_POINTER_CONSTRAINTS_V1_DESTROY	0
#define ZWP_POINTER_CONSTRAINTS_V1_LOCK_POINTER	1
#define ZWP_POINTER_CONSTRAINTS_V1_CONFINE_POINTER	2

static inline void
zwp_pointer_constraints_v1_set_user_data(struct zwp_pointer_constraints_v1 *zwp_pointer_constraints_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_pointer_constraints_v1, user_data);
}

static inline void *
zwp_pointer_constraints_v1_get_user_data(struct zwp_pointer_constraints_v1 *zwp_pointer_constraints_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_pointer_constraints_v1);
}

static inline void
zwp_pointer_constraints_v1_destroy(struct zwp_pointer_constraints_v1 *zwp_pointer_constraints_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_pointer_constraints_v1,
			 ZWP_POINTER_CONSTRAINTS_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_pointer_constraints_v1);
}

static inline struct zwp_locked_pointer_v1 *
zwp_pointer_constraints_v1_lock_pointer(struct zwp_pointer_constraints_v1 *zwp_pointer_constraints_v1, struct wl_surface *surface, struct wl_pointer *pointer, struct wl_region *region, uint32_t lifetime)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) zwp_pointer_constraints_v1,
			 ZWP_POINTER_CONSTRAINTS_V1_LOCK_POINTER, &zwp_locked_pointer_v1_interface, NULL, surface, pointer, region, lifetime);

	return (struct zwp_locked_pointer_v1 *) id;
}

static inline struct zwp_confined_pointer_v1 *
zwp_pointer_constraints_v1_confine_pointer(struct zwp_pointer_constraints_v1 *zwp_pointer_constraints_v1, struct wl_surface *surface, struct wl_pointer *pointer, struct wl_region *region, uint32_t lifetime)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) zwp_pointer_constraints_v1,
			 ZWP_POINTER_CONSTRAINTS_V1_CONFINE_POINTER, &zwp_confined_pointer_v1_interface, NULL, surface, pointer, region, lifetime);

	return (struct zwp_confined_pointer_v1 *) id;
}

/**
 * zwp_locked_pointer_v1 - receive relative pointer motion events
 * @locked: lock activation event
 * @unlocked: lock deactivation event
 *
 * The wp_locked_pointer interface represents a locked pointer state.
 *
 * While the lock of this object is active, the wl_pointer objects of the
 * associated seat will not emit any wl_pointer.motion events.
 *
 * This object will send the event 'locked' when the lock is activated.
 * Whenever the lock is activated, it is guaranteed that the locked surface
 * will already have received pointer focus and that the pointer will be
 * within the region passed to the request creating this object.
 *
 * To unlock the pointer, send the destroy request. This will also destroy
 * the wp_locked_pointer object.
 *
 * If the compositor decides to unlock the pointer the unlocked event is
 * sent. See wp_locked_pointer.unlock for details.
 *
 * When unlocking, the compositor may warp the cursor position to the set
 * cursor position hint. If it does, it will not result in any relative
 * motion events emitted via wp_relative_pointer.
 *
 * If the surface the lock was requested on is destroyed and the lock is
 * not yet activated, the wp_locked_pointer object is now defunct and must
 * be destroyed.
 */
struct zwp_locked_pointer_v1_listener {
	/**
	 * locked - lock activation event
	 *
	 * Notification that the pointer lock of the seat's pointer is
	 * activated.
	 */
	void (*locked)(void *data,
		       struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1);
	/**
	 * unlocked - lock deactivation event
	 *
	 * Notification that the pointer lock of the seat's pointer is no
	 * longer active. If this is a oneshot pointer lock (see
	 * wp_pointer_constraints.lifetime) this object is now defunct and
	 * should be destroyed. If this is a persistent pointer lock (see
	 * wp_pointer_constraints.lifetime) this pointer lock may again
	 * reactivate in the future.
	 */
	void (*unlocked)(void *data,
			 struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1);
};

static inline int
zwp_locked_pointer_v1_add_listener(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1,
				   const struct zwp_locked_pointer_v1_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_locked_pointer_v1,
				     (void (**)(void)) listener, data);
}

#define ZWP_LOCKED_POINTER_V1_DESTROY	0
#define ZWP_LOCKED_POINTER_V1_SET_CURSOR_POSITION_HINT	1
#define ZWP_LOCKED_POINTER_V1_SET_REGION	2

static inline void
zwp_locked_pointer_v1_set_user_data(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_locked_pointer_v1, user_data);
}

static inline void *
zwp_locked_pointer_v1_get_user_data(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_locked_pointer_v1);
}

static inline void
zwp_locked_pointer_v1_destroy(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_locked_pointer_v1,
			 ZWP_LOCKED_POINTER_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_locked_pointer_v1);
}

static inline void
zwp_locked_pointer_v1_set_cursor_position_hint(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_locked_pointer_v1,
			 ZWP_LOCKED_POINTER_V1_SET_CURSOR_POSITION_HINT, surface_x, surface_y);
}

static inline void
zwp_locked_pointer_v1_set_region(struct zwp_locked_pointer_v1 *zwp_locked_pointer_v1, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_locked_pointer_v1,
			 ZWP_LOCKED_POINTER_V1_SET_REGION, region);
}

/**
 * zwp_confined_pointer_v1 - confined pointer object
 * @confined: pointer confined
 * @unconfined: pointer unconfined
 *
 * The wp_confined_pointer interface represents a confined pointer state.
 *
 * This object will send the event 'confined' when the confinement is
 * activated. Whenever the confinement is activated, it is guaranteed that
 * the surface the pointer is confined to will already have received
 * pointer focus and that the pointer will be within the region passed to
 * the request creating this object. It is up to the compositor to decide
 * whether this requires some user interaction and if the pointer will warp
 * to within the passed region if outside.
 *
 * To unconfine the pointer, send the destroy request. This will also
 * destroy the wp_confined_pointer object.
 *
 * If the compositor decides to unconfine the pointer the unconfined event
 * is sent. The wp_confined_pointer object is at this point defunct and
 * should be destroyed.
 */
struct zwp_confined_pointer_v1_listener {
	/**
	 * confined - pointer confined
	 *
	 * Notification that the pointer confinement of the seat's
	 * pointer is activated.
	 */
	void (*confined)(void *data,
			 struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1);
	/**
	 * unconfined - pointer unconfined
	 *
	 * Notification that the pointer confinement of the seat's
	 * pointer is no longer active. If this is a oneshot pointer
	 * confinement (see wp_pointer_constraints.lifetime) this object is
	 * now defunct and should be destroyed. If this is a persistent
	 * pointer confinement (see wp_pointer_constraints.lifetime) this
	 * pointer confinement may again reactivate in the future.
	 */
	void (*unconfined)(void *data,
			   struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1);
};

static inline int
zwp_confined_pointer_v1_add_listener(struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1,
				     const struct zwp_confined_pointer_v1_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_confined_pointer_v1,
				     (void (**)(void)) listener, data);
}

#define ZWP_CONFINED_POINTER_V1_DESTROY	0
#define ZWP_CONFINED_POINTER_V1_SET_REGION	1

static inline void
zwp_confined_pointer_v1_set_user_data(struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_confined_pointer_v1, user_data);
}

static inline void *
zwp_confined_pointer_v1_get_user_data(struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_confined_pointer_v1);
}

static inline void
zwp_confined_pointer_v1_destroy(struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_confined_pointer_v1,
			 ZWP_CONFINED_POINTER_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_confined_pointer_v1);
}

static inline void
zwp_confined_pointer_v1_set_region(struct zwp_confined_pointer_v1 *zwp_confined_pointer_v1, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_confined_pointer_v1,
			 ZWP_CONFINED_POINTER_V1_SET_REGION, region);
}

#ifdef  __cplusplus
}
#endif

#endif
