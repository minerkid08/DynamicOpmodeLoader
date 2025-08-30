---@type Opmode
local opmode = { name = "visionTest" }

---@type Action
local action = nil;

function opmode:init()
	setPosEstimate(9.5, 0, 0);
	action = sequentalAction()
			:add(
				trajectoryAction(9.5, 0, 0)
				:splineTo(39, 0, 0)
				:build()
			)
			:add(recognizeSample())
			:add(moveToSample(48.5, 0, 0))
			:add(extendHSlideToSample(-2 - 9.5))
			:add(sleepAction(1))
			:add(intakeDown())
			:add(sleepAction(1))
			:add(intakeIntake())
			:add(extendHSlideToSample(-9.5))
			:add(
				waitForOtherAction(
					intakeWaitForColor(Colors.YELLOW),
					sequentalAction()
					:add(sleepAction(2))
					:add(moveToSample(48.5, 2, 90))
					:add(moveToSample(48.5, -2, -90))
					:add(extendHSlideToSample(2 - 9.5))
					:add(moveToSample(48.5, 0, 90))
					:build()
				)
			)
			:add(intakeStop())
			:build();
end

function opmode:start()
	run(action);
end

addOpmode(opmode);