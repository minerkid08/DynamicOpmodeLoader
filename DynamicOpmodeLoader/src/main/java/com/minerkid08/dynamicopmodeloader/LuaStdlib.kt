package com.minerkid08.dynamicopmodeloader

import android.util.Log

class LuaStdlib
{
	fun print(string: String)
	{
		println(string);
	}
	
	fun err(msg: String)
	{
		throw LuaError(msg);
	}
}