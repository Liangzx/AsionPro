#define OTL_STL

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <numeric>      // std::accumulate
#include <ctype.h>
#include <set>
#include <string>
#include <time.h>
#include "c_file_reader.h"
#include "c_file_filter_log.h"
#include "c_file_filter_config.h"
#include "c_file_filter_public.h"

extern FileFilterConfig * g_FileFilterConfig;						// ȫ�������ļ�����
extern FileFilterLog * g_FileFilterLog;								// ȫ����־����
static otl_connect db_con;

static bool put_filter_res(const std::string hide_name, const std::string & file_path_name, const std::vector<string>& filter_vec);

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

bool FileReader::load_filter_cfg(const std::string con_str)
{
	otl_stream ots;
	__ENTERFUNCTION;
	lac_ci_cfg_.clear();	
	// ��½���ݿ�
	if (con_str.empty()) {
		return false;
	}
	db_con.rlogon(con_str.c_str());
	// �������ñ�mb_highwaycellinfo(cellid) mb_indoorcellinfo(cellid) mb_VIPuser(imsi)--��ȡcellid
	std::string tb_highwaycellinfo = "MB_HIGHWAYCELLINFO";
	std::string tb_indoorcellinfo = "MB_INDOORCELLINFO";

	std::string sql_buf = "SELECT CELLID FROM ";
	sql_buf += tb_highwaycellinfo;
	sql_buf += " UNION SELECT CELLID FROM ";
	sql_buf += tb_indoorcellinfo;

	ots.open(100, sql_buf.c_str(), db_con);
	long cell_id = 0;
	while (!ots.eof())
	{
		ots >> cell_id;
		lac_ci_cfg_.insert(cell_id);
	}
	ots.close();
	// ��ȡvip imsi
	std::string tb_vip_user = "MB_VIPUSER";

	sql_buf = "SELECT IMSI FROM ";
	sql_buf += tb_vip_user;
	ots.open(100, sql_buf.c_str(), db_con);
	std::string vip_imsi = "";
	vip_imsi_.clear();
	while (!ots.eof())
	{
		vip_imsi.empty();
		ots >> vip_imsi;
		vip_imsi_.insert(vip_imsi);
	}
	ots.close();
	// �˳����ݿ�
	db_con.logoff();

	LOG_WRITE("[%s]load config tables--cell_id records:%d,vip users:%d",(char *)TCTime::Now() ,lac_ci_cfg_.size(), vip_imsi_.size());

	return true;
	__LEAVEFUNCTION;
	ots.close();
	return false;
}

void FileReader::filter_lines(std::vector<string> & filter_vec)
{
	filter_vec.clear();
	TCStringList line_list;
	TCString city_id;
	TCString imsi;

	// ���ж����ڴ��е��н��й���	
	for (int i = 0; i < file_lines_.size(); i ++)
	{
		line_list.Clear();
		city_id = "";		
		

		line_list.CommaText(file_lines_[i], field_separator_);
		if (max_idx_ > line_list.GetCount() || (line_list.GetCount() == 0))
		{
			continue;
		}

		// �����ж� city id
		city_id = line_list[city_id_idx_];
		
		if (std::find(city_ids_.begin(), city_ids_.end(), city_id) == city_ids_.end()) 
		{
			continue;
		}
		// ȷ��city id ���б�������ٰ�cellid || imsi����
		long cell_id = StrToInt(line_list[ci_idx_]);

		if (std::find(lac_ci_cfg_.begin(), lac_ci_cfg_.end(), cell_id) != lac_ci_cfg_.end()
			|| std::find(vip_imsi_.begin(), vip_imsi_.end(), (char *)line_list[imsi_idx_]) != vip_imsi_.end())
		{
			filter_vec.push_back((char *)file_lines_[i]);
		}
	}
	LOG_WRITE("[%s][%s]�ܵļ�¼��:%d,���˺�ļ�¼��:%d", (char *)TCTime::Now(),(char *)src_file_path_name_, file_lines_.size(), filter_vec.size());
}

