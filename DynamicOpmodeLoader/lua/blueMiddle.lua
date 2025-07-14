require("utils");

---@type Action
local action = {}

addOpmode({
	name = "blueMiddle",
	init = function ()
		setPosEstimate(9.5, 31.5, 0);
		local builder = sequentalAction();

		builder:add(
			markerSequentialAction("sample 1")
			:add(
				parallelAction()
				:add(
					sequentalAction()
					:add(sleepAction(0.35))
					:add(hslideGotoMin())
					:add(intakeDown())
					:add(intakeIntake())
					:build()
				)
				:add(
					trajectoryAction(9.5, 31.5, 0)
					:setTangent(0)
					:splineToConstantHeading(19.5, 3.5, 0)
					:build()
				)
				:build()
			)
			:add(intakeUp())
			:add(hslideZero())
			:add(sleepAction(0.32))
			:add(intakeOuttake())
			:add(sleepAction(0.3))
			:build()
		);

		builder:add(
			markerSequentialAction("sample 2")
			:add(intakeDown())
			:add(hslideGotoMin())
			:add(intakeIntake())
			:add(sleepAction(0.1))
			:add(
				trajectoryAction(19.5, 3.5, 0)
				:setTangent(-90)
				:splineToConstantHeading(19.5, -3.5, -90)
				:build()
			)
			:add(intakeUp())
			:add(hslideZero())
			:add(
				parallelAction()
				:add(
					sequentalAction()
					:add(sleepAction(0.25))
					:add(intakeOuttake())
					:build()
				)
				:add(
					trajectoryAction(19.5, -3.5, 0)
					:setTangent(180)
					:splineToLinearHeading(18, -2, 0, 180)
					:build()
				)
				:build()
			)
			:add(sleepAction(0.1))
			:build()
		);

		local trajectory = nil;
		builder:add(markerAction("sample 3"));

		builder:add(
			parallelAction()
			:add(hslideGotoMin())
			:add(intakeDown())
			:add(intakeIntake())
			:add(
				trajectoryAction(18, -2.5, 0)
				:setTangent(0)
				:splineToConstantHeading(29, -3.5, 0)
				:build()
			)
			:build()
		);

		builder:add(hslideZero());
		builder:add(intakeUp());

		seq = sequentalAction();
		seq:add(sleepAction(0.5));
		seq:add(intakeOuttake());

		trajectory = trajectoryAction(29, -3.5, 0);
		trajectory:setTangent(180);
		trajectory:splineToConstantHeading(20, -2, 180);

		parallel = parallelAction();
		parallel:add(trajectory:build());
		parallel:add(seq:build());

		builder:add(parallel:build());

		builder:add(intakeStop());

		builder:add(markerAction("sample 4"));

		builder:add(intakeDown());
		builder:add(hslideGotoMin());
		builder:add(intakeIntake());

		trajectory = trajectoryAction(20, -2, 0);
		trajectory:setTangent(90);
		trajectory:splineToConstantHeading(29, 4, 0);
		builder:add(trajectory:build());

		builder:add(intakeUp());
		builder:add(hslideZero());

		seq = sequentalAction();
		seq:add(sleepAction(0.5));
		seq:add(intakeOuttake());

		trajectory = trajectoryAction(29, 4, 0);
		trajectory:setTangent(180);
		trajectory:splineToConstantHeading(20, 3.2, 180);
		parallel = parallelAction();
		parallel:add(trajectory:build());
		parallel:add(seq:build());
		builder:add(parallel:build());
		builder:add(intakeStop());

		builder:add(markerAction("score 1"));

		builder:add(
			trajectoryActionX(20, 3.5, 0, overides({ maxAccel = 70 }))
			:setTangent(180)
			:splineToConstantHeading(11, 0, 180)
			:build()
		);
		builder:add(specimenGrab());

		builder:add(
			parallelAction()
			:add(
				trajectoryAction(11, 0, 0)
				:setTangent(0)
				:splineToLinearHeading(60.5, 15, -90, 90)
				:build()
			)
			:add(
				sequentalAction()
				:add(sleepAction(2))
				:add(specimenScore())
				:build()
			)
			:build()
		);

		builder:add(markerAction("score 2"));

		builder:add(
			trajectoryActionX(60.5, 15, -90, overides({ maxAccel = 70 }))
			:setTangent(-90)
			:splineToLinearHeading(11, 0, 0, 180)
			:build()
		);
		builder:add(specimenGrab());

		builder:add(
			parallelAction()
			:add(
				trajectoryAction(11, 0, 0)
				:setTangent(0)
				:splineToLinearHeading(62.5, -14, 90, -90)
				:build()
			)
			:add(
				sequentalAction()
				:add(sleepAction(2))
				:add(specimenScore())
				:build()
			):build()
		);

		builder:add(markerAction("score 3"));

		builder:add(
			trajectoryActionX(62.5, -14, 90, overides({ maxAccel = 70 }))
			:setTangent(90)
			:splineToLinearHeading(11, 0, 0, 180)
			:build()
		);
		builder:add(specimenGrab());

		builder:add(
			parallelAction()
			:add(
				trajectoryAction(11, 0, 0)
				:setTangent(0)
				:splineToLinearHeading(61.5, 15, -90, 90)
				:build()
			)
			:add(
				sequentalAction()
				:add(sleepAction(2))
				:add(specimenScore())
				:build()
			)
			:build()
		);

		builder:add(markerAction("score 4"));

		builder:add(
			trajectoryActionX(61.5, 15, -90, overides({ maxAccel = 70 }))
			:setTangent(-90)
			:splineToLinearHeading(11, 0, 0, 180)
			:build()
		);
		builder:add(specimenGrab())

		builder:add(
			parallelAction()
			:add(
				trajectoryAction(11, 0, 0)
				:setTangent(0)
				:splineToLinearHeading(63.5, -14, 90, -90)
				:build()
			)
			:add(
				sequentalAction()
				:add(sleepAction(2))
				:add(specimenScore())
				:build()
			):build()
		);

		builder:add(sleepAction(1));

		action = builder:build();
	end,
	start = function ()
		runTimer(action, "blueMiddle.txt");
	end
});