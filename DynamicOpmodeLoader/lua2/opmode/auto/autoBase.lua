require("modules.action");
require("modules.action.robot");

require("modules.telemPanes");

---@alias AutoPathFunc fun(): Action

---@class AutoPathGate
---@field gate AutoPathFunc
---@field noGate AutoPathFunc

---@class AutoPaths
---@field start vec3
---@field turretTarget vec2
---@field preload AutoPathFunc
---@field line1 AutoPathGate
---@field line2 AutoPathGate
---@field line3 AutoPathFunc
---@field line4 AutoPathFunc?
---@field park AutoPathFunc

if (DISABLE_ROBOT ~= nil) then
	local tmpAction = {
		update = function ()
			return ActionState.Done;
		end
	};
	tmpAction.new = function ()
		return tmpAction;
	end
	RobotActions.IntakeStop = tmpAction;
	RobotActions.Intake = tmpAction;
	RobotActions.ShooterDisable = tmpAction;
	RobotActions.Shoot = tmpAction;
	RobotActions.ShooterStart = tmpAction;
end

---@type Action
action = nil;

---@type string
profileFileName = "unammed auto";

---@type file*
logFile = nil;

---@type vec2
local turretTarget = nil;

function autoUpdate(dt, et)
	local tps = 1 / dt;
	robotPane:addData("tps", tps);
	robotPane:addData("x", follower.getPositionX());
	robotPane:addData("y", follower.getPositionY());
	robotPane:addData("h", follower.getPositionH());
	--currentPane:addData("fl", drive.frontLeft:getCurrent());
	--currentPane:addData("fr", drive.frontRight:getCurrent());
	--currentPane:addData("bl", drive.backLeft:getCurrent());
	--currentPane:addData("br", drive.backRight:getCurrent());
	currentPane:addData("sl", shooter.motorL:getCurrent());
	currentPane:addData("sr", shooter.motorR:getCurrent());
	currentPane:addData("in", intake.motor:getCurrent());

	shooter:telem();
	TelemPaneManager:update();
	follower.update();
	follower.telem();

	local x = follower.getPositionX();
	local y = follower.getPositionY();

	local dx = turretTarget.x - x;
	local dy = turretTarget.y - y;
	local angle = math.atan(dy, dx) - follower.getPositionH();
	angle = math.deg(angle);
	if (angle > 180) then
		angle = angle - 360;
	end
	if (angle < -180) then
		angle = angle + 360;
	end
	turret.turnTo(angle);

	local dist = dx * dx + dy * dy;
	shooter:updateVelocity(x, y, dist);

	logFile:write(
		" x: " .. tostring(follower.getPositionX()) ..
		" y: " .. tostring(follower.getPositionY()) ..
		" z: " .. tostring(follower.getPositionH()) .. "\n"
	);

	local state = action:update(dt, et);
	if (state ~= ActionState.Running) then
		profiler.genString(profileFileName, action);
		logFile:close();
		save.saved("x", follower.getPositionX());
		save.saved("y", follower.getPositionY());
		save.saved("h", follower.getPositionH());
		save.saveb("resetTurret", false);
		if (state ~= ActionState.Done) then
			error(("root action '%s' failed"):format(tostring(action)));
		end
		return true;
	end
	return false;
end

function autoStart()
	logFile = io.open(DATADIR .. "/log.txt", "wb");
	turret.startAutomatic();
	shooter.running = true;
	action:start(0);
end

local genPathFuncs = {
	---@param config AutoPaths
	---@return Action
	[3] = function (config)
		return SeqAction.new(config.preload(), config.park());
	end,
	---@param config AutoPaths
	---@return Action
	[6] = function (config)
		return SeqAction.new(config.preload(), config.line1.noGate(), config.park());
	end,
	---@param config AutoPaths
	---@return Action
	[9] = function (config)
		return SeqAction.new(config.preload(), config.line1.noGate(), config.line2.noGate(), config.park());
	end,
	---@param config AutoPaths
	---@return Action
	[12] = function (config)
		return SeqAction.new(config.preload(), config.line1.gate(), config.line2.noGate(), config.line3(), config.park());
	end,
	---@param config AutoPaths
	---@return Action
	[15] = function (config)
		return SeqAction.new(config.preload(), config.line1.noGate(), config.line2.gate(), config.line3(), config.line4(),
			config.park());
	end
};

local order = {
  [3] = "a",
  [6] = "b",
  [9] = "c",
  [12] = "d",
  [15] = "e"
}

---@param name string
---@param prefix number
---@param config AutoPaths
function addConfig(name, prefix, config)
	addOpmode({
        name = name .. tostring(prefix),
        type = 1,
        group = name,
	  order = order[prefix],
		init = function ()
			--drive = HDrive.new(false);
			require("modules.telemetry");
			follower.setPosition(config.start.x, config.start.y, config.start.z);
			action = genPathFuncs[prefix](config);
			turretTarget = config.turretTarget;
			shooter:init();
			intake:init();
			turret.init(true);
		end,
		start = autoStart,
		update = autoUpdate
	});
end

---@param name string
---@param config AutoPaths
function loadOpmodeConfigs(name, config)
	addConfig(name, 3, config);
	addConfig(name, 6, config);
	addConfig(name, 9, config);
	addConfig(name, 12, config);
	addConfig(name, 15, config);
end