bool FileReader::start()
{
	// �������ñ�--�������¼���
	std::string con_str = (char *)g_FileFilterConfig->GetCTDBConnStr();
	if (!load_filter_cfg(con_str))
	{
		fprintf(stderr, "load config table fail.\n");
		return false;
	}

	const TCString src_dir = g_FileFilterConfig->GetSrcDir();
	const TCString dest_dir = g_FileFilterConfig->GetDestDir();
	const TCString bak_dir = g_FileFilterConfig->GetBakDir();
	const std::string file_filter_name = (char *)g_FileFilterConfig->GetFileFilterName();
	const TCString file_suffix_filter = "*.CSV";
	field_separator_ = '|';

	// ��ȡ�����ļ���Ӧ������
	max_idx_ = -1;

	ci_idx_ = g_FileFilterConfig->GetCiIdx();
	max_idx_ = ci_idx_ > max_idx_ ? ci_idx_ : max_idx_;

	city_id_idx_ = g_FileFilterConfig->GetCityIdIdx();
	max_idx_ = city_id_idx_ > max_idx_ ? city_id_idx_ : max_idx_;

	imsi_idx_ = g_FileFilterConfig->GetImsiIdx();
	max_idx_ = imsi_idx_ > max_idx_ ? imsi_idx_ : max_idx_;

	city_ids_ = g_FileFilterConfig->GetCityIDs();

	// ��ȡ�ļ��б�
	TCStringList files;
	// �ļ����˺󱣴������
	std::vector<std::string> file_filter_lines;
	TCString old_day = TCTime::Today();

	while (true)
	{
		// �ж��Ƿ����¼�������--һ�����һ��
		if (old_day != TCTime::Today())
		{
			old_day = TCTime::Today();
			if (!load_filter_cfg(con_str))
			{
				fprintf(stderr, "load config table fail.\n");
				return false;
			}
		}
		files.Clear();
		GetDirFileList(files, (char *)src_dir, file_suffix_filter, false, true);
		if (files.GetCount() == 0)
		{
			sleep(5);
			continue;
		}

		// �ļ�����
		// ���ļ�����
		vector<TCString> filesVec;
		int file_cnt = files.GetCount();
		for (int i = 0; i < file_cnt; i++)
		{
			filesVec.push_back(AllTrim(files[i]));
		}
		// �������ļ���������
		stable_sort(filesVec.begin(), filesVec.end(), MinTCString);

		// ɨ��һ��Ŀ¼������ɨ�赽���ļ���������ٽ����´�ɨ��
		for (int i = 0; i < file_cnt; i++) {
			// �ļ����ļ���
			src_file_path_name_ = filesVec[i];

			// TODO:�����ļ���ɸѡ�ļ������ϵ�ֱ��mv��bak--file_filter_nameΪ���򲻽����ļ����Ĺ���
			if (!file_filter_name.empty()
				&& get_file_name((char *)src_file_path_name_).find(file_filter_name) == std::string::npos)
			{
				// S1U ����103��ͷ���ļ�
				if (get_file_name((char *)src_file_path_name_).substr(0, 4) != "103_") {
					TCString s_log = '[' + TCTime::Now() + ']';
					s_log += "file name dismatch:";
					s_log += src_file_path_name_;
					g_FileFilterLog->AddFileFilterLogRunLog(s_log);

					MoveFile(src_file_path_name_, bak_dir);
					LOG_WRITE("[%s][%s]�������ļ���ɸѡ����mv��bakĿ¼", (char *)TCTime::Now(), (char *)src_file_path_name_);
					continue;
				}
			}

			// �ļ�������ɸѡ������ʱ�ж��ļ���С�Ƿ�Ϊ0,Ϊ0ֱ��mv��Ŀ���ļ�����
			if (FileSize(src_file_path_name_) == 0)
			{
				// ��copyһ�ݵ�Ŀ��Ŀ¼��mv��bakĿ¼
				CopyFile(src_file_path_name_, IncludeTrailingSlash(dest_dir) + (char *)get_file_name((char *)src_file_path_name_).c_str());
				MoveFile(src_file_path_name_, bak_dir);
				LOG_WRITE("[%s][%s]��СΪ0��desĿ¼�����ļ���mv��bakĿ¼", (char *)TCTime::Now(), (char *)src_file_path_name_);
				continue;
			}


			TCString time_b = TCTime::Now();
			// ÿ�ζ�ȡ��һ���ļ����ڴ�
			if (!read_file_lines(src_file_path_name_))
			{
				continue;
			}

			// ���ڴ��е��ļ����н��й���	
			filter_lines(file_filter_lines);

			// ������˺���ļ���Ŀ���ļ� -- ��д�������ļ���rename
			std::string hide_name = (char *)IncludeTrailingSlash(dest_dir);
			hide_name += '.';
			hide_name += get_file_name((char *)src_file_path_name_);
			std::string des_file_path_name = (char *)IncludeTrailingSlash(dest_dir) + get_file_name((char *)src_file_path_name_);

			put_filter_res(hide_name, des_file_path_name, file_filter_lines);

			// ���ļ�ûmv��bakĿ¼
			MoveFile(src_file_path_name_, bak_dir);
			TCString time_e = TCTime::Now();

			// ������־"#�ļ���,ԭʼ��¼����,���˺��¼����,��ʼʱ��,����ʱ��"
			TCString mgr_log = src_file_path_name_ + ',';
			mgr_log += IntToStr(file_lines_.size()) + ',';
			mgr_log += IntToStr(file_filter_lines.size()) + ',';
			mgr_log += time_b + ',';
			mgr_log += time_e;
			g_FileFilterLog->AddNetMgrFileOperLog(mgr_log, old_day);
		}		
	}
	return true;
}

