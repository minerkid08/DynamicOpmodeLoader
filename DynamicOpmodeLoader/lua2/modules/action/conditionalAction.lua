require("modules.action.action");
require("modules.class");

---@class CondtitionalAction : Action
---@field mainAction Action
---@field altAction Action
---@field startTime number
---@field initTime number
---@field mainTime number
---@field altTime number
---@field currentAction integer
ConditionalAction = {
	dontLog = true,
	mt = {
		__tostring = function (self)
			return "ConditionalAction";
		end
	}
}

---@param main Action
---@param alt Action
---@return CondtitionalAction
function ConditionalAction.new(main, alt)
	local a = new(ConditionalAction);
	a.mainAction = main;
	a.altAction = alt;
	a.currentAction = 0;
	return a;
end

---@param et number
function ConditionalAction:start(et)
	if (self.mainAction.start ~= nil) then
		self.mainAction:start(et);
	end
	self.initTime = et;
	self.startTime = et;
end

---@param dt number
---@param et number
---@return ActionState
function ConditionalAction:update(dt, et)
	if (self.currentAction == 0) then
		local action = self.mainAction;
		local state = action:update(dt, et);
		if (state == ActionState.Error or state == ActionState.ErrCont) then
			if (action.error ~= nil) then
				action:error();
			end
			if (self.altAction.start ~= nil) then
				self.altAction:start(et);
			end
			if (action.dontLog == nil) then
				log.e("actions", "action '" .. tostring(action) .. "' failed");
				if (panes ~= nil) then
					panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
				end
			end
			self.mainTime = -(et - self.startTime);
			self.startTime = et;
			self.currentAction = 1;
		elseif (state == ActionState.Done) then
			if (action.finish ~= nil) then
				action:finish();
			end
			self.mainTime = et - self.startTime;
			self.startTime = et;
			return ActionState.Done;
		end
		return ActionState.Running;
	else
		local action = self.altAction;
		local state = action:update(dt, et);
		if (state == ActionState.ErrCont or state == ActionState.Error) then
			if (action.error ~= nil) then
				action:error();
			end
			self.altTime = -(et - self.startTime);
			self.startTime = et;
			if (action.dontLog == nil) then
				log.e("actions", "action '" .. tostring(action) .. "' failed");
				if (panes ~= nil) then
					panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
				end
			end
			if (state == ActionState.Error) then
				return ActionState.Error;
			else
				return ActionState.Done;
			end
		elseif (state == ActionState.Done) then
			if (action.finish ~= nil) then
				action:finish();
			end
			self.altTime = (et - self.startTime);
			self.startTime = et;
			return ActionState.Done;
		end
		return ActionState.Running;
	end
end

function ConditionalAction:error()
	if (self.currentAction == 0) then
		if (self.mainAction.error ~= nil) then
			self.mainAction:error();
		end
	else
		if (self.altAction.error ~= nil) then
			self.altAction:error();
		end
	end
end

---@param file file*
---@param indent string
function ConditionalAction:genProfileStr(file, indent)
	local tree = indent .. "├─";
	local ext = "│ ";

	local action = self.mainAction;
	local time = self.mainTime;
	if (time == nil) then
		file:write(tree .. ("%s (running) - ?\n"):format(action));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	elseif (time > 0) then
		file:write(tree .. ("%s - %.2f\n"):format(action, time));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	elseif (time < 0) then
		file:write(tree .. ("%s (failed) - %.2f\n"):format(action, -time));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	end

	tree = indent .. "└─";
	ext = "  ";
	action = self.altAction;

	if (self.altTime == nil and self.currentAction == 0) then
		file:write(tree .. ("%s (branch not hit)\n"):format(action));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
		return;
	end

	time = self.altTime;

	if (time == nil) then
		file:write(tree .. ("%s (running) - ?\n"):format(action));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	elseif (time > 0) then
		file:write(tree .. ("%s - %.2f\n"):format(action, time));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	elseif (time < 0) then
		file:write(tree .. ("%s (failed) - %.2f\n"):format(action, -time));
		if (action.genProfileStr ~= nil) then
			action:genProfileStr(file, indent .. ext);
		end
	end
end