---@type Action
local action = nil;

addOpmode({
	name = "specimenAuto5",
	init = function ()
		setPosEstimate(7.0, -7.5, 180);
		local builder = sequentalAction();

		-- score specimen 1 (preload)

		local parallel = parallelAction();
		parallel:add(specimenGrabInstant());

		local trajectory = trajectoryActionX(7.0, -7.5, 180, overides({}));
		trajectory:lineToX(36.25);
		parallel:add(trajectory:build());

		builder:add(parallel:build());
		builder:add(specimenScore());

		-- push samples

		trajectory = trajectoryActionX(36, -6.5, 180.0, overides({ vel = 65, minAccel = -65, maxAccel = 65 }));
		trajectory:setTangent(-180);
		trajectory:splineToConstantHeading2(32.0, -36.0, 0, overides({vel = 65, minAccel = -50, maxAccel = 65}));
		trajectory:splineToConstantHeading2(55.0, -47.0, -90.0, overides({vel = 65, minAccel = -50, maxAccel = 65}));
		trajectory:setTangent(180.0);
		trajectory:lineToX(21.5);
		trajectory:setTangent(0.0);
		trajectory:splineToLinearHeading(56.0, -55.0, 0, -90.0);
		trajectory:setTangent(180.0);
		trajectory:lineToX(20);
		trajectory:setTangent(0.0);
		trajectory:splineToConstantHeading(56.0, -62.5, -90.0);
		trajectory:setTangent(180.0);
		trajectory:lineToX(7.75);
		builder:add(trajectory:build());

		-- grab specimen 2

		builder:add(specimenGrab());

		-- score specimen 2

		trajectory = trajectoryActionX(8.5, -62, 0, overides({}));
		trajectory:setTangent(45);
		trajectory:splineToLinearHeading(36.25, -3.5, 180, 0);
		builder:add(trajectory:build());
		builder:add(specimenScore());
		--builder:add(sleepAction(0.1));

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
		trajectory:splineToLinearHeading(36, -1.5, 180.0, 0);
		builder:add(trajectory:build());
		--builder:add(sleepAction(0.025));
		builder:add(specimenScore());

		-- grab specimen 4

		--builder:add(sleepAction(0.05));
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
		trajectory:splineToLinearHeading(36.0, 1.5, 180.0, 0);
		builder:add(trajectory:build());
		--builder:add(sleepAction(0.05));
		builder:add(specimenScore());

		-- grab specimen 5

		--builder:add(sleepAction(0.05));
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

		-- score specimen 5

		trajectory = trajectoryActionX(8.5, -40.5, 0.0, overides({}));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36.0, 4, 180.0, 0.0);
		builder:add(trajectory:build());
		--builder:add(sleepAction(0.05));
		builder:add(specimenScore());

		builder:add(sleepAction(0.2));
		action = builder:build();
	end,
	start = function ()
		--runTimer(action, "specimenAuto2.txt");
		run(action);
	end
})
