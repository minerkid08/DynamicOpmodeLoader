package com.minerkid08.dynamicopmodeloader

import android.util.Log
import com.minerkid08.dynamicopmodeloader.error.CompileError
import com.minerkid08.dynamicopmodeloader.error.FunctionBuilderError
import com.minerkid08.dynamicopmodeloader.error.LuaError
import com.minerkid08.dynamicopmodeloader.error.UndefinedOpmodeError

/**
 * internal class for managing print statements and errors
 */
class LuaStdlib
{
	fun print(string: String)
	{
		Log.d("lua", string);
	}
	
	fun luaErr(msg: String)
	{
		Log.e("lua", msg);
		throw LuaError(msg);
	}

	fun cpErr(msg: String)
	{
		Log.e("lua", msg);
		throw CompileError(msg);
	}

	fun fbErr(msg: String)
	{
		Log.e("function builder", msg);
		throw FunctionBuilderError(msg);
	}

	fun opmodeErr(msg: String)
	{
		Log.e("undefined opmode", msg);
		throw UndefinedOpmodeError(msg);
	}
}