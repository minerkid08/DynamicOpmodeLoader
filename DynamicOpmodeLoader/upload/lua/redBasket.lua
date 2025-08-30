require("utils");

---@type Action
local action = {}

local prevPos = { x = 0, y = 0, h = 0 };

LuaTrajectoryBuilder.build2 = LuaTrajectoryBuilder.build;
LuaTrajectoryBuilder.build = nil;

function LuaTrajectoryBuilder:build()
	prevPos.x = self:getEndX();
	prevPos.y = self:getEndY();
	prevPos.h = self:getEndH();
	return self:build2();
end

function prevPos:get()
	return self.x, self.y, self.h;
end

addOpmode({
	name = "redBasket",
	init = function ()
		setPosEstimate(7.5, 30.5, 90);
		builder = sequentalAction();
		builder:add(
			markerSequentialAction("score spec")
			:add(specimenGrabInstant())
			:add(
				trajectoryAction(7.5, 30.5, 90)
				:setTangent(0)
				:splineToConstantHeading(63, 33.5, -22.5)
				:build()
			)
			:add(specimenScore())
			:build()
		);

		builder:add(
			markerSequentialAction("grab 1")
			:add(
				trajectoryAction(63, 33.5, 90)
				:turnTo(143)
				:build()
			)
			:add(intakeDown())
			:add(intakeIntake())
			:add(hslideGotoPos(0.6))
			:add(sleepAction(0.55))
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
					:add(sleepAction(0.2))
					:add(sampleClawTransfer())
					:add(sampleClawUp())
					:build()
				)
				:add(
					trajectoryAction(63, 33.5, 140)
					:setTangent(90)
					:splineToLinearHeading(15, 54, -45, 135)
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
					trajectoryAction(16, 53, -45)
					:setTangent(0)
					:splineToLinearHeading(21, 53, 13, 0)
					:build()
				)
				:add(
					sequentalAction()
					:add(sleepAction(0.6))
					:add(hslideGotoMin())
					:build()
				)
				:build()
			)
			:add(sleepAction(0.65))
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
					trajectoryAction(21, 53, 13)
					:setTangent(90)
					:splineToLinearHeading(15, 54, -45, 135)
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
					trajectoryAction(16, 53, -45)
					:setTangent(0)
					:splineToLinearHeading(21, 53, 35, 0)
					:build()
				)
				:add(
					sequentalAction()
					:add(sleepAction(0.6))
					:add(hslideGotoMin())
					:build()
				)
				:build()
			)
			:add(sleepAction(0.65))
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
					trajectoryAction(21, 53, 35)
					:setTangent(90)
					:splineToLinearHeading(15, 54, -45, 135)
					:build()
				)
				:build()
			)
			:add(sampleClawScore())
			:build()
		);

		builder:add(sleepAction(1));

		action = builder:build();

		action = initProfileAction(action);
	end,
	start = function ()
		runTimer(action, "redBasket.txt");
	end
})