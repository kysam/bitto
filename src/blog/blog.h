#ifndef h_blog
#define h_blog
#include <fstream>
#define BLOG_FILE "bitto.log"

#define log_blist(msg, ...)	BLog::Log("blist", msg, ## __VA_ARGS__)
#define log_session(msg, ...)	BLog::Log("session", msg, ## __VA_ARGS__)
#define log_protocol(msg, ...)	BLog::Log("protocol", msg, ## __VA_ARGS__)
#define log_network(msg, ...)	BLog::Log("network", msg, ## __VA_ARGS__)


using std::fstream;
class BLog {
public:
  BLog();
  ~BLog();

  static BLog* m_singleton;
  fstream m_stream;

  static BLog* Get();
  static void Log(const char *section, const char *format, ...);
  void InitStream();
};
#endif
