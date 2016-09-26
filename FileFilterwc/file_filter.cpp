#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <map>
#include "cmpublic.h"
#include "c_file_filter_config.h"
#include "c_file_filter_log.h"
#include "c_file_filter_public.h"
#include "c_file_reader.h"

void MainFunc();											// ������
void ReleaseFunc();											// �ͷź���
FileFilterConfig * g_FileFilterConfig;						// ȫ�������ļ�����
FileFilterLog * g_FileFilterLog;							// ȫ����־����
bool app_first_run_flag = false;							// �����״����б�ʶ

int main(int argc, char ** argv)
{
	string v_str;
	if (argc == 2)
	{
		v_str = argv[1];
	}
	if (argc == 2 && (v_str == "-v" || v_str == "-V" || v_str == "-Version" || v_str == "-version" || v_str == "v" || v_str == "V" || v_str == "Version" || v_str == "version"))
	{
		std::cout << "******************* ��ͨ�ೡ���û���֪--�ļ�ɸѡģ�� ************************************" << std::endl;
		std::cout << endl;
		std::cout << "Version: v2016.09.12" << endl;
		std::cout << "Starting mode: nohup [your app path]/FileFilter.bin -MutiP1 >> [your log path]/FileFilter.bin.output &" << std::endl;
		std::cout << endl;
		std::cout << "******************* ��ͨ�ೡ���û���֪--�ļ�ɸѡģ�� ************************************" << std::endl;
		std::cout << std::endl;
		return 0;
	}
	otl_connect::otl_initialize(1);

	// ���ö���̱�־--��������100������
	Application.MutiProcess(100);
	Application.Initialize("FileFilter", argc, argv);
	Application.SetRunningHandle(MainFunc);
	Application.InstallExitHandle(ReleaseFunc);

	Application.SetRunningDelay(2000);

	// ����config
	g_FileFilterConfig = new FileFilterConfig;
	// ����ȫ����־����
	g_FileFilterLog = new FileFilterLog;
	Application.Run();

	if (g_FileFilterConfig != NULL)
	{
		delete g_FileFilterConfig;
	}
	if (g_FileFilterLog != NULL) {
		delete g_FileFilterLog;
	}
	otl_connect::otl_terminate();
	return 0;
}

void MainFunc()
{
	// ��¼�����״�����ʱ��
	TCString log_str;
	if (!app_first_run_flag)
	{
		TCString m_sNowDate = "";
		app_first_run_flag = true;
		log_str = "MainFunc ��������ȡĿ¼�б���ǰʱ�� ";
		log_str += TCTime::Now();
		log_str += "] \n";
		g_FileFilterLog->AddFileFilterLogRunLog(log_str);
	}
	// ���ñ���سɹ����ж�ȡ���ļ�
	FileReader reader;
	reader.start();
}

void ReleaseFunc()
{
}
