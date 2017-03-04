/* Copyright (c) 2016, Rice University

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1.  Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.
3.  Neither the name of Rice University
     nor the names of its contributors may be used to endorse or
     promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
 * hclib-async-struct.h
 *  
 *      Authors: Vivek Kumar (vivekk@rice.edu), Max Grossman (jmg3@rice.edu)
 */

#ifndef HCLIB_ASYNCSTRUCT_H_
#define HCLIB_ASYNCSTRUCT_H_

#include <string.h>

#include "hclib-place.h"
#include "hclib-task.h"

#ifdef __cplusplus
extern "C" {
#endif

inline hclib_future_t ** get_future_list(hclib_task_t *t) {
    return t->future_list;
}

inline void mark_as_async_any_task(hclib_task_t *t) {
    t->is_async_any_type = 1;
}

inline int is_async_any_task(hclib_task_t *t) {
    return t->is_async_any_type;
}

void spawn(hclib_task_t * task);
void spawn_at_hpt(place_t* pl, hclib_task_t * task);
void spawn_await_at(hclib_task_t * task, hclib_future_t** future_list,
        place_t *pl);
void spawn_await(hclib_task_t * task, hclib_future_t** future_list);
void spawn_escaping(hclib_task_t * task, hclib_future_t** future_list);
void spawn_comm_task(hclib_task_t * task);
void spawn_gpu_task(hclib_task_t *task);

#ifdef __cplusplus
}
#endif

#endif /* HCLIB_ASYNCSTRUCT_H_ */
