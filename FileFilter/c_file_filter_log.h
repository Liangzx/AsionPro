#ifndef _FILE_FILTER_LOG_H_
#define _FILE_FILTER_LOG_H_
#include "cmpublic.h"
#include "c_critical_section.h"
#include "c_file_filter_config.h"

class FileFilterLog
{
	static TCCriticalSection csLog;
public:
	FileFilterLog();
	~FileFilterLog();
	void AddFileFilterLogRunLog(const TCString&);

	// 增加网管文件操作日志
	void AddNetMgrFileOperLog(const TCString&, TCString);

	// 增加网管数据表操作日志
	void AddNetMgrTableOperLog(const TCString&, TCString);

	// 增加运行时信息
	void AddFileFilterLogRunLog(char*);

	void DebugLog(const TCString&);
	void DebugLog(char*);
};

#endif // !_FILE_FILTER_LOG_H_

