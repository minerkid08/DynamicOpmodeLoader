---@type Opmode
local opmode = { name = "turretTest",
type = 0};

local turretStateLabels = { "tracking", "waiting", "manual" };

function opmode.init()
	turret.init();
end


function opmode.update()
	telemetry.addLine("turret state: " .. turretStateLabels[turret.getState()]);

	if (gamepad.getCircle2()) then
		turret.startManual();
	end
	if (gamepad.getCross2()) then
		turret.startAutomatic();
	end
	if (gamepad.getSquare2()) then
		turret.resetHeading();
	end

	turret.update(gamepad.getLeftStickX());
	telemetry.update();

	return false;
end

addOpmode(opmode);