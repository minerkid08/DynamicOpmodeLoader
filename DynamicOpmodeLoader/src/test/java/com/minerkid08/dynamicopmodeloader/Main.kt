package com.minerkid08.dynamicopmodeloader

import org.junit.Test

fun printf(fmt: String, vararg args: Any?)
{
	print(fmt.format(args));
}

class E
{
	@OpmodeLoaderFunction
	fun doThing(callback: LuaCallback)
	{
		println("did thing");
		callback.call(69);
		println("called callback");
	}

	@OpmodeLoaderFunction
	fun add(a: Int, b: Double) = a.toDouble() + b;

	@OpmodeLoaderBuilderFunction
	fun printThing(a: String)
	{
		printf("printed thing $a");
	}
}

object F
{
	@JvmStatic
	@OpmodeLoaderFunction
	fun getE() = E();
}

enum class Enum
{
	Forward, Backward
}

fun run()
{
	
	val opmodeLoader = OpmodeLoader();

	val builder = opmodeLoader.getFunctionBuilder();

	builder.addClassAsClass(E::class.java);
	builder.addStaticClassAsGlobal(F::class.java);

	val opmodes = opmodeLoader.init() ?: return;

	for (opmode in opmodes)
	{
		println("found opmode: $opmode");
	}

	opmodeLoader.loadOpmode(":)");

	opmodeLoader.start();

	while(true)
	{
		val b = opmodeLoader.update();
		if(b)
			break;
	}

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
		FileServer.path = "./upload";
		FileServer.start();
		while (true);
	}
}