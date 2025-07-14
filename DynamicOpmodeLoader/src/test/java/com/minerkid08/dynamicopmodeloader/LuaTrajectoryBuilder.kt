package com.minerkid08.dynamicopmodeloader

class LuaTrajectoryBuilder
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
				"lineToX2",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToY",
				LuaType.Builder,
				listOf(LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"lineToY2",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToLinearHeading",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToLinearHeading2",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToConstantHeading",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineToConstantHeading2",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineTo",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"splineTo2",
				LuaType.Builder,
				listOf(LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double, LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"turnTo",
				LuaType.Builder,
				listOf(LuaType.Double)
			);

			builder.addClassFunction(
				LuaTrajectoryBuilder::class.java,
				"build",
				LuaType.Object(Action::class.java)
			);

			builder.addClassFunction(LuaTrajectoryBuilder::class.java, "getEndX", LuaType.Double);
			builder.addClassFunction(LuaTrajectoryBuilder::class.java, "getEndY", LuaType.Double);
			builder.addClassFunction(LuaTrajectoryBuilder::class.java, "getEndH", LuaType.Double);
		}
	}

	private var intEndX = 0.0;
	private var intEndY = 0.0;
	private var intEndH = 0.0;

	constructor(x: Double, y: Double, h: Double)
	{
		intEndX = x;
		intEndY = y;
		intEndH = h;
	}

	constructor(x: Double, y: Double, h: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndX = x;
		intEndY = y;
		intEndH = h;
	}

	fun setTangent(tangent: Double)
	{
		println("setTangent $tangent");
	}

	fun lineToX(x: Double)
	{
		intEndX = x;
		println("lineToX $x");
	}

	fun lineToX2(x: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndX = x;
		println("lineToX $x");
	}

	fun lineToY(y: Double)
	{
		intEndY = y;
		println("lineToY $y");
	}

	fun lineToY2(x: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndY = x;
		println("lineToY $x");
	}

	fun splineToLinearHeading(x: Double, y: Double, h: Double, t: Double)
	{
		intEndX = x;
		intEndY = y;
		intEndH = h;
		println("splineToLinearHeading $x, $y, $h, $t");
	}

	fun splineToLinearHeading2(x: Double, y: Double, h: Double, t: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndX = x;
		intEndY = y;
		intEndH = h;
		println("splineToLinearHeading $x, $y, $h, $t");
	}

	fun splineToConstantHeading(x: Double, y: Double, t: Double)
	{
		intEndX = x;
		intEndY = y;
		println("splineToConstantHeading $x, $y, $t");
	}

	fun splineToConstantHeading2(x: Double, y: Double, t: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndX = x;
		intEndY = y;
		println("splineToConstantHeading $x, $y, $t");
	}

	fun splineTo(x: Double, y: Double, t: Double)
	{
		intEndX = x;
		intEndY = y;
		println("splineTo $x, $y, $t");
	}

	fun splineTo2(x: Double, y: Double, t: Double, vel: Double, minAccel: Double, maxAccel: Double)
	{
		intEndX = x;
		intEndY = y;
		println("splineTo $x, $y, $t");
	}

	fun turnTo(h: Double)
	{
		intEndH = h;
		println("turnTo $h");
	}

	fun getEndX(): Double
	{
		return intEndX;
	}

	fun getEndY(): Double
	{
		return intEndY;
	}

	fun getEndH(): Double
	{
		return intEndH;
	}

	fun build(): Action
	{
		println("build");
		return NullAction();
	}
}