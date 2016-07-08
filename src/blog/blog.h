#include <fstream>
#define BLOG_FILE "bitto.log"
using std::fstream;

class BLog {
  BLog();
  ~BLog();

public:
  static BLog* m_singleton;
  fstream m_stream;

  static BLog* Get();
  static void Log(const char* format, ...);
  void InitStream();
};
