package com.minerkid08.dynamicopmodeloader

import org.junit.Test

fun printf(fmt: String, vararg args: Any?)
{
    print(fmt.format(args));
}

class E
{
    fun doThing(callback: LuaCallback)
    {
        println("did thing");
        callback.call(69);
        println("called callback");
    }
}

fun run()
{
    val opmodeLoader = OpmodeLoader();

    val builder = opmodeLoader.getFunctionBuilder();

    val e = E();

    builder.setCurrentObject(e);
    builder.addObjectFunction("doThing", LuaType.Void, listOf(LuaType.Callback));

    val opmodes = opmodeLoader.init() ?: return;

    for (opmode in opmodes)
    {
        println("found opmode: $opmode");
    }

    opmodeLoader.loadOpmode(":)");

    opmodeLoader.start();

    opmodeLoader.close();
}

class Main
{
    @Test
    fun main()
    {
        OpmodeLoader.loadLibrary();
        //for (i in 1..20)
            run();
    }

    @Test
    fun fileServer()
    {
        FileServer.start();
        while (true);
    }
}