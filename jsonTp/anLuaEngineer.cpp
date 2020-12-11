
#include "anLuaEngineer.h"
#include <assert.h>

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "luajit.lib")

anLuaEngineer::anLuaEngineer() : L_(nullptr) {

}

anLuaEngineer::~anLuaEngineer() {
	if (L_) {
		lua_close(L_);
	}
}

int anLuaEngineer::init()
{
	if (nullptr == L_) {

		//create lua state
		L_ = luaL_newstate();

		if (nullptr == L_) {
			return -1;
		}

		//load lua libs
		luaL_openlibs(L_);

	}

	return 0;
}


int anLuaEngineer::load_luafile(const char * filename) {
	int rc = 0;

	assert(nullptr != L_);

	if (nullptr == filename) {
		return -1;
	}

	//load file
	rc = luaL_loadfile(L_, filename);
	if (0==rc) {

		//load run
		rc = lua_pcall(L_, 0, 0, 0);
	}

	return rc;
}

int anLuaEngineer::run(const char * fn)
{
	int rc = 0;
	
	assert(nullptr != L_);
	if (nullptr == fn) {
		return -1;
	}

	lua_getglobal(L_, fn);
	rc = lua_pcall(L_, 0, 1, 0);
	if (rc) {
		fprintf_s(stderr, "***** lua_pcall %s error %s(%d)\n", fn, lua_tostring(L_, -1), rc);
	}

	return rc;

}

