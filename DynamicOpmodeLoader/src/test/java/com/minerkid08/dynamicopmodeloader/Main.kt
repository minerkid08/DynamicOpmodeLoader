package com.minerkid08.dynamicopmodeloader

import org.junit.Test

fun printf(fmt: String, vararg args: Any?)
{
	print(fmt.format(args));
}

class E
{
	@OpmodeLoaderFunction
	fun errorFun(i: Int): String
	{
		if(i == 1)
			throw LuaError("error :)");

		return "no error :)";
	}

	@OpmodeLoaderFunction
	fun doThing(callback: LuaCallback)
	{
		println("did thing");
		callback.call(69);
		println("called callback");
	}

	@OpmodeLoaderFunction
	fun enumThing(thing: Enum)
	{
		if(thing == Enum.Forward)
			println("fwd");
		else
			println("rev");
	}

	@OpmodeLoaderFunction
	fun add(a: Int, b: Double) = a.toDouble() + b;

	@OpmodeLoaderBuilderFunction
	fun printThing(a: String)
	{
		printf("printed thing $a\n");
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

class OpmodeGroup
{
	lateinit var name: String;
	val opmodes = ArrayList<Opmode>();
}

fun run()
{
	
	val opmodeLoader = OpmodeLoader();

	val builder = opmodeLoader.getFunctionBuilder();

	builder.addClassAsClass(E::class.java);
	builder.addStaticClassAsGlobal(F::class.java);
	builder.createClass("Enum");

	builder.pushTable("enum");
	builder.pushValueo("fwd", Enum.Forward);
	builder.pushValueo("bck", Enum.Backward);
	builder.popTable();

	val opmodes = opmodeLoader.init() ?: return;

	val telopGroups = ArrayList<OpmodeGroup>();
	val autoGroups = ArrayList<OpmodeGroup>();

	for (opmode in opmodes)
	{
		if(opmode.order == null)
			opmode.order = "";
		var added = false;
		val groups = if(opmode.type == Opmode.Telop) telopGroups else autoGroups;
		for(group in groups)
		{
			val name = if(opmode.group != null) opmode.group else "unsorted";
			if(group.name == name)
			{
				group.opmodes.add(opmode);
				added = true;
				break;
			}
		}
		if(added)
			continue;

		val group = OpmodeGroup();

		val name = if(opmode.group != null) opmode.group else "unsorted";
		group.name = name!!;
		group.opmodes.add(opmode);
		groups.add(group);
	}

	for(group in telopGroups)
		group.opmodes.sortWith { a, b -> a.order?.compareTo(b.order!!)!! };
	for(group in autoGroups)
		group.opmodes.sortWith { a, b -> a.order?.compareTo(b.order!!)!! };
	println("telop");
	for(group in telopGroups)
	{
		println("  ${group.name}");
		for(opmode in group.opmodes)
			println("    ${opmode.name}");
	}

	println("auto");
	for(group in autoGroups)
	{
		println("  ${group.name}");
		for(opmode in group.opmodes)
			println("    ${opmode.name}");
	}

	opmodeLoader.loadOpmode(":)");

	opmodeLoader.start();

	while(true)
	{
		val b = opmodeLoader.update();
		if(b)
			break;
	}

	opmodeLoader.stop();

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
}