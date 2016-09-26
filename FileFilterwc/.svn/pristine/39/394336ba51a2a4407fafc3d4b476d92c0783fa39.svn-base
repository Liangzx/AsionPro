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

void MainFunc();											// 主函数
void ReleaseFunc();											// 释放函数
FileFilterConfig * g_FileFilterConfig;						// 全局配置文件对象
FileFilterLog * g_FileFilterLog;							// 全局日志对象
bool app_first_run_flag = false;							// 程序首次运行标识

int main(int argc, char ** argv)
{
	string v_str;
	if (argc == 2)
	{
		v_str = argv[1];
	}
	if (argc == 2 && (v_str == "-v" || v_str == "-V" || v_str == "-Version" || v_str == "-version" || v_str == "v" || v_str == "V" || v_str == "Version" || v_str == "version"))
	{
		std::cout << "******************* 联通多场景用户感知--文件筛选模块 ************************************" << std::endl;
		std::cout << endl;
		std::cout << "Version: v2016.09.12" << endl;
		std::cout << "Starting mode: nohup [your app path]/FileFilter.bin -MutiP1 >> [your log path]/FileFilter.bin.output &" << std::endl;
		std::cout << endl;
		std::cout << "******************* 联通多场景用户感知--文件筛选模块 ************************************" << std::endl;
		std::cout << std::endl;
		return 0;
	}
	otl_connect::otl_initialize(1);

	// 设置多进程标志--最所允许100个进程
	Application.MutiProcess(100);
	Application.Initialize("FileFilter", argc, argv);
	Application.SetRunningHandle(MainFunc);
	Application.InstallExitHandle(ReleaseFunc);

	Application.SetRunningDelay(2000);

	// 加载config
	g_FileFilterConfig = new FileFilterConfig;
	// 创建全局日志对象
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
	// 记录程序首次运行时间
	TCString log_str;
	if (!app_first_run_flag)
	{
		TCString m_sNowDate = "";
		app_first_run_flag = true;
		log_str = "MainFunc 启动，获取目录列表，当前时间 ";
		log_str += TCTime::Now();
		log_str += "] \n";
		g_FileFilterLog->AddFileFilterLogRunLog(log_str);
	}
	// 配置表加载成功按行读取问文件
	FileReader reader;
	reader.start();
}

void ReleaseFunc()
{
}
