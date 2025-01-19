if(data == nil) then
	print("data is nil");
end

addOpmode({
	name = "opmode 1",
	init = function()
		print("opmode 1 init");
		local res = getThing();
		print(res:dothing());
		
		local action = trajectoryAction(0, 0, 0);
		action:lineToX(30)
		action:setTangent(-90)
		action:splineToConstantHeading(40, 40, 45);
		action = action:build();
		
		runAction(action);
	end,
	start = function()
		print("opmode 1 start");
	end,
	update = function(deltaTime, elapsedTime)
		print("dt: " .. tostring(deltaTime) .. ", et: " .. tostring(elapsedTime))
	end
})

addOpmode({
	name = "opmode 2",
init = function()
		print("opmode 2 init");
		end
})

addOpmode({
	name = "opmode 3",
init = function()
		print("opmode 3 init");
		end
})