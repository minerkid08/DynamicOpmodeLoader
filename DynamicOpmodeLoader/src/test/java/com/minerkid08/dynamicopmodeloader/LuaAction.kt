package com.minerkid08.dynamicopmodeloader

class LuaAction
{
    companion object
    {
        fun init(builder: FunctionBuilder)
        {
            builder.setCurrentObject(LuaAction());

            builder.addObjectFunction(
                "run",
                LuaType.Void,
                listOf(LuaType.Object(Action::class.java))
            );
            builder.addObjectFunction(
                "runTimer",
                LuaType.Void,
                listOf(LuaType.Object(Action::class.java), LuaType.String)
            );
            builder.addObjectFunction(
                "trajectoryAction", LuaType.Object(LuaTrajectoryBuilder::class.java), listOf(
                    LuaType.Double, LuaType.Double, LuaType.Double
                )
            );

            builder.addObjectFunction(
                "trajectoryActionX", LuaType.Object(LuaTrajectoryBuilder::class.java), listOf(
                    LuaType.Double, LuaType.Double, LuaType.Double,
                    LuaType.Double, LuaType.Double, LuaType.Double
                )
            );

            builder.addObjectFunction(
                "setPosEstimate", LuaType.Void, listOf(
                    LuaType.Double, LuaType.Double, LuaType.Double
                )
            );

            builder.addObjectFunction(
                "sequentalAction", LuaType.Object(LuaSequentalAction::class.java)
            );

            builder.addObjectFunction(
                "markerSequentialAction",
                LuaType.Object(LuaSequentalAction::class.java),
                listOf(LuaType.String)
            );

            builder.addObjectFunction(
                "parallelAction", LuaType.Object(LuaParallelAction::class.java)
            );

            builder.addObjectFunction(
                "waitForOtherAction",
                LuaType.Object(Action::class.java),
                listOf(LuaType.Object(Action::class.java), LuaType.Object(Action::class.java))
            );

            builder.addObjectFunction(
                "sleepAction",
                LuaType.Object(Action::class.java),
                listOf(LuaType.Double)
            );

            builder.addObjectFunction(
                "markerAction",
                LuaType.Object(Action::class.java),
                listOf(LuaType.String)
            );

            builder.addObjectFunction(
                "setAngAccel",
                LuaType.Void,
                listOf(LuaType.Double, LuaType.Double)
            );

            builder.addObjectFunction(
                "initProfileAction",
                LuaType.Object(Action::class.java),
                listOf(LuaType.Object(Action::class.java))
            );

            builder.createClass(Action::class.java.simpleName);

            LuaTrajectoryBuilder.init(builder);
            LuaSequentalAction.init(builder);
            LuaParallelAction.init(builder);
        }
    }

    fun setPosEstimate(x: Double, y: Double, h: Double)
    {
        println("setPoseEstimate $x, $y, $h");
    }

    fun setAngAccel(v: Double, a: Double)
    {
        println("setAngAccel $v, $a");
    }

    fun trajectoryAction(x: Double, y: Double, h: Double): LuaTrajectoryBuilder
    {
        return LuaTrajectoryBuilder(x, y, h);
    }

    fun trajectoryActionX(
        x: Double, y: Double, h: Double, vel: Double, minAccel: Double, maxAccel: Double
    ): LuaTrajectoryBuilder
    {
        return LuaTrajectoryBuilder(x, y, h, vel, minAccel, maxAccel);
    }

    fun sequentalAction(): LuaSequentalAction
    {
        return LuaSequentalAction(null);
    }

    fun parallelAction(): LuaParallelAction
    {
        return LuaParallelAction();
    }

    fun waitForOtherAction(a: Action, b: Action): Action
    {
        return Action();
    }

    fun markerSequentialAction(label: String): LuaSequentalAction
    {
        return LuaSequentalAction(label);
    }

    fun sleepAction(time: Double): Action
    {
        return Action();
    }

    fun markerAction(name: String): Action
    {
        return Action();
    }

    fun runTimer(action: Action, filename: String)
    {
        println("run");
    }

    fun run(action: Action)
    {
        println("run");
    }

    fun initProfileAction(action: Action): Action
    {
        return action;
    }
}

class LuaSequentalAction(val label: String?)
{
    companion object
    {
        fun init(builder: FunctionBuilder)
        {
            builder.addClassFunction(
                LuaSequentalAction::class.java,
                "add",
                LuaType.Builder,
                listOf(LuaType.Object(Action::class.java))
            );
            builder.addClassFunction(
                LuaSequentalAction::class.java, "build", LuaType.Object(Action::class.java)
            );
        }
    }

    private val actions = ArrayList<Action>();

    fun add(action: Action)
    {
        actions.add(action)
    }

    fun build(): Action
    {
        println("build seq");
        return Action();
    }
}

class LuaParallelAction
{
    companion object
    {
        fun init(builder: FunctionBuilder)
        {
            builder.addClassFunction(
                LuaParallelAction::class.java,
                "add",
                LuaType.Builder,
                listOf(LuaType.Object(Action::class.java))
            );
            builder.addClassFunction(
                LuaParallelAction::class.java, "build", LuaType.Object(Action::class.java)
            );
        }
    }

    private val actions = ArrayList<Action>();

    fun add(action: Action)
    {
        actions.add(action)
    }

    fun build(): Action
    {
        println("build parallel");
        return Action();
    }
}