#pragma once

namespace base {

// Used for tracking file and line where a call is made for debug builds.
// No-op for non-debug builds.
// Callers can use the DEBUG_LOCATION macro in either case.
#ifndef NDEBUG
// TODO(roth): See if there's a way to automatically populate this,
// similarly to how absl::SourceLocation::current() works, so that
// callers don't need to explicitly pass DEBUG_LOCATION anywhere.
class DebugLocation {
 public:
  DebugLocation(const char* file, int line) : file_(file), line_(line) {}
  const char* file() const { return file_; }
  int line() const { return line_; }

 private:
  const char* file_;
  const int line_;
};
#define DEBUG_LOCATION Doraemon::base::DebugLocation(__FILE__, __LINE__)
#else
class DebugLocation {
 public:
  const char* file() const { return nullptr; }
  int line() const { return -1; }
};
#define DEBUG_LOCATION  Doraemon::base::DebugLocation()
#endif

}