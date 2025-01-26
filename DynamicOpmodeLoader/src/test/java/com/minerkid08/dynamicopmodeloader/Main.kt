package com.minerkid08.dynamicopmodeloader

import android.os.Environment
import org.junit.Test
import java.nio.file.Paths


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
				LuaType.Void,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToX",
				LuaType.Void,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToY",
				LuaType.Void,
				listOf(LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToLinearHeading",
				LuaType.Void,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);
			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToConstantHeading",
				LuaType.Void,
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
		val opmodeLoader = OpmodeLoader();
		val opmodes = opmodeLoader.init() ?: return;
		
		val builder = opmodeLoader.getFunctionBuilder();
		
		LuaTrajectoryBuilder.init(builder);
		LuaAction.init(builder);
		
		for(opmode in opmodes)
		{
			println("found opmode: $opmode");
		}
		
		opmodeLoader.loadOpmode(opmodes[0]);
		
		opmodeLoader.start();
		
		return;
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
}