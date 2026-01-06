require("opmode.auto.autoBase");

---@type AutoPaths
local config = {
	start = { x = 32, y = 136, z = 180 },
	turretTarget = { x = 6, y = 138 },
	preload = function ()
		return SeqAction.newl(
			"preload",
			RobotActions.ShooterStart.new(880),
			PathAction.new(
				path.chain()
				:add(path.line(32, 136, 54.00, 90.00))
				:constantHeading(180.00)
				:build()
			),
			RobotActions.Shoot.new(4)
		)
	end,
	line1 = {
		noGate = function ()
			return SeqAction.newl(
				"line1",
				PathAction.new(
					path.chain()
					:add(path.line(54.00, 90.00, 38.00, 87.00))
					:constantHeading(180.00)
					:add(path.line(38.00, 87.00, 20.00, 87.00))
					:constantHeading(180.00)
					:build()
				),
				PathAction.new(
					path.chain()
					:add(path.line(20.00, 87.00, 54.00, 90.00))
					:constantHeading(180.00)
					:build()
				),
				RobotActions.Shoot.new(3)
			)
		end,
		gate = function ()
			return SeqAction.newl(
				"line1",
				PathAction.new(
					path.chain()
					:add(path.line(54.00, 90.00, 38.00, 87.00))
					:constantHeading(180.00)
					:add(path.line(38.00, 87.00, 23.25, 87.00))
					:constantHeading(180.00)
					:add(path.line(23.25, 87.00, 23.25, 80.00))
					:constantHeading(180.00)
					:build()
				),
				WaitForFirstAction.new(
					PathAction.new(
						path.chain()
						:add(path.line(23.25, 80.00, 17.00, 80.00))
						:constantHeading(180.00)
						:build()
					),
					Delay.new(2.0)
				),
				Delay.new(1.0),
				PathAction.new(
					path.chain()
					:add(path.line(17.00, 80.00, 54.00, 90.00))
					:constantHeading(180.00)
					:build()
				),
				RobotActions.Shoot.new(3)
			)
		end
	},
	line2 = {
		noGate = function ()
			return SeqAction.newl(
				"line2",
				PathAction.new(
					path.chain()
					:add(path.line(54.00, 90.00, 38.00, 65.00))
					:constantHeading(180.00)
					:build()
				),
				PathAction.new(
					path.chain()
					:add(path.line(38.00, 65.00, 23.25, 65.00))
					:constantHeading(180.00)
					:add(path.line(23.25, 65.00, 54.00, 90.00))
					:constantHeading(180.00)
					:build()
				),
				RobotActions.Shoot.new(3)
			)
		end,
		gate = function ()
			return SeqAction.newl(
				"line2",
				PathAction.new(
					path.chain()
					:add(path.line(54.00, 90.00, 38.00, 65.00))
					:constantHeading(180.00)
					:build()
				),
				PathAction.new(
					path.chain()
					:add(path.line(38.00, 65.00, 24.00, 65.00))
					:constantHeading(180.00)
					:add(path.line(24.00, 65.00, 24.00, 77.00))
					:constantHeading(180)
					:build()
				),
				WaitForFirstAction.new(
					PathAction.new(
						path.chain()
						:add(path.line(24.00, 77.00, 15.00, 77.00))
						:constantHeading(180.00)
						:build()
					),
					Delay.new(2.0)
				),
				PathAction.new(
					path.chain()
					:add(path.line(24.00, 65.00, 54.00, 90.00))
					:constantHeading(180.00)
					:build()
				),
				RobotActions.Shoot.new(3)
			)
		end
	},
	line3 = function ()
		return SeqAction.newl(
			"line3",
			PathAction.new(
				path.chain()
				:add(path.line(54.00, 90.00, 38.00, 39.00))
				:constantHeading(180.00)
				:build()
			),
			PathAction.new(
				path.chain()
				:add(path.line(38.00, 39.00, 24.00, 39.00))
				:constantHeading(180.00)
				:add(path.line(24.00, 39.00, 54.00, 90.00))
				:constantHeading(180.00)
				:build()
			),
			RobotActions.Shoot.new(3)
		)
	end,
	line4 = function ()
		return SeqAction.newl(
			"line4",
			PathAction.new(
				path.chain()
				:add(path.curve3(54.00, 90.00, 54, 12, 12, 12))
				:constantHeading(180.00)
				:build()
			),
			PathAction.new(
				path.chain()
				:add(path.curve3(12, 12, 54, 12, 54, 90))
				:constantHeading(180.00)
				:build()
			),
			RobotActions.Shoot.new(3)
		)
	end,
	park = function ()
		return PathAction.new(
			path.chain()
			:add(path.line(54.00, 90.00, 48.00, 71.00))
			:constantHeading(180.00)
			:build()
		)
	end
};

loadOpmodeConfigs("blueFront", config);