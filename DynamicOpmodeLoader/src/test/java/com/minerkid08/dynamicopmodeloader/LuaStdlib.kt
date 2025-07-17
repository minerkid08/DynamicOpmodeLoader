package com.minerkid08.dynamicopmodeloader

import com.minerkid08.dynamicopmodeloader.error.CompileError
import com.minerkid08.dynamicopmodeloader.error.FunctionBuilderError
import com.minerkid08.dynamicopmodeloader.error.LuaError
import com.minerkid08.dynamicopmodeloader.error.UndefinedOpmodeError

class LuaStdlib
{
	fun print(string: String)
	{
		println(string);
	}

	fun luaErr(msg: String)
	{
		throw LuaError(msg);
	}

	fun cpErr(msg: String)
	{
		throw CompileError(msg);
	}

	fun fbErr(msg: String)
	{
		throw FunctionBuilderError(msg);
	}

	fun opmodeErr(msg: String)
	{
		throw UndefinedOpmodeError(msg);
	}
}
