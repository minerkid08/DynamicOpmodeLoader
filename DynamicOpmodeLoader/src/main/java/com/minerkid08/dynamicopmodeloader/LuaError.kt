package com.minerkid08.dynamicopmodeloader

import java.lang.RuntimeException

class LuaError(private var msg: String) : RuntimeException()
{
	override fun getLocalizedMessage(): String
	{
		return msg;
	}
}