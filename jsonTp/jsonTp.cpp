// jsonTp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "sqlite3.h"
#include "CJsonObject.hpp"
#include "anLuaEngineer.h"

#include <optional>
#include <string>

static const char * DB_NAME = R"(D:\myProgramming\C\jsSysdig\2020-10-28-test\Rawu2DhZNO5W-7751-1-Backup3.db)";


static int query_cb(void * context, int colcount, char **colvalues, char **colnames) {
	neb::CJsonObject * result = reinterpret_cast<neb::CJsonObject*>(context);
	if (nullptr == result) {
		fprintf(stderr, "sqliteSelect2 query_cb result=null(%p)\n", result);
		return -1;
	}

	neb::CJsonObject row;
	for (int i = 0; i < colcount; ++i)
	{
		//row.Add(colnames[i], colvalues[i]);
		row.Add(colvalues[i]);
	}

	if (!row.IsEmpty()) {
		result->Add(row);
	}


	return 0;
}

static anLuaEngineer g_anlua;

#ifdef _DEBUG
const char * lua_path = R"(d:\myProgramming\C\myStudy\jsonTp\Debug\)";
#else
const char * lua_path = R"(d:\myProgramming\C\myStudy\jsonTp\Release\)";
#endif

// operator |
template <typename T, class F>
auto operator | (T &&param, const F &f)->decltype(f(std::forward<T>(param))) {
	return f(std::forward<T>(param));
}

int main(int argc, char* argv)
{
	int a = 10;
	const int b = 20;
	const int * pa = &a;

	*(int*)pa = 11;
	pa = &b;
	*(int*)pa = 21;

	std::optional<std::string> opt1(std::in_place, "c++17 I");
	std::cout << "opt1 = " << *opt1 << std::endl;
	//std::string ss = opt1.value();

	std::optional<std::string> opt2("c++17 II");
	std::cout << "opt2 = " << opt2.value() << std::endl;

	std::optional<std::string> opt3({"c++17 III"});
	std::cout << "opt3 = " << opt3.value() << std::endl;

	//
	auto add_one = [](auto a) {return 1 + a; };
	auto result = 2 | add_one;
	std::cout << "result = " << result << std::endl;

	int rc = g_anlua.init();

    std::cout << "anrong jsonTp init lua env " << rc << std::endl;
	
	std::string fn(lua_path);
	fn += "wjr_test4.lua";

	rc = g_anlua.load_luafile(fn.c_str());
	rc = g_anlua.run("main");
	

	sqlite3* db=nullptr;
	rc = sqlite3_open(DB_NAME, &db);
	if (SQLITE_OK!=rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	char * sqlErrMsg = nullptr;
	neb::CJsonObject json_root;

	const char * sql = R"(select DATA, TYPE from Backup order by ID limit 0,400;)";
	rc = sqlite3_exec(db, sql, query_cb, (void*)&json_root, &sqlErrMsg);
	if (SQLITE_OK != rc) {
		fprintf(stderr, "sqlite3_exec : %s error = (%d)\n", sqlErrMsg, rc);
		return -1;
	}

	/*neb::CJsonObject json_item;

	json_item.Add("DATA", "{\"dsp_ip:192.168.128.59\"}");
	json_item.Add("TYPE", "1");

	json_root.Add(json_item);

	json_item.Clear();
	json_item.Add("DATA", "{\"dsp_ip:192.168.128.75\"}");
	json_item.Add("TYPE", "1");

	json_root.Add(json_item);

	std::cout << "json_root is arrary : " << json_root.IsArray() << ", value = " << json_root.ToFormattedString() << std::endl;


	json_root.Clear();
	json_item.Clear();

	json_item.Add("{\"dsp_ip:192.168.128.59\"}");
	json_item.Add("{\"dsp_ip:192.168.128.75\"}");

	json_root.Add(json_item);*/

	
	//std::cout << "json_root is arrary : " << json_root.IsArray() << ", value = " << json_root.ToFormattedString() << ",size = " << json_root.GetArraySize() << std::endl;
	sqlite3_close(db);

	int rows = json_root.GetArraySize();
	for (int i = 0; i < rows; ++i) {
		std::string data,type;
		json_root[i].Get(0, data);
		json_root[i].Get(1, type);
		//json_root[i].Get("DATA", data);
		//json_root[i].Get("TYPE", type);
		std::cout << "json_root[" << i << "]= " << data << ", " << type << std::endl;
	}

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
