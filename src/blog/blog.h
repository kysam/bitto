#include <fstream>
#define BLOG_FILE "bitto.log"
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
