#include "blog.h"
BLog::m_singleton = nullptr;
void BLog::BLog() {

}

void BLog::~BLog() {

}

Blog* BLog::Get() {
  if (m_singleton == nullptr) {
    m_singleton = new BLog;
  }
  return m_singleton;
}

void BLog::InitStream() {
  m_stream.open(BLOG_FILE, std::ios::binary);
}

void BLog::Log(const char* format, ...) {
  if (!m_stream.is_open())
    InitStream();

  va_list args;
  va_start(args, format);
  

}