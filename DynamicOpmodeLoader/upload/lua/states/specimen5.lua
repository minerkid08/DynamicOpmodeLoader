---@type Action
local action = {}

addOpmode({
	name = "specimenAuto5",
	init = function ()
		setPosEstimate(7.0, -7.5, 180);
		setAngAccel(5 * 3.14, 5 * 3.14);
		local builder = sequentalAction();

		-- score specimen 1 (preload)

		local parallel = parallelAction();
		local seq2 = sequentalAction();

		seq2:add(specimenGrabInstant());
		seq2:add(sleepAction(1.5));
		seq2:add(specimenScore());
		parallel:add(seq2:build());

		local trajectory = trajectoryActionX(7.0, -7.5, 180, overides({ minAccel = -25 }));
		trajectory:lineToX(36.125);
		parallel:add(trajectory:build());

		builder:add(parallel:build());
		--builder:add(sleepAction(0.05));

		-- push samples

		trajectory = trajectoryActionX(36, -6.5, 180.0, overides({ vel = 80, minAccel = -65, maxAccel = 65 }));
		trajectory:setTangent(-180);
		trajectory:splineToConstantHeading2(32.0, -33.0, 0, overides({ vel = 65, minAccel = -50, maxAccel = 65 }));
		trajectory:splineToConstantHeading2(55.0, -47.0, -90.0, overides({ vel = 65, minAccel = -50, maxAccel = 65 }));
		trajectory:setTangent(180.0);
		trajectory:lineToX(20);
		trajectory:setTangent(0.0);
		trajectory:splineToLinearHeading(56.0, -55.0, 0, -90.0);
		trajectory:setTangent(180.0);
		trajectory:lineToX(17.5);
		trajectory:setTangent(0.0);
		trajectory:splineToConstantHeading(56.0, -62.5, -90.0);
		trajectory:setTangent(180.0);
		trajectory:lineToX2(7.75, overides({ vel = 65, minAccel = -60, maxAccel = 65 }));
		builder:add(trajectory:build());

		-- grab specimen 2

		builder:add(sleepAction(0.025));
		builder:add(specimenGrab());

		-- score specimen 2

		trajectory = trajectoryActionX(8.5, -62, 0, overides({}));
		trajectory:setTangent(45);
		trajectory:splineToLinearHeading(36.25, -5.5, 180, 0);
		parallel = parallelAction();
		parallel:add(trajectory:build());
		seq2 = sequentalAction();
		seq2:add(sleepAction(2.5));
		seq2:add(specimenScore());
		parallel:add(seq2:build());
		builder:add(parallel:build());

		-- grab specimen 3

		trajectory = trajectoryActionX(36, -7.5, 180, overides({}));
		trajectory:setTangent(-180);
		trajectory:splineToLinearHeading(8, -36.5, 0, -180)

		parallel = parallelAction();
		parallel:add(trajectory:build())
		local seq = sequentalAction();
		seq:add(sleepAction(2));
		seq:add(specimenGrab());
		parallel:add(seq:build());
		builder:add(parallel:build());

		-- score specimen 3

		trajectory = trajectoryActionX(7.75, -40.5, 0.0, overides({}));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36.5, -2.5, 180.0, 0);
		parallel = parallelAction();
		parallel:add(trajectory:build());
		seq2 = sequentalAction();
		seq2:add(sleepAction(2));
		seq2:add(specimenScore());
		parallel:add(seq2:build());
		builder:add(parallel:build());

		-- grab specimen 4

		trajectory = trajectoryActionX(35.0, -1.5, 180.0, overides({}));
		trajectory:setTangent(-180.0);
		trajectory:splineToLinearHeading(7.75, -36.5, 0.0, -180.0);

		parallel = parallelAction();
		parallel:add(trajectory:build())
		seq = sequentalAction();
		seq:add(sleepAction(2));
		seq:add(specimenGrab());
		parallel:add(seq:build());
		builder:add(parallel:build());

		-- score specimen 4

		trajectory = trajectoryActionX(8.5, -40.5, 0.0, overides({}));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36.75, 1.5, 180.0, 0);
		parallel = parallelAction();
		parallel:add(trajectory:build());
		seq2 = sequentalAction();
		seq2:add(sleepAction(2));
		seq2:add(specimenScore());
		parallel:add(seq2:build());
		builder:add(parallel:build());

		-- grab specimen 5

		trajectory = trajectoryActionX(35.0, -1.5, 180.0, overides({}));
		trajectory:setTangent(-180.0);
		trajectory:splineToLinearHeading(7.75, -36.5, 0.0, -180.0);
		parallel = parallelAction();
		parallel:add(trajectory:build())
		seq = sequentalAction();
		seq:add(sleepAction(2));
		seq:add(specimenGrab());
		parallel:add(seq:build());
		builder:add(parallel:build());

		trajectory = trajectoryActionX(8.5, -40.5, 0.0, overides({}));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36.25, 4, 180.0, 0.0);
		parallel = parallelAction();
		parallel:add(trajectory:build());
		seq2 = sequentalAction();
		seq2:add(sleepAction(2));
		seq2:add(specimenScore());
		parallel:add(seq2:build());
		builder:add(parallel:build());

		builder:add(sleepAction(0.2));

		action = initProfileAction(builder:build());
	end,
	start = function ()
		runTimer(action, "specimen5.txt");
	end
})