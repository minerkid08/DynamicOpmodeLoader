require("specimen");
require("specimen5");

---@type Action
local action = {}

addOpmode({
	name = "builderTest",
	init = function ()
		action = trajectoryAction(0, 0, 0)
				:splineToConstantHeading(0, 12, 0)
				:splineToConstantHeading(12, 12, 0)
				:splineToConstantHeading(12, 0, 0)
				:splineToConstantHeading(0, 0, 0)
				:build();
	end,
	start = function ()
		run(action);
	end
})