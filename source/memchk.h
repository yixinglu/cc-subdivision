// Memory leaks check header file

#ifndef _MEMORY_CHECK_H_
#define _MEMORY_CHECK_H_

#if defined(_DEBUG) || defined(DEBUG)
#define MEM_CHK_DEBUG
#endif

#ifdef MEM_CHK_DEBUG

#define _CRTDBG_MAP_ALLOC_
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif // DBG_NEW

#define MEM_CHK_START_DBG \
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

#define MEM_CHK_EXIT_DBG  _CrtDumpMemoryLeaks();

#define MEM_CHKPT_BEG                   \
    _CrtMemState MC_state_beg;          \
    _CrtMemCheckpoint( &MC_state_beg );

#define MEM_CHKPT_END                   \
    _CrtMemState MC_state_end;          \
    _CrtMemCheckpoint( &MC_state_end ); \
    _CrtMemState MC_state_diff;         \
    if (_CrtMemDifference(&MC_state_diff, &MC_state_beg, &MC_state_end)) {   \
        _CrtMemDumpStatistics(&MC_state_diff);     \
    }

#define MEM_CHK_SET_BREAK_ALLOC(num) _CrtSetBreakAlloc(num)

#else

#define MEM_CHK_START_DBG
#define MEM_CHK_EXIT_DBG
#define MEM_CHKPT_BEG
#define MEM_CHKPT_END
#define MEM_CHK_SET_BREAK_ALLOC(num)

#endif // MEM_CHK_DEBUG
#endif // _MEMORY_CHECK_H_

