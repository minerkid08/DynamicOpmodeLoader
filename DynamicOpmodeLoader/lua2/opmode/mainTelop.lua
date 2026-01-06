require("modules.hdrive");
require("modules.telemPanes");
require("modules.intake");
require("modules.shooter");

---@type HDrive
local drive;

---@type number[]
local shooterVelocity = { 900, 1120 }

---@type DcMotor
local turretMotor = {};

---@type vec3
local startPos;

---@type boolean
local shooterAutomatic = true;

---@type integer
local turretOffset = 0;

---@type file*
local logFile;

function telopInit()
	logFile = io.open(DATADIR .. "telop" .. tostring(os.time), "w");
	require("modules.telemetry");

	drive = HDrive.new();
	drive.localizerMode = LocalizerMode.pinpoint;
	drive.pinpoint = hardwareMap.pinpointGet();

	if (save.containsb("resetTurret")) then
		actionPane:addLine("values from auto found");
		local x = save.loadd("x");
		local y = save.loadd("y");
		local h = save.loadd("h");

		startPos = { x = x, y = y, z = h };


		local resetTurret = save.loadb("resetTurret");
		turret.init(resetTurret);
	else
		actionPane:addLine("values from auto not found, reseting");
		turret.init(true);
	end

	turretMotor = turret.getMotor();
	--aprilTagProcessor.init(1920, 1080, 2, 255, 1.0)

	intake:init();
	shooter:init();
end

---@type vec3
local initPos = nil;

---@type vec2
local targetPos = nil;

function telopStartBlue()
	drive.offset = math.pi2;
	turret.setTargetTag(20);
	turret.startAutomatic();
	shooter:close();
	initPos = { x = 144 - 15.5 / 2, y = 9, z = math.pi2 };
	--targetPos = { x = 7, y = 138 };
	targetPos = { x = 0, y = 144 };

	if (startPos == nil) then
		startPos = initPos;
	end

	drive.pinpoint:setPosX(startPos.x);
	drive.pinpoint:setPosY(startPos.y);
	drive.pinpoint:setHeading(startPos.z);
	shooter.vel = shooterVelocity[1];
	shooter:start(shooter.vel);
end

function telopStartRed()
	drive.offset = -math.pi2;
	turret.setTargetTag(24);
	turret.startAutomatic();
	shooter:close();
	initPos = { x = -(144 - 15.5 / 2), y = 9, z = math.pi2 };
	--targetPos = { x = -7, y = 138 };
	targetPos = { x = 0, y = 144 };

	if (startPos == nil) then
		startPos = initPos;
	end

	drive.pinpoint:setPosX(startPos.x);
	drive.pinpoint:setPosY(startPos.y);
	drive.pinpoint:setHeading(startPos.z);
	shooter.vel = shooterVelocity[1];
	shooter:start(shooter.vel);
end

function telopUpdate(dt, et)
	drive.pinpoint:update();

	local x = drive.pinpoint:getX();
	local y = drive.pinpoint:getY();
	local h = drive.pinpoint:getHeading();

	--Drive the bot
	local forward = gamepad.getLeftStickY();
	local right = gamepad.getLeftStickX();
	local rotate = gamepad.getRightStickX();
	drive:driveFr(forward, right, rotate);

	--Obtain the blue goal april tag
	--local bTag = aprilTagProcessor.getTag(20)
	--local dist = 0;

	--if (bTag:valid()) then
	--	dist = bTag:getDist()
	--end

	local dx = targetPos.x - x;
	local dy = targetPos.y - y;
	local angle = math.atan(dy, dx) - h;
	angle = math.deg(angle);
	if (angle > 180) then
		angle = angle - 360;
	end
	if (angle < -180) then
		angle = angle + 360;
	end
	turret.turnTo(angle + turretOffset);

	--Forward/stop intake
	if (gamepad.getRightBumper2()) then
		if (intake.state == IntakeState.Forward) then
			intake:stop();
		else
			intake:forward();
		end
	end

	--Reverse/stop intake
	if (gamepad.getLeftBumper2()) then
		if (intake.state == IntakeState.Reverse) then
			intake:stop();
		else
			intake:reverse();
		end
	end

	if (gamepad.getDpadUp2()) then
		turretOffset = turretOffset + 1;
		logFile:write(("x: %6.2f, y: %6.2f, h: %6.4f, angle: %6.2f, offset: %d"):format(x, y, h, angle, turretOffset));
	end
	if (gamepad.getDpadDown2()) then
		turretOffset = turretOffset - 1;
		logFile:write(("x: %6.2f, y: %6.2f, h: %6.4f, angle: %6.2f, offset: %d"):format(x, y, h, angle, turretOffset));
	end

	if (gamepad.getDpadLeft2()) then
		shooterAutomatic = false;
		shooter.vel = shooterVelocity[1];
		shooter:start(shooter.vel);
	end
	if (gamepad.getDpadRight2()) then
		shooterAutomatic = false;
		shooter.vel = shooterVelocity[2];
		shooter:start(shooter.vel);
	end

	--Run/don't run specifically the shooter
	if (gamepad.getCircle2()) then
		shooterAutomatic = true;
		shooter.running = true;
	end
	if (gamepad.getTriangle2()) then
		shooter.running = false;
	end

	--Start intake and shooter
	if (gamepad.getCross2()) then
		intake:forward();
		shooter:shootNum(et, 3);
	end

	if (gamepad.getSquare2()) then
		turret.lockOnTag();
	end

	--if (shooterAutomatic) then
	--local dist = dx * dx + dy * dy;
	--shooter:updateVelocity(x, y, 0);
	--end

	--turret.updateMotor();

	if (gamepad.getStart()) then
		drive.pinpoint:setPosX(initPos.x);
		drive.pinpoint:setPosY(initPos.y);
		drive.pinpoint:setHeading(initPos.z);
	end

	--Automatically updates
	shooter:update(et);

	local tps = 1 / dt;

	robotPane:addData("tps", tps);
	robotPane:addData("x", drive.pinpoint:getX());
	robotPane:addData("y", drive.pinpoint:getY());
	robotPane:addData("h", math.deg(drive.pinpoint:getHeading()));
	robotPane:addData("dist", "nil");
	robotPane:addData("tarPos", turretMotor:getTargetPosition());
	robotPane:addData("curPos", turretMotor:getCurrentPosition());
	robotPane:addData("offset", turretOffset);

	shooter:telem();

	robotPane:addData("setVel", shooter.vel);

	currentPane:addData("fl", drive.frontLeft:getCurrent());
	currentPane:addData("fr", drive.frontRight:getCurrent());
	currentPane:addData("bl", drive.backLeft:getCurrent());
	currentPane:addData("br", drive.backRight:getCurrent());
	currentPane:addData("sl", shooter.motorL:getCurrent());
	currentPane:addData("sr", shooter.motorR:getCurrent());
	currentPane:addData("in", intake.motor:getCurrent());

	TelemPaneManager:update();

	return false;
end

---@type Opmode
local telopRed = {
    name = "mainTelopRed",
	type = 0,
	init = telopInit,
	start = telopStartRed,
	update = telopUpdate
};

---@type Opmode
local telopBlue = {
    name = "mainTelopBlue",
	type = 0,
	init = telopInit,
	start = telopStartBlue,
	update = telopUpdate
};

addOpmode(telopRed);
addOpmode(telopBlue);