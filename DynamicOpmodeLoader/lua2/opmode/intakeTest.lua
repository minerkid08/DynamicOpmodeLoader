require("modules.hdrive");

---@type Opmode
local opmode = { name = "intakeTest",
 type = 0};

---@type HDrive
local drive;

---@type DcMotorEx
local intake;

---@type number
local power;

---@type number[]
local list = {};

function opmode.init()
	drive = HDrive.new();
	intake = hardwareMap.dcmotorexGet("intake");
end

function opmode.update()
	local forward = gamepad.getLeftStickY();
	local right = gamepad.getLeftStickX();
	local rotate = gamepad.getRightStickX();
	drive:drive(forward, right, rotate);

	if (gamepad.getDpadUp()) then
		power = 1
		intake:setPower(power);
	end

	if (gamepad.getDpadDown()) then
		power = -1
		intake:setPower(power);
	end

	if (gamepad.getCross()) then
		power = 0
		intake:setPower(power);
	end

	if (gamepad.getRightBumper()) then
		table.insert(list, intake:getCurrent());
	end

	robotPane:addData("current", intake:getCurrent());
	if (list ~= nil) then
		robotPane:addData("avg current", table.mean(list))
		robotPane:addData("max current", table.max(list))
	end
	robotPane:addData("set power", power);

	TelemPaneManager:update();

	return false;
end

addOpmode(opmode);