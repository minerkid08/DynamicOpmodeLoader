require("utils");
require("vision");

---@type Action
local action = {}

addOpmode({
	name = "blueBasket",
	init = function ()
		setPosEstimate(7.5, -30.5, -90);

		local builder = sequentalAction();
		builder:add(
			markerSequentialAction("spec")
			:add(specimenGrabInstant())
			:add(
				trajectoryAction(7.5, -30.5, -90)
				:setTangent(0)
				:splineToConstantHeading(63, -33.5, 27)
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
			:add(intakeWaitForColor(Colors.YELLOW, 2))
			:build()
		);

		builder:add(
			markerSequentialAction("score 1")
			:add(
				parallelAction()
				:add(
					sequentalAction()
					:add(intakeUp())
					:add(hslideZero())
					:add(sleepAction(0.3))
					:add(sampleClawTransfer())
					:add(sampleClawUp())
					:build()
				)
				:add(
					trajectoryAction(63, -33.5, -140)
					:setTangent(-90)
					:splineToLinearHeading(15, -54, 45, -135)
					:build()
				)
				:build()
			)
			:add(sampleClawScore())
			:build()
		);

		builder:add(
			markerSequentialAction("grab 2")
			:add(intakeDown())
			:add(intakeIntake())
			:add(
				parallelAction()
				:add(
					trajectoryAction(16, -53, 45)
					:setTangent(0)
					:splineToLinearHeading(21, -53, -18, 0)
					:build()
				)
				:add(
					sequentalAction()
					:add(sleepAction(0.6))
					:add(hslideGotoMin())
					:add(intakeWaitForColor(Colors.YELLOW, 2))
					:build()
				)
				:build()
			)
			:build()
		);

		builder:add(
			markerSequentialAction("score 2")
			:add(
				parallelAction()
				:add(
					sequentalAction()
					:add(intakeUp())
					:add(hslideZero())
					:add(sleepAction(0.65))
					:add(sampleClawTransfer())
					:add(sampleClawUp())
					:add(sleepAction(1))
					:build()
				)
				:add(
					trajectoryAction(21, -53, -13)
					:setTangent(-90)
					:splineToLinearHeading(15, -54, 45, -135)
					:build()
				)
				:build()
			)
			:add(sampleClawScore())
			:build()
		);

		builder:add(
			markerSequentialAction("grab 3")
			:add(intakeDown())
			:add(intakeIntake())
			:add(
				parallelAction()
				:add(
					trajectoryAction(16, -53, 45)
					:setTangent(0)
					:splineToLinearHeading(21, -53, -36, 0)
					:build()
				)
				:add(
					sequentalAction()
					:add(sleepAction(0.6))
					:add(hslideGotoMin())
					:add(intakeWaitForColor(Colors.YELLOW, 2))
					:build()
				)
				:build()
			)
			:build()
		);

		builder:add(
			markerSequentialAction("score 3")
			:add(
				parallelAction()
				:add(
					sequentalAction()
					:add(intakeUp())
					:add(hslideZero())
					:add(sleepAction(0.65))
					:add(sampleClawTransfer())
					:add(sampleClawUp())
					:add(sleepAction(1))
					:build()
				)
				:add(
					trajectoryAction(21, -53, -35)
					:setTangent(-90)
					:splineToLinearHeading(15, -54, 45, -135)
					:build()
				)
				:build()
			)
			:add(sampleClawScore())
			:build()
		);

		builder:add(
			markerSequentialAction("grab 4")
			:add(
				trajectoryAction(15, -54, 45)
				:setTangent(45)
				:splineToLinearHeading(39, 0, 0, 0)
				:build()
			)
			:add(grabSample(48.5, 0))
			:build()
		);

		builder:add(sleepAction(1));

		action = builder:build();

		action = initProfileAction(action);
	end,
	start = function ()
		runTimer(action, "blueBasket.txt");
	end
})