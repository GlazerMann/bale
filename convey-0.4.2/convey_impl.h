// Copyright (c) 2018, Institute for Defense Analyses,
// 4850 Mark Center Drive, Alexandria, VA 22311-1882; 703-845-2500.
//
// This material may be reproduced by or for the U.S. Government 
// pursuant to the copyright license under the clauses at DFARS 
// 252.227-7013 and 252.227-7014.

#ifndef CONVEY_IMPL_H
#define CONVEY_IMPL_H

#include "convey.h"

// Negative error codes:
enum convey_error {
  convey_error_OFLO = -10, // item is too large for conveyor
  convey_error_MISFIT,     // next item is wrong size for pull
  convey_error_RIGID,      // conveyor is not elastic
  convey_error_ALLOC,      // a memory allocation failed
  convey_error_COMMS,      // an internal communication operation failed
  convey_error_TEAM,       // collective method called with wrong communicator
  convey_error_RANGE,      // PE number out of range
  convey_error_NULL,       // method called on a null conveyor
  convey_error_USAGE,      // caller has violated the contract
  convey_error_STATE,      // method called in the wrong state
};

enum convey_state {
  convey_DORMANT = 0,
  convey_WORKING = 1,
  convey_ENDGAME = 2,
  convey_CLEANUP = 3,
  convey_COMPLETE = 4,
};

typedef struct conveyor_methods convey_methods_t;

struct conveyor_methods {
  const convey_methods_t* _super_;
  int (*push)(convey_t* self, const void* item, int64_t pe);
  int (*pull)(convey_t* self, void* item, int64_t* from);
  int (*unpull)(convey_t* self);
  int (*advance)(convey_t* self, bool done);
  int (*begin)(convey_t* self);
  int (*reset)(convey_t* self);
  int (*free)(convey_t* self);
  int64_t (*statistic)(convey_t* self, int which);
  const char* (*error_string)(convey_t* self, int error);
  void* (*apull)(convey_t* self, int64_t* from);
  int (*epush)(convey_t* self, size_t bytes, const void* item, int64_t pe);
  int (*epull)(convey_t* self, convey_item_t* result);
  // Private method
  int (*panic)(convey_t* self, const char* where, int error);
};

struct conveyor {
  const convey_methods_t* _class_;
  uint64_t features;
  size_t item_size;
  size_t n_procs;
  uint64_t suppress;  // bitmask: errors to pass through
  int64_t state;
};

int convey_checked_push(convey_t* self, const void* item, int64_t pe);
int convey_checked_pull(convey_t* self, void* item, int64_t* from);
int convey_checked_unpull(convey_t* self);
void* convey_checked_apull(convey_t* self, int64_t* from);
int convey_checked_epush(convey_t* c, size_t bytes, const void* item, int64_t pe);
int convey_checked_epull(convey_t* c, convey_item_t* result);

int convey_no_epush(convey_t* c, size_t bytes, const void* item, int64_t pe);
int convey_no_epull(convey_t* c, convey_item_t* result);

int convey_panic(convey_t* self, const char* where, int error);

#endif