bool FileReader::read_file_lines(const TCString & file_name)
{
	file_lines_.clear();

	std::fstream ifs((char *)file_name, std::ifstream::in);
	char buf[MAX_LINE_LEN] = { 0 };
	if (ifs.fail())
	{
		fprintf(stderr, "open file[%s] failed.\n", (char *)file_name);
		ifs.close();
		return false;
	}
	while (ifs.getline(buf, MAX_LINE_LEN))
	{
		file_lines_.push_back(buf);
	}
	ifs.close();

	TCString s_log = '[' + TCTime::Now() + ']';
	s_log += "read file:";
	s_log += file_name;
	g_FileFilterLog->AddFileFilterLogRunLog(s_log);
	return true;
}

std::string FileReader::get_file_name(std::string file_path_name)
{
	TCStringList file_name;
	file_name.CommaText((char *)file_path_name.c_str(), '/');
	int name_idx = file_name.GetCount() - 1;
	TCString fname = file_name[name_idx];
	return std::string((char *)fname);
}

static bool put_filter_res(const std::string hide_name, const std::string & file_path_name, const std::vector<string>& filter_vec)
{

	std::fstream ofs(hide_name.c_str(), std::fstream::out | std::fstream::trunc);
	if (ofs.fail()) {
		fprintf(stderr, "open file[%s] failed\n", hide_name.c_str());
		ofs.close();
		return false;
	}
	for (int i = 0; i < filter_vec.size(); i++) {
		ofs << filter_vec[i] << std::endl;
	}
	ofs.close();

	// �������ļ�������Ŀ���ļ�
	if (!file_path_name.empty()) {
		rename(hide_name.c_str(), file_path_name.c_str());
	}

	TCString s_log = '[' + TCTime::Now() + ']';
	s_log += "put filter file:";
	s_log += (char *)file_path_name.c_str();
	g_FileFilterLog->AddFileFilterLogRunLog(s_log);

	return true;
}