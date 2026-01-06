require("modules.class")

---@enum TelemPaneSplitDir
TelemPaneSplitDir = {
	Vertical = 0,
	Horizontal = 1
};

---@class Rect
---@field x integer
---@field y integer
---@field w integer
---@field h integer

---@class TelemPaneState
---@field pane TelemPane
---@field line integer

---@alias TelemPaneLine TelemPaneState[]

---@class TelemPaneContainer
---@field a TelemPane | TelemPaneContainer
---@field b TelemPane | TelemPaneContainer
---@field splitDir TelemPaneSplitDir
---@field x integer
---@field y integer
---@field w integer
---@field h integer

---@class PaneLocation
---@field parrent TelemPaneContainer?
---@field index integer

---@class TelemPaneManager
---@field w integer
---@field h integer
---@field lines TelemPaneLine[]
---@field rootPane TelemPane | TelemPaneContainer
TelemPaneManager = {
	w = 37,
	h = 19,
	lines = {}
};

---@param label string?
function TelemPaneManager:reset(label)
	self.panes = {};
	self.rootPane = TelemPane.new(1, 0, self.w - 1, self.h - 1, label or "main");
end

function TelemPaneManager:generate()
	--	local len = label:len();
	--	p.header = "-" .. label;
	--	for i = 1, w - len do
	--		p.header = p.header .. "-";
	--	end

	for y = 1, self.h do

	end
end

---@param pane TelemPane
---@param parrent TelemPaneContainer
---@return PaneLocation?
function TelemPaneManager:findPaneLocation(pane, parrent)
	if (parrent.a == pane) then
		return { parrent = parrent, index = 0 };
	end
	if (parrent.b == pane) then
		return { parrent = parrent, index = 1 };
	end
	if (parrent.a.label == nil) then
		local loc = self:findPaneLocation(pane, parrent.a);
		if (loc ~= nil) then
			return loc;
		end
	elseif (parrent.b.label == nil) then
		local loc = self:findPaneLocation(pane, parrent.b);
		if (loc ~= nil) then
			return loc;
		end
	end
	return nil;
end

---@param pane TelemPane
---@param container TelemPaneContainer
function TelemPaneManager:split(pane, container)
	if (self.rootPane == pane) then
		self.rootPane = container;
		return;
	end
	local loc = self:findPaneLocation(pane, self.rootPane);
	if (loc == nil) then
		error("tried to split invalid pane");
	end
	if (loc.index == 0) then
		loc.parrent.a = container;
	else
		loc.parrent.b = container;
	end
end

function TelemPaneManager:update()
	for y = 0, self.h - 2 do
		local s = "|" .. self:getLine(self.rootPane, y);
		telemetry.addLine(s);
	end
	telemetry.addLine("|" .. string.rep("-", self.w - 2) .. "|");
	telemetry.update();
end

---@param container TelemPaneContainer
---@param y integer
function TelemPaneManager:getLine(container, y)
	if (container.splitDir == TelemPaneSplitDir.Vertical) then
		---@type TelemPaneContainer | TelemPane
		local c = nil;
		if (y >= container.a.y and y < container.a.y + container.a.h) then
			c = container.a;
		else
			c = container.b;
		end
		if (c.label == nil) then
			return self:getLine(c, y);
		else
			return c:getLine(y);
		end
	else
		local l = nil;
		local a = container.a;
		local b = container.b;
		if (a.label == nil) then
			l = self:getLine(a, y);
		else
			l = a:getLine(y);
		end
		if (b.label == nil) then
			l = l .. self:getLine(b, y);
		else
			l = l .. b:getLine(y);
		end
		return l;
	end
end

---@class TelemPane
---@field x integer
---@field y integer
---@field w integer
---@field h integer
---@field label string
---@field header string
---@field data string[]
---@field autoReset boolean
TelemPane = {};

