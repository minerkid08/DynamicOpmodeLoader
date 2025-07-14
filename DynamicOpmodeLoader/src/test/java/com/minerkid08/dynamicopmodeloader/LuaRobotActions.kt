package com.minerkid08.dynamicopmodeloader

class LuaRobotActions
{
	companion object
	{
		fun init(builder: FunctionBuilder)
		{
			builder.setCurrentObject(LuaRobotActions());
			builder.addObjectFunction("specimenGrab", LuaType.Object(Action::class.java));
			builder.addObjectFunction("specimenGrabInstant", LuaType.Object(Action::class.java));
			builder.addObjectFunction("specimenScore", LuaType.Object(Action::class.java));

			builder.addObjectFunction("hslideGotoPos", LuaType.Object(Action::class.java), listOf(LuaType.Double));
			builder.addObjectFunction("hslideZero", LuaType.Object(Action::class.java));
			builder.addObjectFunction("hslideGotoMin", LuaType.Object(Action::class.java));

			builder.addObjectFunction("intakeUp", LuaType.Object(Action::class.java));
			builder.addObjectFunction("intakeDown", LuaType.Object(Action::class.java));

			builder.addObjectFunction("intakeIntake", LuaType.Object(Action::class.java));
			builder.addObjectFunction("intakeOuttake", LuaType.Object(Action::class.java));
			builder.addObjectFunction("intakeStop", LuaType.Object(Action::class.java));
			builder.addObjectFunction("intakeWaitForColor", LuaType.Object(Action::class.java), listOf(LuaType.Double, LuaType.Double));

			builder.addObjectFunction("sampleClawTransfer", LuaType.Object(Action::class.java));
			builder.addObjectFunction("sampleClawUp", LuaType.Object(Action::class.java));
			builder.addObjectFunction("sampleClawScore", LuaType.Object(Action::class.java));

			builder.addObjectFunction("recognizeSample", LuaType.Object(Action::class.java));
			builder.addObjectFunction("moveToSample", LuaType.Object(Action::class.java), listOf(LuaType.Double, LuaType.Double, LuaType.Double));
			builder.addObjectFunction("extendHSlideToSample", LuaType.Object(Action::class.java), listOf(LuaType.Double));

			builder.createClass("SpecimenOuttakeAction_Grab");
			builder.createClass("SpecimenOuttakeAction_GrabInstant");
			builder.createClass("SpecimenOuttakeAction_Score");

			builder.createClass("SampleClawAction_Transfer");
			builder.createClass("SampleClawAction_Up");
			builder.createClass("SampleClawAction_Score");

			builder.createClass("HSlideAction_GotoPos");
			builder.createClass("HSlideAction_GotoMin");
			builder.createClass("HSlideAction_Zero");

			builder.createClass("ArmAction_Up");
			builder.createClass("ArmAction_Down");

			builder.createClass("IntakeAction_Intake");
			builder.createClass("IntakeAction_Outtake");
			builder.createClass("IntakeAction_Stop");
			builder.createClass("IntakeAction_WaitForColor");

			builder.createClass("VisionAction_RecognizeSample");
			builder.createClass("VisionAction_MoveToSample");
			builder.createClass("VisionAction_ExtendHslideToSample");
		}
	}
	
	fun specimenGrab(): Action
	{
		return Action();
	}

	fun specimenGrabInstant(): Action
	{
		return Action();
	}

	fun specimenScore(): Action
	{
		return Action();
	}

	fun hslideGotoPos(pos: Double): Action
	{
		return Action();
	}

	fun hslideGotoMin(): Action
	{
		return Action();
	}

	fun hslideZero(): Action
	{
		return Action();
	}

	fun intakeUp(): Action
	{
		return Action();
	}

	fun intakeDown(): Action
	{
		return Action();
	}

	fun intakeIntake(): Action
	{
		return Action();
	}

	fun intakeOuttake(): Action
	{
		return Action();
	}

	fun intakeStop(): Action
	{
		return Action();
	}

	fun intakeWaitForColor(color: Double, maxTime: Double): Action
	{
		return Action();
	}

	fun sampleClawTransfer(): Action
	{
		return Action();
	}

	fun sampleClawUp(): Action
	{
		return Action();
	}

	fun sampleClawScore(): Action
	{
		return Action();
	}

	fun recognizeSample(): Action
	{
		return Action();
	}

	fun moveToSample(xOff: Double, yOff: Double, tangent: Double): Action
	{
		return Action();
	}

	fun extendHSlideToSample(offset: Double): Action
	{
		return Action();
	}
}