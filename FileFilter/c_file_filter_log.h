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

	// ���������ļ�������־
	void AddNetMgrFileOperLog(const TCString&, TCString);

	// �����������ݱ������־
	void AddNetMgrTableOperLog(const TCString&, TCString);

	// ��������ʱ��Ϣ
	void AddFileFilterLogRunLog(char*);

	void DebugLog(const TCString&);
	void DebugLog(char*);
};

#endif // !_FILE_FILTER_LOG_H_

