package com.minerkid08.dynamicopmodeloader

data class SequentialAction(
	val actions: List<Action>
) : Action
{
	private var actionIndex = 0;
	private lateinit var currentAction: Action;

	constructor(vararg actions: Action) : this(actions.asList())

	override fun start()
	{
		currentAction = actions[0];
		currentAction.start();
	}

	override fun update(): ActionState
	{
		if (actions.isEmpty())
			return ActionState.Done;

		val state = currentAction.update();

		when (state)
		{

			ActionState.Done          ->
			{
				currentAction.finish();
				actionIndex++;
				if(actionIndex == actions.size)
					return ActionState.Done;
				currentAction = actions[actionIndex];
				currentAction.start();
				return ActionState.Running;
			}

			ActionState.Error         ->
			{
				currentAction.error();
				return ActionState.Error;
			}

			ActionState.ErrorContinue ->
			{
				currentAction.error();
				actionIndex++;
				currentAction = actions[actionIndex];
				currentAction.start();
				return ActionState.Running;
			}

			ActionState.Running       ->
				return ActionState.Running;
		}
	}
}

data class ParallelAction(
	val actions: List<Action>
) : Action
{
	val actionState = Array(actions.size, { ActionState.Running });

	constructor(vararg actions: Action) : this(actions.asList())

	override fun start()
	{
		for (action in actions)
			action.start();
	}

	override fun update(): ActionState
	{
		if (actions.isEmpty())
			return ActionState.Done;

		var doneCount = 0;
		for (i in actions.indices)
		{
			if (actionState[i] != ActionState.Running)
			{
				doneCount++;
				continue;
			}

			actionState[i] = actions[i].update();

			when (actionState[i])
			{
				ActionState.Done          ->
				{
					doneCount++;
					actions[i].finish();
				}

				ActionState.ErrorContinue ->
				{
					doneCount++;
					actions[i].error();
				}

				ActionState.Error         ->
				{
					actions[i].error();
					for (j in actions.indices)
					{
						if (i == j)
							continue;
						actions[j].finish();
					}
					return ActionState.Error;
				}

				ActionState.Running       ->
				{
				}
			}
		}
		if (doneCount == actions.size)
			return ActionState.Done;
		return ActionState.Running;
	}
}
