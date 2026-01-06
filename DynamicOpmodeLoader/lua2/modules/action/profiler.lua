require("modules.action.seqAction");
require("modules.action.parallelAction");

DATADIR = DATADIR or "/sdcard/";

profiler = {};

---@param filename string
---@param action Action
function profiler.genString(filename, action)
	if (action.genProfileStr == nil) then
		error("error: 'getProfileStr' is nil on the root action");
	end
	local file = io.open(DATADIR .. filename, "wb");
	if (file == nil) then
		error("error: failed to open file '" .. DATADIR .. filename .. '\'');
	end
	action:genProfileStr(file);
	file:close();
end