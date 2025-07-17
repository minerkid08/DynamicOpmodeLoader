package com.minerkid08.dynamicopmodeloader.error

import kotlin.RuntimeException

class LuaError(private val msg: String) : RuntimeException()
{
    override fun getLocalizedMessage(): String
    {
        var i = msg.indexOf(':');
        i = msg.indexOf(':', i + 1) + 2;
        return msg.substring(i, msg.indexOf('\n'));
    }

    override fun getStackTrace(): Array<StackTraceElement>
    {
        val lines = msg.split('\n').drop(2);
        val trace = Array(lines.size, { i ->
            val line = lines[i];
            if (line[1] == '[')
            {
                val j = line.indexOf("'");
                val function = line.substring(j + 1, line.length - 1);
                StackTraceElement("", '\b' + function, "native function", 0)
            } else
            {
                val k = line.indexOf(':');
                val file = line.substring(1, k);
                val j = line.indexOf("'");
                val function = if (j == -1) "main chunk";
                else line.substring(j + 1, line.length - 1);
                val num = line.substring(k + 1, line.indexOf(':', k + 1));

                StackTraceElement("", '\b' + function, file, num.toInt())
            }
        });
        return trace;
    }
}

class CompileError(private val msg: String) : RuntimeException()
{
    override fun getLocalizedMessage(): String
    {
        return msg;
    }
}

class FunctionBuilderError(private val msg: String) : RuntimeException()
{
    override fun getLocalizedMessage(): String
    {
        return msg;
    }
}

class UndefinedOpmodeError(private val msg: String) : RuntimeException()
{
    override fun getLocalizedMessage(): String
    {
        return msg;
    }
}
