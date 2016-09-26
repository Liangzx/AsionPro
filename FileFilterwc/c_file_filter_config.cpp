#include "c_file_filter_config.h"



FileFilterConfig::FileFilterConfig()
{
	LoadIniFile();
}


FileFilterConfig::~FileFilterConfig()
{
}

void FileFilterConfig::LoadIniFile()
{
	TCString  sReadStr;
	m_bDebugLog = false;

	// ���г��������
	m_sHostName = ProfileAppString(Application.GetAppName(), "GENERAL", "HostName", "");
	m_sHostName = AllTrim(m_sHostName);
	m_sHostName = TrimCRLF(m_sHostName);

	// ����IP
	m_ip = ProfileAppString(Application.GetAppName(), "GENERAL", "IP", "127.0.0.1");
	m_ip = AllTrim(m_ip);
	m_ip = TrimCRLF(m_ip);

	// ���˵�city id ��Ϊ�վͲ�����
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "FILTER_CITY_ID", "");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);

	TCStringList city_id_list;
	m_city_id.clear();
	city_id_list.CommaText(sReadStr, '|');
	for (int i = 0; i < city_id_list.GetCount(); i ++)
	{
		m_city_id.push_back(city_id_list[0]);
	}
	
	// city_id index
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "CITY_ID_IDX", "-1");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_city_id_idx = StrToInt(sReadStr);
	assert(m_city_id_idx != -1);


	// ci index
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "CELL_ID_IDX", "-1");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_ci_idx = StrToInt(sReadStr);
	assert(m_ci_idx != -1);

	// ci index
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "IMSI_IDX", "-1");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_imsi_idx = StrToInt(sReadStr);
	assert(m_ci_idx != -1);

	// file name filter string
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "FILE_NAME_FILTER", "");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_name_filter = sReadStr;

	// ���ݿ����Ӵ�
	m_sDB_ConStr = ProfileAppString(Application.GetAppName(), "GENERAL", "DB_CONSTR", "");
	m_sDB_ConStr = AllTrim(m_sDB_ConStr);
	m_sDB_ConStr = TrimCRLF(m_sDB_ConStr);
	if (m_sDB_ConStr == "")
	{
		printf("�������ݿ�����ݴ�δ���ã����������ļ��е�DB_CONSTR�ֶ�\n");
		exit(-1);
	}

	// ���ݿ��ռ�
	m_sTableSpaceName = ProfileAppString(Application.GetAppName(), "GENERAL", "DB_TABLE_SPACE", "");

	// ����ԴĿ¼
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "SRC_DIR", "");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_sSrc_Path = sReadStr;
	if (m_sSrc_Path == "")
	{
		printf("����Ŀ¼δָ��,�����ļ���SRC_DIR�����ֶ�\n");
		exit(-1);
	}

	// ����Ŀ¼
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "WORK_DIR", "");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_sWork_Path = sReadStr;
	if (m_sWork_Path == "")
	{
		printf("����Ŀ¼δָ��,�����ļ���WORK_DIR�����ֶ�\n");
		exit(-1);
	}
	// �������Ŀ¼�����ڣ���ô������Ŀ¼��
	if (!DirectoryExists(m_sWork_Path))
	{
		ForceDirectories(m_sWork_Path);
	}

	// Ŀ���ļ�Ŀ¼
	sReadStr = ProfileAppString(Application.GetAppName(), "GENERAL", "DEST_DIR", "");
	sReadStr = AllTrim(sReadStr);
	sReadStr = TrimCRLF(sReadStr);
	m_sDest_Path = sReadStr;
	if (m_sDest_Path == "")
	{
		printf("Ŀ���ļ�Ŀ¼δָ��,�����ļ���DEST_DIR�����ֶ�\n");
		exit(-1);
	}
	// �������Ŀ¼�����ڣ���ô������Ŀ¼��
	if (!DirectoryExists(m_sDest_Path))
	{
		ForceDirectories(m_sDest_Path);
	}

	// ����Ŀ¼
	m_sBack_Path = ProfileAppString(Application.GetAppName(), "GENERAL", "BAK_DIR", "");
	if (m_sBack_Path == "")
	{
		printf("Ŀ���ļ�Ŀ¼δָ��,�����ļ���BAK_DIR�����ֶ�\n");
		exit(-1);
	}
	// �������Ŀ¼�����ڣ���ô������Ŀ¼��
	if (!DirectoryExists(m_sBack_Path))
	{
		ForceDirectories(m_sBack_Path);
	}
	
	// �����ļ���Ŀ¼
	m_sNetMgr = TAppPath::AppRoot();
	if (Right(m_sNetMgr, 1) != "/")
		m_sNetMgr += "/";
	m_sNetMgr = m_sNetMgr + "NetMgr";
	if (!DirectoryExists(m_sNetMgr)) {
		ForceDirectories(m_sNetMgr);
	}
}
