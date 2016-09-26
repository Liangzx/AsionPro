#ifndef _FILE_READER_H_
#define _FILE_READER_H_
#include "cmpublic.h"

#define MAX_LINE_LEN 4096

class FileReader
{
public:
	FileReader();
	~FileReader();
	bool load_filter_cfg(const std::string con_str);
	void filter_lines(std::vector<string> & filter_vec);
	bool start();
	bool read_file_lines(const TCString & file_name);
	std::string get_file_name(std::string file_path_name);
private:
	std::set<long> lac_ci_cfg_;
	std::set<std::string> vip_imsi_;
	std::vector<TCString> file_lines_;
	char field_separator_;

	// 配置的索引
	int ci_idx_;
	int city_id_idx_;
	int imsi_idx_;
	int max_idx_;	//取上述索引最大的一个用户判定数组越界

	// 过滤的cityid列表
	std::vector<TCString> city_ids_;
	// file name
	TCString src_file_path_name_;
};
#endif // !_FILE_READER_H_


