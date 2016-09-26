#ifndef _FILE_DISTURB_PUBLIC_
#define _FILE_DISTURB_PUBLIC_

#include "c_file_filter_log.h"

struct LacCell
{
	long _lac;
	long _cell;
	void clear() {
		_lac = 0;
		_cell = 0;
	};
	bool operator < (const LacCell ot) const 
	{
		/* 两个key必须都匹配才命中 */
		return ((_lac < ot._lac) || \
			((_lac == ot._lac) && (_cell < ot._cell)));
	}

	bool operator == (const LacCell & ot)
	{
		return (_cell == ot._cell) && (_lac == ot._lac);
	}
};

#define LOG_DEBUGV2(fmt, ...) \
    do \
    { \
        printf("DEBUG-->" fmt"\n", ##__VA_ARGS__); \
    }while(0);

#define LOG_INFO(fmt, ...) \
    do \
    { \
        char arrLog[512] = {0}; \
        sprintf(arrLog, fmt " [%s,%s(),line %d]", ##__VA_ARGS__, __FILE__,__FUNCTION__,__LINE__); \
        LOG_DEBUGV2("%s", arrLog); \
        FileFilterLog  *g_FileDisturbLog; \
        g_FileFilterLog->AddFileFilterLogRunLog(arrLog);\
    }while(0);

#define LOG_WRITE(fmt, ...) \
    do \
    { \
        char arrLog[512] = {0}; \
        sprintf(arrLog, fmt " ", ##__VA_ARGS__); \
        LOG_DEBUGV2("%s", arrLog); \
        FileFilterLog  *g_FileFilterLog; \
        g_FileFilterLog->AddFileFilterLogRunLog(arrLog);\
    }while(0);

#define __ENTERFUNCTION;  try{

#define __LEAVEFUNCTION; \
} \
	catch (otl_exception& e) \
{ \
	LOG_INFO("SQL执行异常\n\tmsg=[%s]\n\tSQL=[%s]\n\tvar_info=[%s]", e.msg, e.stm_text, e.var_info); \
} \
	catch (TCException &e) \
{ \
	LOG_INFO("Error:%s", (char*)e.GetExceptionMessage()); \
} \
	catch (...) \
{ \
	LOG_INFO("UnKnown Error"); \
}
#endif // file_disturb_public.h
