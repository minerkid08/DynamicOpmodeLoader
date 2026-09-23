package com.minerkid08.dynamicopmodeloader

import android.util.Log

/**
 * internal class for managing print statements and errors
 */
object LuaStdlib
{
	@JvmStatic
	fun print(string: String)
	{
		Log.d("lua", string);
	}

	fun log(tag: String, string: String)
	{
		Log.d(tag, string);
	}
}