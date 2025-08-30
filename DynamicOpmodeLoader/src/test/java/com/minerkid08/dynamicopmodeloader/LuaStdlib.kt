package com.minerkid08.dynamicopmodeloader

object LuaStdlib
{
	@JvmStatic
	fun print(string: String)
	{
		println(string);
	}

	fun log(tag: String, value: String)
	{
		println("%10s | %s".format(tag, value));
	}
}
