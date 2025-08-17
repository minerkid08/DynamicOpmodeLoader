package com.minerkid08.dynamicopmodeloader.error

import kotlin.RuntimeException

class LuaError(message: String) : RuntimeException()
{
	private val msg: String;
	private val trace: Array<StackTraceElement>;

	init
	{
		var i = message.indexOf(':');
		i = message.indexOf(':', i + 1) + 2;
		msg = message.substring(i, message.indexOf('\n'));

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

	override fun getLocalizedMessage() = msg;

	override fun printStackTrace()
	{
		val stackTrace = trace;
		var str = localizedMessage + '\n';
		for (elem in stackTrace)
			str += elem.toString() + '\n';
		print(str)
	}
}

class CompileError(private val msg: String) : RuntimeException()
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
