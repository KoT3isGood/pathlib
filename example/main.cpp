#include "../src/pathlib.h"
int main() {
	TraceLog(LogType::General, "Hello,world\n");
  if(FileExists("general.log")) {
    TraceLog(LogType::General, "general.log exists");
  };
	return 0;
}
