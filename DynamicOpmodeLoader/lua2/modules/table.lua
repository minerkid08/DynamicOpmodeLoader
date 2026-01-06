---@param list table
---@param input integer
---@return number
---@return number[]
function table.closestInt(list, input)
	--list = table.sort(list)

	---@type number
	local val = 0;
	local v2 = {};
	local prev = list[1];
	local low = 0;
	local high = 0;

	if (#list == 1) then
		val = list[1];
		table.insert(v2, -1);
		return val, v2;
	end

	for i, v in ipairs(list) do
		if (input < v) then
			low = math.abs(v - input)
			high = math.abs(list[i + 1] - input)
			if (low < high) then
				return v, v2;
			else return list[i + 1], v2; end
		end
		prev = v;
	end

	val = prev;
	table.insert(v2, 1);
	return val, v2;
end


---@param list table
---@return number
function table.mean(list)
	---@type number
	local avg = 0;

	for i, v in ipairs(list) do
		avg = v + avg
	end

	avg = avg / #list

	return avg;
end

---@param list table
---@return number
function table.max(list)
	---@type number
	local max = 0;

	for i, v in ipairs(list) do
		if (v > max) then
			max = v
		end
	end

	return max;
end