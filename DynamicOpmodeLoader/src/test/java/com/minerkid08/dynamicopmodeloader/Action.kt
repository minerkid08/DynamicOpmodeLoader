package com.minerkid08.dynamicopmodeloader

enum class ActionState
{
    Running,
    Done,
    Error,
    ErrorContinue
}

interface Action
{
    fun start() {};
    fun update(): ActionState;
    fun finish() {};
    fun error() {};
}

data class SleepAction(private val time: Double): Action
{
    private var startTime = 0.0;
    override fun start()
    {
        startTime = System.nanoTime() * 1e-9;
    }

    override fun update(): ActionState
    {
        val now = System.nanoTime() * 1e-9;
        if(now - startTime > time)
            return ActionState.Done;
        return ActionState.Running;
    }
}

class NullAction : Action
{
    override fun update(): ActionState = ActionState.Done;
}