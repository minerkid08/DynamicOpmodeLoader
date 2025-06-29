local action = {};

addOpmode({
	name = "testOpmode",
	init = function ()
		action = trajectoryAction(0, 0, 0)
			:lineToX(20)
			:lineToX(0)
			:lineToX(1)
			:lineToX(2)
			:lineToX(3)
			:build();
	end,
	start = function ()
		run(action);
	end,
	---@param a string
	---@param b number
	---@param c number
	dothing = function (a, b, c)
		print(a .. tostring(b + c));
	end
})
---@param a number
---@param b number
function dothing4(a, b)
	print(tostring(a + b));
end