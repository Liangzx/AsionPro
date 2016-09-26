#include "c_file_filter_log.h"

extern FileFilterConfig* g_FileFilterConfig;



FileFilterLog::FileFilterLog()
{
}


FileFilterLog::~FileFilterLog()
{
}
//==========================================================================
// 函数 : FileFilterLog::AddFileFilterLogRunLog()
// 用途 : 写运行日志
// 原型 : AddSimuATURunLog(const TCString& sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::AddFileFilterLogRunLog(const TCString& sLog)
{
	csLog.Enter();
	try
	{
		TCString strFileName;
		TCString sLogPath;
		TCString strAppName;
		TCString strBuff;
		TCFileStream cFile;
		strAppName = Application.GetAppName();
		sLogPath = TAppPath::AppLog() + strAppName;
		ForceDirectories(sLogPath);
		strFileName = TCAppLog::GetDailyLogFileNameOfApplication();
		cFile.Open(strFileName, omAppend);
		strBuff = TCTime::Now() + ": ";
		strBuff += sLog;
		cFile.WriteLn(strBuff);
		cFile.Close();
	}
	catch (TCException& e)
	{
		printf("\nLog Exception:%s", (char*)e.GetExceptionMessage());
	}
	csLog.Leave();
}

//==========================================================================
// 函数 :FileFilterLog::AddFileFilterLogRunLog
// 用途 : 写运行日志
// 原型 : AddSimuATURunLog(char *sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::AddFileFilterLogRunLog(char* sLog)
{
	AddFileFilterLogRunLog(TCString(sLog));
}

//==========================================================================
// 函数 :FileFilterLog::DebugLog
// 用途 : 写运行日志
// 原型 : DebugLog(char *sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::DebugLog(char* sLog)
{
	if (g_FileFilterConfig->IsDebug())
		AddFileFilterLogRunLog(TCString(sLog));
}

//==========================================================================
// 函数 :FileFilterLog::DebugLog
// 用途 : 写运行日志
// 原型 : DebugLog(const TCString& sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::DebugLog(const TCString& sLog)
{
	if (g_FileFilterConfig->IsDebug())
		AddFileFilterLogRunLog(sLog);
}

//==========================================================================
// 函数 : FileFilterLog::AddNetMgrFileOperLog()
// 用途 : 写网管数据日志
// 原型 : AddNetMgrFileOperLog(const TCString& sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::AddNetMgrFileOperLog(const TCString& sLog, TCString sDay)
{
	csLog.Enter();
	try
	{
		TCString strFileName;
		TCString sNetMgrPath;
		TCString strAppName;
		TCString strBuff;
		TCFileStream cFile;
		strAppName = Application.GetAppName();
		sNetMgrPath = IncludeTrailingSlash(IncludeTrailingSlash(TAppPath::AppRoot()) + "NetMgr");

		TCString app_flag = Application.GetProcessFlag();
		sNetMgrPath += strAppName;
		sNetMgrPath += "_";
		sNetMgrPath += app_flag;
		// 
		ForceDirectories(sNetMgrPath);
		//
		TCString ip;
		ip = ProfileAppString(Application.GetAppName(), "GENERAL", "IP", "");;
		ip = (ip == "") ? "127.0.0.1" : ip;
		//: 修改文件名处理方式；
		strFileName = MergePathAndFile(sNetMgrPath, "F_" + ip + "_" + strAppName + "_" + app_flag + "_" + sDay + ".csv");

#ifdef __TEST__
		printf("网管文件 strFileName=[%s]\n", (char*)strFileName);
#endif

		//: 如果文件不存在，那么新建文件，并增加相应的文件头；
		if (!FileExists(strFileName)) {
			cFile.Open(strFileName, omAppend);
			//: 增加该文件头；
			TCString sFirstLine = "#文件名,原始记录条数,过滤后记录条数,开始时间,结束时间\n";
			strBuff = sFirstLine;
			cFile.WriteLn(strBuff);
		}
		else {
			cFile.Open(strFileName, omAppend);
		}

		strBuff = sLog;
		cFile.WriteLn(strBuff);
		cFile.Close();
	}
	catch (TCException& e)
	{
		printf("\nLog Exception:%s", (char*)e.GetExceptionMessage());
	}
	csLog.Leave();
}

//==========================================================================
// 函数 : FileFilterLog::AddNetMgrTableOperLog()
// 用途 : 写网管数据日志
// 原型 : AddNetMgrTableOperLog(const TCString& sLog)
// 参数 : sLog -- 要写入日志的内容
// 返回 : 无
// 说明 : 
//==========================================================================
void FileFilterLog::AddNetMgrTableOperLog(const TCString& sLog, TCString sDay)
{
	csLog.Enter();
	try
	{
		TCString strFileName;
		TCString sNetMgrPath;
		TCString strAppName;
		TCString strBuff;
		TCFileStream cFile;
		strAppName = Application.GetAppName();
		sNetMgrPath = g_FileFilterConfig->GetNetMgrPath();
		if (Right(AllTrim(sNetMgrPath), 1) != "/")
			sNetMgrPath = sNetMgrPath + "/";

		sNetMgrPath += strAppName;


		ForceDirectories(sNetMgrPath);
		//: 修改文件名处理方式；
		strFileName = MergePathAndFile(sNetMgrPath, "T_" + sDay + ".csv");

#ifdef __TEST__
		printf("网管文件 strFileName=[%s]\n", (char*)strFileName);
#endif

		//: 如果文件不存在，那么新建文件，并增加相应的文件头；
		if (!FileExists(strFileName)) {
			cFile.Open(strFileName, omAppend);
			//: 增加该文件头；
			TCString sFirstLine = "#小时,频次标示,表名,入库量\n";
			strBuff = sFirstLine;
			cFile.WriteLn(strBuff);
		}
		else {
			cFile.Open(strFileName, omAppend);
		}

		strBuff = sLog;
		cFile.WriteLn(strBuff);
		cFile.Close();
	}
	catch (TCException& e)
	{
		printf("\nLog Exception:%s", (char*)e.GetExceptionMessage());
	}
	csLog.Leave();
}

TCCriticalSection FileFilterLog::csLog;