---@param x integer
---@param y integer
---@param w integer
---@param h integer
---@param label string
---@return TelemPane
function TelemPane.new(x, y, w, h, label)
	local p = new(TelemPane);
	p.x = x;
	p.y = y;
	p.w = w;
	p.h = h;
	p.label = label;
	p.data = {};
	p.autoReset = true;
	return p;
end

---@param label string
---@param newHeight integer?
---@return TelemPane
function TelemPane:vsplit(label, newHeight)
	newHeight = newHeight or math.floor(self.h / 2);

	local y = self.y + newHeight;
	local h = self.h - newHeight;

	local newPane = TelemPane.new(self.x, y, self.w, h, label);

	---@type TelemPaneContainer
	local container = {
		x = self.x,
		y = self.y,
		w = self.w,
		h = self.h,
		a = self,
		b = newPane,
		splitDir = TelemPaneSplitDir.Vertical
	};
	self.h = newHeight;

	TelemPaneManager:split(self, container);
	return newPane;
end

---@param label string
---@param newWidth integer?
---@return TelemPane
function TelemPane:hsplit(label, newWidth)
	newWidth = newWidth or math.floor(self.w / 2);

	local x = self.x + newWidth;
	local w = self.w - newWidth;

	local newPane = TelemPane.new(x, self.y, w, self.h, label);

	---@type TelemPaneContainer
	local container = {
		x = self.x,
		y = self.y,
		w = self.w,
		h = self.h,
		a = self,
		b = newPane,
		splitDir = TelemPaneSplitDir.Horizontal
	};
	self.w = newWidth;

	TelemPaneManager:split(self, container);
	return newPane;
end

---@param y integer
---@return string
function TelemPane:getLine(y)
	if (y == self.y) then
		return "-" .. self.label .. string.rep("-", self.w - self.label:len() - 2) .. "|";
	end
	---@type string
	local data = self.data[y - self.y];
	if (data == nil) then
		return string.rep(" ", self.w - 1) .. "|";
	end
	if (self.autoReset) then
		self.data[y - self.y] = nil;
	end
	if (data:len() >= self.w - 1) then
		return data:sub(0, self.w - 1) .. "|";
	end
	return data .. string.rep(" ", self.w - data:len() - 1) .. "|";
end

---@param line string
function TelemPane:addLine(line)
	if (#self.data >= self.h - 1) then
		for i = 1, #self.data do
			self.data[i] = self.data[i + 1];
		end
		self.data[#self.data + 1] = line;
	else
		table.insert(self.data, line);
	end
end

---@param label string
---@param value any
function TelemPane:addData(label, value)
	local line = label .. ": " .. tostring(value);
	if (#self.data >= self.h - 1) then
		for i = 1, #self.data do
			self.data[i] = self.data[i + 1];
		end
		self.data[#self.data + 1] = line;
	else
		table.insert(self.data, line);
	end
end

---@param container TelemPaneContainer
---@param indent string
function TelemPaneManager:printDebugPane(container, indent)
	print(indent ..
		"container " ..
		tostring(container.x) .. ", " ..
		tostring(container.y) .. ", " ..
		tostring(container.w) .. ", " ..
		tostring(container.h)
	);
	if (container.a.label == nil) then
		printPaneContianer(container.a, indent .. "  ");
	else
		print(indent ..
			"  pane " ..
			container.a.label .. " " ..
			tostring(container.a.x) .. ", " ..
			tostring(container.a.y) .. ", " ..
			tostring(container.a.w) .. ", " ..
			tostring(container.a.h)
		);
	end
	if (container.b.label == nil) then
		printPaneContianer(container.b, indent .. "  ");
	else
		print(indent ..
			"  pane " ..
			container.b.label .. " " ..
			tostring(container.b.x) .. ", " ..
			tostring(container.b.y) .. ", " ..
			tostring(container.b.w) .. ", " ..
			tostring(container.b.h)
		);
	end
end