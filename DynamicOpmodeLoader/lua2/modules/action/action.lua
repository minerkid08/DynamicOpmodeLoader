require("modules.class");

---@enum ActionState
ActionState = {
	Running = 1,
	Done = 2,
	Error = 3,
	ErrCont = 4
};

---@class Action
---@field start fun(self: Action, et: number)?
---@field update fun(self: Action, dt: number, et: number): ActionState
---@field finish fun(self: Action)?
---@field error fun(self: Action)?
---@field genProfileStr fun(self: Action, file: file*, indent: string?)?
---@field dontLog boolean?

---@class SleepAction : Action
---@field delay number
---@field startTime number
SleepAction = {
	mt = {
		__tostring = function (self)
			return ("SleepAction(%.2fs)"):format(self.delay);
		end
	}
};

---@param time number
---@return SleepAction
function SleepAction.new(time)
	local a = new(SleepAction);
	a.delay = time;
	a.startTime = -1;
	return a;
end

function SleepAction:start(et)
	self.startTime = et;
end

---@param dt number
---@param et number
---@return ActionState
function SleepAction:update(dt, et)
	if (self.startTime + self.delay <= et) then
		return ActionState.Done;
	end
	return ActionState.Running;
end

---@class PathAction : Action
---@field path PathChain
---@field maxPower number
PathAction = {
	mt = {
		__tostring = function (self)
			if (self.maxPower == nil) then
				return "PathAction";
			end
			return ("PathAction(%.2f%%)"):format(self.maxPower * 100);
		end
	}
};

---@param path PathChain
---@param maxPower number?
---@return PathAction
function PathAction.new(path, maxPower)
	local a = new(PathAction);
	a.path = path;
	a.maxPower = maxPower or 1.0;
	return a;
end

function PathAction:start()
	follower.followPathc(self.path, self.maxPower);
end

function PathAction:error()
	follower.stop();
end

---@param dt number
---@param et number
---@return ActionState
function PathAction:update(dt, et)
	---follower.update();
	if (follower.isBusy()) then
		return ActionState.Running;
	end
	--follower.stop();
	return ActionState.Done;
end

---@class CallbackAction : Action
CallbackAction = {
	mt = {
		__tostring = function (self)
			return "CallbackAction";
		end
	}
};

---@param callback function
---@return CallbackAction
function CallbackAction.new(callback)
	local a = new(CallbackAction);
	a.start = callback;
	return a;
end

---@param dt number
---@param et number
---@return ActionState
function CallbackAction:update(dt, et)
	return ActionState.Done;
end

---@class ErrorAction : Action
---@field msg string?
ErrorAction = {
	mt = {
		__tostring = function (self)
			return "ErrorAction - " .. tostring(self.msg);
		end
	}
};

---@param msg string
---@return ErrorAction
function ErrorAction.new(msg)
	local a = new(ErrorAction);
	a.msg = msg;
	return a;
end

---@param dt number
---@param et number
---@return ActionState
function ErrorAction:update(dt, et)
	return ActionState.Error;
end