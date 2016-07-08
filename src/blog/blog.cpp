#include "blog.h"
#include <stdarg.h>

BLog::BLog() {

}

BLog::~BLog() {

}

BLog* BLog::Get() {
  if (m_singleton == nullptr) {
    m_singleton = new BLog;
  }
  return m_singleton;
}

void BLog::InitStream() {
  m_stream.open(BLOG_FILE, std::ios::binary);
}

void BLog::Log(const char* format, ...) {
  if (!BLog::m_stream.is_open())
    InitStream();

  va_list args;
  va_start(args, format);
  
}
