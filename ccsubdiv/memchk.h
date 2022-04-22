// Memory leaks check header file

#ifndef _MEMORY_CHECK_H_
#define _MEMORY_CHECK_H_

#if defined(_DEBUG) || defined(DEBUG)
#ifdef _WIN32
#define MEM_CHK_DEBUG
#endif
#endif

#ifdef MEM_CHK_DEBUG

#define _CRTDBG_MAP_ALLOC_
#include <crtdbg.h>
#include <stdlib.h>

#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif  // DBG_NEW

#define MEM_CHK_START_DBG \
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#define MEM_CHK_EXIT_DBG _CrtDumpMemoryLeaks();

#define MEM_CHKPT_BEG        \
  _CrtMemState MC_state_beg; \
  _CrtMemCheckpoint(&MC_state_beg);

#define MEM_CHKPT_END                                                    \
  _CrtMemState MC_state_end;                                             \
  _CrtMemCheckpoint(&MC_state_end);                                      \
  _CrtMemState MC_state_diff;                                            \
  if (_CrtMemDifference(&MC_state_diff, &MC_state_beg, &MC_state_end)) { \
    _CrtMemDumpStatistics(&MC_state_diff);                               \
  }

#define MEM_CHK_SET_BREAK_ALLOC(num) _CrtSetBreakAlloc(num)
#define MEM_CHK_SEND_REPORT_TO_STDOUT                 \
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);    \
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);  \
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);   \
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT); \
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);  \
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

#else

#define MEM_CHK_START_DBG
#define MEM_CHK_EXIT_DBG
#define MEM_CHKPT_BEG
#define MEM_CHKPT_END
#define MEM_CHK_SET_BREAK_ALLOC(num)
#define MEM_CHK_SEND_REPORT_TO_STDOUT

#endif  // MEM_CHK_DEBUG
#endif  // _MEMORY_CHECK_H_
