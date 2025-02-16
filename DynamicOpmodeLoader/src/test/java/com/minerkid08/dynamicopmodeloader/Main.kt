package com.minerkid08.dynamicopmodeloader

import org.junit.Test
import java.lang.Thread.sleep

class Action;

class LuaTrajectoryBuilder()
{
	companion object
	{
		fun init(builder: FunctionBuilder)
		{
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"setTangent",
				LuaType.Builder,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToX",
				LuaType.Builder,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToY",
				LuaType.Builder,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToLinearHeading",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToConstantHeading",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"build",
				LuaType.Object(Action::class.java)
			);
		}
	}

	fun setTangent(tangent: Double)
	{
		println("set tangent $tangent");
	}

	fun lineToX(x: Double)
	{
		println("line to x $x");
	}

	fun lineToY(y: Double)
	{
		println("line to y $y");
	}

	fun splineToLinearHeading(x: Double, y: Double, h: Double, t: Double)
	{
		println("spline $x, $y, $h, $t");
	}

	fun splineToConstantHeading(x: Double, y: Double, t: Double)
	{
		println("spline $x, $y, $t");
	}

	fun build(): Action
	{
		println("build");
		return Action();
	}
}

class LuaAction
{
	companion object
	{
		fun init(builder: FunctionBuilder)
		{
			builder.setCurrentObject(LuaAction());

			builder.addObjectFunction("run", LuaType.Void, listOf(LuaType.Object(Action::class.java)));
			builder.addObjectFunction(
				"trajectoryAction",
				LuaType.Object(LuaTrajectoryBuilder::class.java),
				listOf(LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.createClass("Action");
		}
	}

	fun trajectoryAction(x: Double, y: Double, h: Double): LuaTrajectoryBuilder
	{
		println("builder $x, $y, $h");
		return LuaTrajectoryBuilder();
	}

	fun run(action: Action)
	{
		println("run");
	}
}

class Main
{
	@Test
	fun main()
	{
		doThing();
		//FileServer.start();
		//while(true)
		//{
		//	sleep(1000);
		//}
	}
}
fun doThing()
{
	val opmodeLoader = OpmodeLoader();
	
	val builder = opmodeLoader.getFunctionBuilder();
	
	LuaTrajectoryBuilder.init(builder);
	LuaAction.init(builder);
	
	val opmodes = opmodeLoader.init() ?: return;
	
	for(opmode in opmodes)
	{
		println("found opmode: $opmode");
	}
	
	opmodeLoader.loadOpmode(opmodes[0]);
	
	opmodeLoader.start();
}