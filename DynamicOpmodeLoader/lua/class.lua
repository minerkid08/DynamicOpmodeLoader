---@generic T
---@param t T
---@return T
function new(t)
	local n = {};
	for k, v in pairs(t) do
		n[k] = v;
	end
	n.new = nil;
	n.mt = nil;
	n.__id = tostring(n):sub(8);
	if(t.mt ~= nil) then
	   setmetatable(n, t.mt);
	end
	return n;
end