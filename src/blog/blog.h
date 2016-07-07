#include <fstream>
#define BLOG_FILE "bitto.log"
using std::fstream;

class BLog {
  Blog();
  ~BLog();

private:
  static Blog* m_singleton;
public:
  fstream m_stream;

  static Blog* Get();
  static void Log(const char* str);
  void InitStream();
}
