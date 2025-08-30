
---@type Action
local action = {}

local opmode = { name = "blueBasket" }

function opmode.init()
	action = sequentalAction()
		:add(intakeDown())
		:add(intakeIntake())
		:add(hslideGotoMin())
		:add(intakeWaitForColor(Colors.YELLOW))
		:add(intakeUp())
		:add(hslideZero())
		:add(sleepAction(0.65))
		:add(sampleClawTransfer())
		:build();
end

function opmode.start()
	run(action);
end

addOpmode(opmode);