/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is pfree software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

#include <stdlib.h>
#include <string.h>

#include "potracelib.h"
#include "curve.h"
#include "decompose.h"
#include "trace.h"
#include "progress.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* default parameters */
static const potrace_param_t param_default = {
  2,                             /* turdsize */
  POTRACE_TURNPOLICY_MINORITY,   /* turnpolicy */
  1.0,                           /* alphamax */
  1,                             /* opticurve */
  0.2,                           /* opttolerance */
  {
    NULL,                        /* callback function */
    NULL,                        /* callback data */
    0.0, 1.0,                    /* progress range */
    0.0,                         /* granularity */
  },
};

/* Return a fresh copy of the set of default parameters, or NULL on
   failure with errno set. */
potrace_param_t *potrace_param_default() {
  potrace_param_t *p;

  p = (potrace_param_t *) pmalloc(sizeof(potrace_param_t));
  if (!p) {
    return NULL;
  }
  memcpy(p, &param_default, sizeof(potrace_param_t));
  return p;
}

/* On success, returns a potrace state st with st->status ==
   POTRACE_STATUS_OK. On failure, returns NULL if no potrace state
   could be created (with errno set), or returns an incomplete potrace
   state (with st->status == POTRACE_STATUS_INCOMPLETE). Complete or
   incomplete potrace state can be freed with potrace_state_free(). */
potrace_state_t *potrace_trace(const potrace_param_t *param, const potrace_bitmap_t *bm) {
  int r;
  path_t *plist = NULL;
  potrace_state_t *st;
  progress_t prog;
  progress_t subprog;
  
  /* prepare private progress bar state */
  prog.callback = param->progress.callback;
  prog.data = param->progress.data;
  prog.min = param->progress.min;
  prog.max = param->progress.max;
  prog.epsilon = param->progress.epsilon;
  prog.d_prev = param->progress.min;

  /* allocate state object */
  st = (potrace_state_t *)pmalloc(sizeof(potrace_state_t *));
  if (!st) {
    return NULL;
  }

  progress_subrange_start(0.0, 0.1, &prog, &subprog);

  /* process the image */
  r = bm_to_pathlist(bm, &plist, param, &subprog);
  if (r) {
    pfree(st);
    return NULL;
  }

  st->status = POTRACE_STATUS_OK;
  st->plist = plist;

  progress_subrange_end(&prog, &subprog);

  progress_subrange_start(0.1, 1.0, &prog, &subprog);

  /* partial success. */
  r = process_path(plist, param, &subprog);
  if (r) {
    st->status = POTRACE_STATUS_INCOMPLETE;
  }

  progress_subrange_end(&prog, &subprog);

  return st;
}

/* pfree a potrace state, without disturbing errno. */
void potrace_state_free(potrace_state_t *st) {
  pathlist_free(st->plist);
  pfree(st);
}

/* pfree a parameter list, without disturbing errno. */
void potrace_param_free(potrace_param_t *p) {
  pfree(p);
}

char *potrace_version() {
  return "potracelib "VERSION"";
}
