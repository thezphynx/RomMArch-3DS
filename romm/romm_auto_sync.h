#ifndef ROMM_AUTO_SYNC_H
#define ROMM_AUTO_SYNC_H

#include <boolean.h>

typedef enum rommarch_auto_sync_phase
{
   ROMMARCH_AUTO_SYNC_PULL = 1,
   ROMMARCH_AUTO_SYNC_PUSH = 2
} rommarch_auto_sync_phase_t;

typedef void (*rommarch_auto_sync_complete_cb_t)(bool success, void *userdata);

bool rommarch_auto_sync_start(const char *content_path,
      rommarch_auto_sync_phase_t phase,
      rommarch_auto_sync_complete_cb_t callback, void *userdata);
bool rommarch_auto_sync_active(void);
bool rommarch_save_sync_busy(void);

/* Automatic lifecycle sync normally runs without the manual whole-library
 * progress task. Close Content may opt in to the same umbrella progress bar
 * once its one-shot PUSH phase has started. */
void rommarch_auto_sync_show_progress(void);

/* Automatic lifecycle sync owns menu input while work is pending. A save
 * conflict is the exception: its confirmation dialog must remain interactive. */
bool rommarch_auto_sync_blocks_menu(void);

/* True while Close Content is holding a core unload for its automatic save
 * PUSH. The menu uses this to ignore navigation input until the deferred
 * unload resumes. */
bool rommarch_auto_exit_pending(void);

/* True while automatic save sync is holding a content launch/reload. The menu
 * uses this to keep input owned by the lifecycle task until sync completes. */
bool rommarch_deferred_launch_pending(void);

/* Lifecycle continuations are deliberately resumed from the runloop, never
 * directly from an HTTP task callback. */
void rommarch_deferred_launch_process(void);
void rommarch_deferred_exit_process(void);

#endif
