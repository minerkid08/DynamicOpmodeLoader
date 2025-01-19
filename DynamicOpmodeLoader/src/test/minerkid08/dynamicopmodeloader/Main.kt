package com.minerkid08.dynamicopmodeloader

class Action(private val label: String)
{
	fun run()
	{
		println("running $label");
	}
	
	companion object
	{
		fun runAction(a: Action)
		{
			a.run();
		}
	}
}

class TrajectoryBuilder
{
	fun init(x: Double, y: Double, h: Double)
	{
		println("start $x, $y, $h");
	}
	
	fun lineToX(end: Double)
	{
		println("lineToX $end");
	}
	
	fun setTangent(tangent: Double)
	{
		println("setTan $tangent");
	}
	
	fun splineToConstantHeading(x: Double, y: Double, t: Double)
	{
		println("spline $x, $y, $t");
	}
	
	fun build(): Action
	{
		println("building");
		return Action("trajectory");
	}
}

class TestObj
{
	fun dothing(): String
	{
		println("did thing");
		return "hi";
	}
}

class TestObj2(val e: TestObj)
{
	fun getThing(): TestObj
	{
		return e;
	}
	
	fun trajectoryAction(x: Double, y: Double, h: Double): TrajectoryBuilder
	{
		val b = TrajectoryBuilder();
		b.init(x, y, h);
		return b;
	}
}

fun main()
{
	val opmodeLoader = OpmodeLoader();
	val opmodes = opmodeLoader.init() ?: return;
	
	val builder = opmodeLoader.getFunctionBuilder();
	val obj = TestObj();
	val obj2 = TestObj2(obj);
	
	builder.setCurrentObject(obj2);
	builder.objectAddFun("getThing", LuaType.Object(TestObj::class.java));
	builder.objectAddFun(
		"trajectoryAction",
		LuaType.Object(TrajectoryBuilder::class.java),
		listOf(LuaType.Double(), LuaType.Double(), LuaType.Double())
	);
	
	builder.setCurrentObject(Action);
	builder.objectAddFun("runAction", LuaType.Void(), listOf(LuaType.Object(Action::class.java)));
	
	builder.classAddFun(TestObj::class.java, "dothing", LuaType.String());
	
	builder.createClass("Action");
	
	builder.classAddFun(
		TrajectoryBuilder::class.java,
		"lineToX",
		LuaType.Void(),
		listOf(LuaType.Double())
	);
	builder.classAddFun(
		TrajectoryBuilder::class.java,
		"setTangent",
		LuaType.Void(),
		listOf(LuaType.Double())
	);
	builder.classAddFun(
		TrajectoryBuilder::class.java,
		"splineToConstantHeading",
		LuaType.Void(),
		listOf(LuaType.Double(), LuaType.Double(), LuaType.Double())
	);
	builder.classAddFun(TrajectoryBuilder::class.java, "build", LuaType.Object(Action::class.java));
	
	for(opmode in opmodes)
	{
		println("found opmode: $opmode");
	}
	
	opmodeLoader.loadOpmode(opmodes[0]);
	
	return;
	
	opmodeLoader.start();
	
	val startTime = System.currentTimeMillis();
	var lastFrameTime = startTime;
	while(true)
	{
		val currentTime = System.currentTimeMillis();
		val elapsedTime = currentTime - startTime;
		val deltaTime = currentTime - lastFrameTime;
		
		opmodeLoader.update(deltaTime / 1000.0, elapsedTime / 1000.0);
		
		lastFrameTime = currentTime;
	}
}