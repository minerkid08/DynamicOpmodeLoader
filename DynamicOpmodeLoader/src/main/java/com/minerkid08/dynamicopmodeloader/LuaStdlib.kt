package com.minerkid08.dynamicopmodeloader

import android.util.Log

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