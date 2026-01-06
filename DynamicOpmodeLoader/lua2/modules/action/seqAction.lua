require("modules.action.action");
require("modules.class");

---@class SeqAction : Action
---@field actions Action[]
---@field times number[]
---@field startTime number
---@field initTime number
---@field label string?
SeqAction = {
	dontLog = true,
	mt = {
		__tostring = function (self)
			if (self.label == nil) then
				return "SeqAction";
			end
			return ("SeqAction(%s)"):format(self.label);
		end
	}
}

---@param ... Action
---@return SeqAction
function SeqAction.new(...)
	local a = new(SeqAction);
	a.actions = { ... };
	a.times = {};
	return a;
end

---@param label string
---@param ... Action
---@return SeqAction
function SeqAction.newl(label, ...)
	local a = new(SeqAction);
	a.label = label;
	a.actions = { ... };
	a.times = {};
	return a;
end

---@param et number
function SeqAction:start(et)
	self.index = 1;
	local a = self.actions[1];
	if (a.start ~= nil) then
		a:start(et);
	end
	self.startTime = et;
	self.initTime = et;
end

function SeqAction:error()
	local a = self.actions[self.index];
	if (a.error ~= nil) then
		a:error();
	end
	self.startTime = -1;
end

---@param dt number
---@param et number
---@return ActionState
function SeqAction:update(dt, et)
	local action = self.actions[self.index];
	local state = action:update(dt, et);

	if (state == ActionState.Running) then
		return ActionState.Running;
	end

	if (state == ActionState.Error) then
		if (action.error ~= nil) then
			action:error();
		end
		if (action.dontLog == nil) then
			log.e("actions", "action '" .. tostring(action) .. "' failed");
			if (panes ~= nil) then
				panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
			end
		end
		table.insert(self.times, 1000);
		return ActionState.Error;
	end

	if (state == ActionState.Done) then
		if (action.finish ~= nil) then
			action:finish();
		end
		table.insert(self.times, et - self.startTime);
	end

	if (state == ActionState.ErrCont) then
		if (action.error ~= nil) then
			action:error();
		end
		if (action.dontLog == nil) then
			log.e("actions", "action '" .. tostring(action) .. "' failed");
			if (panes ~= nil) then
				panes.action:addLine("error: action '" .. tostring(action) .. "' failed");
			end
		end
		table.insert(self.times, -(et - self.startTime) - 1);
	end

	self.startTime = et;
	self.index = self.index + 1;
	local nextAction = self.actions[self.index];

	if (nextAction == nil) then
		return ActionState.Done;
	end
	if (nextAction.start ~= nil) then
		nextAction:start(et);
	end

	return ActionState.Running;
end

---@param file file*
---@param indent string?
function SeqAction:genProfileStr(file, indent)
	if (indent == nil) then
		if (self.startTime == nil) then
			file:write("timing not complete\n");
			file:write(("%s - ?\n"):format(tostring(self)));
		elseif (self.startTime < 0) then
			file:write("root action failed, timing not complete\n");
			file:write(("%s (failed) - ?\n"):format(tostring(self)));
		else
			file:write(("%s - %.2f\n"):format(tostring(self), self.startTime - self.initTime));
		end
	end

	indent = indent or "";

	for i, action in ipairs(self.actions) do
		local time = self.times[i];
		local tree;
		local ext;
		if (i == #self.actions) then
			tree = indent .. "└─";
			ext = "  ";
		else
			tree = indent .. "├─";
			ext = "│ ";
		end
		if (time == nil) then
			if (self.index == i) then
				file:write(tree .. ("%s (running) - ?\n"):format(action));
				if (action.genProfileStr ~= nil) then
					action:genProfileStr(file, indent .. ext);
				end
			else
				file:write(tree .. ("%s - ?\n"):format(action));
				if (action.genProfileStr ~= nil) then
					action:genProfileStr(file, indent .. ext);
				end
			end
		elseif (time < 0) then
			file:write(tree .. ("%s (failed, continuing) - %.2f\n"):format(action, -self.times[i] - 1));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		elseif (time == 1000) then
			file:write(tree .. ("%s (failed) - ?\n"):format(action));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		else
			file:write(tree .. ("%s - %.2f\n"):format(action, self.times[i]));
			if (action.genProfileStr ~= nil) then
				action:genProfileStr(file, indent .. ext);
			end
		end
	end
end