local action = {};

addOpmode({
		name = "testOpmode",
		init = function()
				local trajectory = trajectoryAction(0, 0, 0);
				trajectory:lineToX(20);
				trajectory:lineToX(0);
				trajectory:lineToX(1);
				trajectory:lineToX(2);
				trajectory:lineToX(3);
				action = trajectory:build();
		end,
		start = function()
			run(action);
		end
})
