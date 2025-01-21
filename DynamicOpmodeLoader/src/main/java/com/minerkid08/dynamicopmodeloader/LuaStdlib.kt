package com.minerkid08.dynamicopmodeloader

import android.util.Log

class LuaStdlib
{
	fun print(string: String)
	{
		Log.d("lua", string);
	}
	
	fun err(msg: String)
	{
		throw LuaError(msg);
	}
	
	fun stop(): Boolean
	{
		return true;
	}
}