require("utils");

---@type Action
local action = {}

addOpmode({
	name = "specimenAuto45",
	init = function ()
		setPosEstimate(7.0, -7.5, 180);
		local builder = sequentalAction();

		-- score specimen 1 (preload)

		local parallel = parallelAction();
		parallel:add(specimenGrabInstant());

		local trajectory = trajectoryActionX(7.0, -7.5, 180, overides({ minAccel = -20 }));
		trajectory:lineToX(36.125);
		parallel:add(trajectory:build());

		builder:add(parallel:build());
		builder:add(sleepAction(0.05));
		builder:add(specimenScore());

		-- push samples

		trajectory = trajectoryActionX(36, -6.5, 180.0, overides({ vel = 65, minAccel = -65, maxAccel = 65 }));
		trajectory:setTangent(-180);
		trajectory:splineTo(32.0, -35.0, 0);
		trajectory:splineToConstantHeading(55.0, -47.0, -90.0);
		trajectory:setTangent(180.0);
		trajectory:lineToX(17);
		trajectory:setTangent(22.5);
		trajectory:splineToConstantHeading(56.0, -55.0, -90.0);
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
		trajectory:splineToLinearHeading(36.25, -5, 180, 0);
		builder:add(trajectory:build());
		builder:add(sleepAction(0.025));
		builder:add(specimenScore());
		--builder:add(sleepAction(0.1));

		-- grab specimen 3

		trajectory = trajectoryActionX(36, -7.5, 180, overides({}));
		trajectory:setTangent(-180);
		trajectory:splineToLinearHeading(8, -36.5, 0, -180)
		builder:add(trajectory:build());
		builder:add(specimenGrab());

		-- score specimen 3

		trajectory = trajectoryActionX(7.75, -40.5, 0.0, overides({ minAccel = (-35 / 2) }));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36.25, -2.5, 180.0, 0);
		builder:add(trajectory:build());
		--builder:add(sleepAction(0.025));
		builder:add(sleepAction(0.125));
		builder:add(specimenScore());

		-- grab specimen 4

		--builder:add(sleepAction(0.05));
		trajectory = trajectoryActionX(35.0, -1.5, 180.0, overides({}));
		trajectory:setTangent(-180.0);
		trajectory:splineToLinearHeading(7.75, -36.5, 0.0, -180.0);
		builder:add(trajectory:build());
		builder:add(specimenGrab());

		-- score specimen 4

		trajectory = trajectoryActionX(8.5, -40.5, 0.0, overides({ minAccel = (-35 / 2) }));
		trajectory:setTangent(45.0);
		trajectory:splineToLinearHeading(36, 1.5, 180.0, 0);
		builder:add(trajectory:build());
		--builder:add(sleepAction(0.05));
		builder:add(sleepAction(0.125));
		builder:add(specimenScore());

		-- grab specimen 5

		--builder:add(sleepAction(0.05));
		trajectory = trajectoryActionX(35.0, -1.5, 180.0, overides({}));
		trajectory:setTangent(-180.0);
		trajectory:splineToLinearHeading(7.75, -36.5, 0.0, -180.0);
		builder:add(trajectory:build());
		action = builder:build();
		action = initProfileAction(action);
	end,
	start = function ()
		runTimer(action, "specimenAuto2.txt");
	end
})