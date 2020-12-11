#pragma once


#include <stdlib.h>
#include "lua.hpp"
#include "CJsonObject.hpp"


class anLuaEngineer
{
public:
	anLuaEngineer();
	~anLuaEngineer();

	int init();

	lua_State * getluaState() {
		return L_;
	}

	int load_luafile(const char * filename);
	int run(const char * fn);
private:
	lua_State * L_;

};
