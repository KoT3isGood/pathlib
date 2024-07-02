#include "../src/pathlib.h"
int main() {
	TraceLog(LogType::General, "Hello,world\n");
  TraceLog(LogType::General, "%d\n", FileExists("general.log"));
  TraceLog(LogType::General, "%d\n", DirectoryExists("src"));
  TraceLog(LogType::General,"%d\n",IsFileExtension("general.log",".log")); 
  TraceLog(LogType::General, "%i\n", GetFileLenght("general.log"));
	return 0;
}
