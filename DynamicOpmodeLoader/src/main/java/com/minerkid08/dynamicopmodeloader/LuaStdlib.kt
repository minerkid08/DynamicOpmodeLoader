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
		Log.e("lua", msg);
		throw LuaError(msg);
	}
}