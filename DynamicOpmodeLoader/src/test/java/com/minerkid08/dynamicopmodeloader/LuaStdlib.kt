package com.minerkid08.dynamicopmodeloader

class LuaStdlib
{
	fun print(string: String)
	{
		println(string);
	}
	
	fun err(msg: String)
	{
		println(msg);
		throw LuaError(msg);
	}
}