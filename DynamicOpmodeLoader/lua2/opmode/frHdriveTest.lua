require("modules.hdrive");

---@type Opmode
local opmode = { name = "frHdriveTest",
 type = 0};

---@type HDrive
local drive;

function opmode.init()
	drive = HDrive.new();
	drive.imu = hardwareMap.spimuGet();
end

function opmode.update()
	local forward = gamepad.getLeftStickY();
	local right = gamepad.getLeftStickX();
	local rotate = gamepad.getRightStickX();
	drive:driveFr(forward, right, rotate);
	return false;
end

addOpmode(opmode);