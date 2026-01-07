package com.minerkid08.dynamicopmodeloader

import kotlin.RuntimeException

class LuaRuntimeError(message: String) : RuntimeException()
{
	private var msg: String;
	private var trace: Array<StackTraceElement> =
		Array(0, { StackTraceElement("", "", "", 0) });

	init
	{
		try
		{
			var ind = message.indexOf(':');
			ind = message.indexOf(':', ind + 1) + 2;
			msg = message.substring(ind, message.indexOf('\n'));

			val lines = message.split('\n').drop(2);
			trace = Array(lines.size, { i ->
				val line = lines[i];
				if (line[1] == '[')
				{
					val j = line.indexOf('\'');
					val function = line.substring(j + 1, line.length - 1);
					StackTraceElement("", '\b' + function, "native function", 0)
				}
				else
				{
					val k = line.indexOf(':');
					val file = line.substring(1, k);
					val j = line.indexOf('\'');
					val function = if (j == -1) "main chunk";
					else line.substring(j + 1, line.length - 1);
					val num = line.substring(k + 1, line.indexOf(':', k + 1));

					StackTraceElement("", '\b' + function, file, num.toInt())
				}
			});
			stackTrace = trace;
		}
		catch (e: RuntimeException)
		{
			msg = message;
		}
	}

	override fun getLocalizedMessage() = msg;
}

class LuaCompileError(private val msg: String) : RuntimeException()
{
	override fun getLocalizedMessage() = msg;
}

class FunctionBuilderError(private val msg: String) : RuntimeException()
{
	override fun getLocalizedMessage() = msg;
}

class UndefinedOpmodeError(private val msg: String) : RuntimeException()
{
	override fun getLocalizedMessage() = msg;
}

class LuaError(private val msg: String) : RuntimeException()
{
	override fun getLocalizedMessage() = msg;
}
