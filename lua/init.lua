if(data == nil) then
	print("data is nil");
end

loadOpmode({
	name = "opmode 1",
	init = function()
		print("opmode 1 init");
		dothing();
		error("stop");
	end,
	start = function()
		print("opmode 1 start");
	end,
	update = function(deltaTime, elapsedTime)
		print("dt: " .. tostring(deltaTime) .. ", et: " .. tostring(elapsedTime))
	end
})

loadOpmode({
	name = "opmode 2",
init = function()
		print("opmode 2 init");
		end
})

loadOpmode({
	name = "opmode 3",
init = function()
		print("opmode 3 init");
		end
})

print(data[0].name .. '\n')
print(data[1].name .. '\n')
print(data[2].name .. '\n')