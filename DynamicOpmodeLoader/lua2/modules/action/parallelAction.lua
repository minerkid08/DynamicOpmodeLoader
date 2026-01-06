require("modules.action.action");
require("modules.class");

---@class ParallelAction: Action
---@field actions Action[]
---@field initalActions Action[]
---@field startTime number
---@field endTimes number[]
---@field endTime number
ParallelAction = {
	dontLog = true,
	mt = {
		__tostring = function (self)
			return "ParallelAction";
		end
	}
}

---@param ... Action
---@return ParallelAction
function ParallelAction.new(...)
	local a = new(ParallelAction);
	a.actions = { ... };
	a.initalActions = { ... };
	a.endTimes = {};
	return a;
end

---@param et number
function ParallelAction:start(et)
	for _, action in ipairs(self.actions) do
		if (action.start ~= nil) then
			action:start(et);
		end
	end
	self.startTime = et;
end

function ParallelAction:error()
	for _, action in ipairs(self.actions) do
		if (action.error ~= nil) then
			action:error();
		end
	end
	self.endTime = -1;
end

---@param dt number
---@param et number
---@return ActionState
function ParallelAction:update(dt, et)
	local count = 0;
	for k, action in pairs(self.actions) do
		count = count + 1;
		local newState = action:update(dt, et);
		if (newState == ActionState.Running) then
			goto continue
		elseif (newState == ActionState.Done) then
			if (action.finish ~= nil) then
				action:finish();
			end
			count = count - 1;
			self.endTimes[k] = et - self.startTime;
			self.actions[k] = nil;
		elseif (newState == ActionState.Error) then
			for _, a in pairs(self.actions) do
				if (action.error ~= nil) then
					a:error();
				end
			end
			if (action.dontLog == nil) then
				log.e("actions", "action '" .. tostring(action) .. "' failed");
				if (panes ~= nil) then
					panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
				end
			end
			self.endTime = -1;
			self.endTimes[k] = 1000;
			return ActionState.Error;
		elseif (newState == ActionState.ErrCont) then
			if (action.error ~= nil) then
				action:error();
			end
			count = count - 1;
			if (action.dontLog == nil) then
				log.e("actions", "action '" .. tostring(action) .. "' failed");
				if (panes ~= nil) then
					panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
				end
			end
			self.actions[k] = nil;
			self.endTimes[k] = -(et - self.startTime);
		end
		::continue::
	end
	if (count == 0) then
		self.endTime = et;
		return ActionState.Done;
	end
	return ActionState.Running;
end

---@param file file*
---@param indent string?
function ParallelAction:genProfileStr(file, indent)
	if (indent == nil) then
		if (self.endTime == nil) then
			file:write("timing not complete\n");
			file:write(("%s - ?\n"):format(tostring(self)));
		elseif (self.endTime < 0) then
			file:write("root action failed, timing not complete\n");
			file:write(("%s (failed) - ?\n"):format(tostring(self)));
		else
			file:write(("%s - %.2f\n"):format(tostring(self), self.endTime - self.startTime));
		end
	end
	indent = indent or "";

	for i, action in ipairs(self.initalActions) do
		local time = self.endTimes[i];
		local tree;
		local ext;
		if (i == #self.initalActions) then
			tree = indent .. "└─";
			ext = "  ";
		else
			tree = indent .. "├─";
			ext = "│ ";
		end
		if (time == nil) then
			file:write(tree .. ("%s (running) - ?\n"):format(action));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		elseif (time == 1000) then
			file:write(tree .. ("%s (failed) - ?\n"):format(action));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		elseif (time < 0) then
			file:write(tree .. ("%s (failed, continuing) - %.2f\n"):format(action, -time));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		else
			file:write(tree .. ("%s - %.2f\n"):format(action, time));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		end
	end
end