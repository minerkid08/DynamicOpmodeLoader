---@return Action
---@param x number
---@param y number
function grabSample(x, y)
	return markerSequentialAction("grab sample")
			:add(recognizeSample())
			:add(moveToSample(x, y, 0))
			:add(extendHSlideToSample(-2 - 9.5))
			:add(sleepAction(1))
			:add(intakeDown())
			:add(sleepAction(1))
			:add(intakeIntake())
			:add(extendHSlideToSample(-9.5))
			:add(
				waitForOtherAction(
					intakeWaitForColor(Colors.YELLOW, 999),
					sequentalAction()
					:add(sleepAction(2))
					:add(moveToSample(x, y + 2, 90))
					:add(moveToSample(x, y - 2, -90))
					:add(extendHSlideToSample(2 - 9.5))
					:add(moveToSample(x, y, 90))
					:build()
				)
			)
			:build();
end