package com.minerkid08.dynamicopmodeloader

import org.junit.Test
import java.lang.Thread.sleep

class Action;

fun printf(fmt: String, vararg args: Any?)
{
    print(fmt.format(args));
}

class E
{
    fun doThing(i: Int, f: Float)
    {
        println("i $i, f $f");
    }
}

fun run()
{
    val opmodeLoader = OpmodeLoader();

    val builder = opmodeLoader.getFunctionBuilder();

    println(opmodeLoader);

    LuaAction.init(builder);
    LuaRobotActions.init(builder);

    val e = E();

    builder.setCurrentObject(e);
    builder.addObjectFunction("doThing", LuaType.Void, listOf(LuaType.Int, LuaType.Float));

    val opmodes = opmodeLoader.init() ?: return;

    for (opmode in opmodes)
    {
        println("found opmode: $opmode");
    }

    opmodeLoader.loadOpmode("testOpmode");

    opmodeLoader.start();

    opmodeLoader.close();
}

class Main
{
    @Test
    fun main()
    {
        System.loadLibrary("dynamicopmodeloader");
        for (i in 1..20)
            run();
    }

    @Test
    fun fileServer()
    {
        FileServer.start();
        while (true);
    }
}