---@type Action
local action = {}

local opmode = { name = "blueRight" };

function opmode.init()
	setPosEstimate(7.5, -30.5, -90);

	local builder = sequentalAction();
	builder:add(
		markerSequentialAction("spec")
		:add(specimenGrabInstant())
		:add(
			trajectoryAction(7.5, -30.5, -90)
			:setTangent(0)
			:splineToConstantHeading(63, -33.5, 25)
			:build()
		)
		:add(specimenScore())
		:build()
	);

	builder:add(
		markerSequentialAction("grab 1")
		:add(
			trajectoryAction(63, -33.5, -90)
			:turnTo(-143)
			:build()
		)
		:add(intakeDown())
		:add(intakeIntake())
		:add(hslideGotoPos(0.6))
		:add(intakeWaitForColor(Colors.YELLOW))
		:build()
	);

	builder:add(
		markerSequentialAction("move")
		:add(intakeUp())
		:add(hslideZero())
		:add(
			trajectoryAction(63, -33, -140)
			:setTangent(45)
			:splineToLinearHeading(82, -36, 180, 0)
			:build()
		)
		:build()
	);

	builder:add(sleepAction(1));

	action = builder:build();

	action = initProfileAction(action);
end

function opmode.start()
	runTimer(action, "redRight.txt");
end