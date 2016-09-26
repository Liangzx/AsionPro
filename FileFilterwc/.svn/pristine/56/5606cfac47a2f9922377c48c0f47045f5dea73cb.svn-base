#ifndef _FILE_FILTER_CONFIG_H_
#define _FILE_FILTER_CONFIG_H_
#include "cmpublic.h"
#include <set>


class FileFilterConfig
{
public:
	FileFilterConfig();
	~FileFilterConfig();
	TCString GetCTDBConnStr() { return m_sDB_ConStr; }
	TCString GetHostName() { return m_sHostName; }
	TCString GetNetMgrPath() { return m_sNetMgr; }
	bool IsDebug() { return m_bDebugLog; }
	TCString GetWorkDir() { return m_sWork_Path; }
	TCString GetBakDir() { return m_sBack_Path; }
	TCString GetDestDir() { return m_sDest_Path; }
	TCString GetSrcDir() { return m_sSrc_Path; }
	TCString GetIp() { return m_ip; }
	TCString GetFileFilterName() { return m_name_filter; }
	std::vector<TCString> GetCityIDs() { return m_city_id; }
	int GetCiIdx() { return m_ci_idx; }
	int GetCityIdIdx() { return m_city_id_idx; }
	int GetImsiIdx() { return m_imsi_idx; }

private:
private:
	void LoadIniFile();
	bool m_bDebugLog;																			// 是否打开调试
	TCString m_sDB_ConStr;																		// 数据库连接串
	TCString m_sHostName;																		// 运行该程序的主机IP
	TCString m_sWork_Path;																		// 该程序运行的工作目录	
	TCString m_sBack_Path;																		// 备份目录
	TCString m_sSrc_Path;																		// 数据源目录
	TCString m_sDest_Path;																		// 过滤后得文件目录
	TCString m_sNetMgr;																			// 网管
	TCString m_ip;																				// ip
	TCString m_sTableSpaceName;																	// 表空间
	TCString m_name_filter;																		// filter file name
	int m_tac_idx;																				// lac index
	int m_ci_idx;																				// ci index
	int m_city_id_idx;																			// city_id index
	int m_imsi_idx;																				// imsi index
	std::vector<TCString> m_city_id;															// 过滤的city id
};
#endif // !_FILE_FILTER_CONFIG_H_



