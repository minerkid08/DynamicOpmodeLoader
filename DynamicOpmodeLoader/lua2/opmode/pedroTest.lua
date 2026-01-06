require("modules.telemetry");
require("modules.action.action");
require("modules.action.seqAction");
require("modules.action.parallelAction");

---@type Opmode
local opmode = { name = "pedroTest",
 type = 1};

---@type Action
local a;

function opmode.init()
	a = ParallelAction.new(
		PathAction.new(
			path.chain()
			:add(path.line(0, 0, 72, 0))
			:constantHeading(0)
			:add(path.curve3(72, 0, 96, 0, 96, 24))
			:constantHeading(0)

			:add(path.line(96, 24, 96, 72))
			:constantHeading(0)
			:add(path.curve3(96, 72, 96, 96, 72, 96))
			:constantHeading(0)

			:add(path.line(72, 96, 24, 96))
			:constantHeading(0)
			:add(path.curve3(24, 96, 0, 96, 0, 72))
			:constantHeading(0)

			:add(path.line(0, 72, 0, 0))
			:constantHeading(0)
			:build()
		)
	)
end

function opmode.start()
	a:start(0);
end

function opmode.update(dt, et)
	drivePane:addData("x", follower.getPositionX());
	drivePane:addData("y", follower.getPositionY());
	drivePane:addData("h", follower.getPositionH());
	TelemPaneManager:update();
	follower.telem();
	local state = a:update(dt, et);
	if (state == ActionState.Running) then
		return false;
	elseif (state == ActionState.Done) then
		return true;
	end
	error(("root action '%s' failed"):format(tostring(a)));
	return true;
end

addOpmode(opmode);