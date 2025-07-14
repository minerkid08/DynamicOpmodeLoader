local defaultVel = 75;
local defaultMinAccel = -35;
local defaultMaxAccel = 60;

---@class Overides
---@field vel number?
---@field minAccel number?
---@field maxAccel number?

---@param overideTable Overides
---@return number, number, number
function overides(overideTable)
	local vel = overideTable.vel or defaultVel;
	local minAccel = overideTable.minAccel or defaultMinAccel;
	local maxAccel = overideTable.maxAccel or defaultMaxAccel;
	return vel, minAccel, maxAccel;
end

function initExternalFunctions()
	---@return number
	---@return number
	---@return number
	function LuaTrajectoryBuilder:getEnd()
		return self:getEndX(), self:getEndY(), self:getEndH();
	end
end