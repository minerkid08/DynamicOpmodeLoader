require("modules.telemPanes");

---@type Opmode
local opmode = {
    name = "TwoMotorShooterTest",
    type = 0,
};

---@type number[]
local shooterVelocity = { 1200, 1300, 1600 }

---@type string[]
local shooterLabel = { "Close", "Moderate", "Far" }

---@type number
local id = 1;

---@type DcMotorEx
local motorA;

---@type DcMotorEx
local motorB;

function opmode.init()
	require("modules.telemetry");
	motorA = hardwareMap.dcmotorexGet("flywheelLeft");
	motorB = hardwareMap.dcmotorexGet("flywheelRight");

	motorB:setDirection(Direction.Reverse);
end

function opmode.update(dt, et)
	if (gamepad.getDpadDown2() and id ~= 1) then
		id = id - 1
	end

	if (gamepad.getDpadUp2() and id ~= 3) then
		id = id + 1
	end

	if (gamepad.getCircle2()) then
		motorA:setPower(1);
		motorB:setPower(1);
		motorA:setVelocity(shooterVelocity[id]);
		motorB:setVelocity(shooterVelocity[id]);
	end

	if (gamepad.getTriangle2()) then
		motorA:setVelocity(0);
		motorB:setVelocity(0);
		motorA:setPower(0);
		motorB:setPower(0);
	end

	robotPane:addLine(shooterLabel[id]);
	robotPane:addData("setVel", shooterVelocity[id]);
	robotPane:addData("left", motorA:getVelocity());
	robotPane:addData("right", motorB:getVelocity());
	TelemPaneManager:update();

	return false;
end

addOpmode(opmode);