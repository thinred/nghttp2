/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2015 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef SHRPX_MRUBY_H
#define SHRPX_MRUBY_H

#include "shrpx.h"

#include <memory>

#include <mruby.h>
#include <mruby/proc.h>

using namespace nghttp2;

namespace shrpx {

namespace mruby {

class MRubyContext {
public:
  MRubyContext(mrb_state *mrb, RProc *on_request_proc, RProc *on_response_proc);
  ~MRubyContext();

  int run_on_request_proc(Downstream *downstream);
  int run_on_response_proc(Downstream *downstream);

  int run_request_proc(Downstream *downstream, RProc *proc, int phase);

  void delete_downstream(Downstream *downstream);

private:
  mrb_state *mrb_;
  RProc *on_request_proc_;
  RProc *on_response_proc_;
  bool running_;
};

enum {
  PHASE_NONE = 0,
  PHASE_REQUEST = 1,
  PHASE_RESPONSE = 1 << 1,
};

struct MRubyAssocData {
  Downstream *downstream;
  int phase;
  bool request_headers_dirty;
  bool response_headers_dirty;
};

RProc *compile(mrb_state *mrb, const char *filename);

std::unique_ptr<MRubyContext> create_mruby_context();

// Return interned |ptr|.
mrb_sym intern_ptr(mrb_state *mrb, void *ptr);

// Checks that |phase| is set in |phase_mask|.  If not set, raise
// exception.
void check_phase(mrb_state *mrb, int phase, int phase_mask);

} // namespace mruby

} // namespace shrpx

#endif // SHRPX_MRUBY_H
