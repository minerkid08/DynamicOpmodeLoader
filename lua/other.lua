local action = {};

LuaTrajectoryBuilder.line = LuaTrajectoryBuilder.lineToX;

addOpmode({
    name = "testOpmode",
    init = function ()
        action = trajectoryAction(0, 0, 0)
            :line(20)
            :line(0)
            :line(1)
            :line(2)
            :line(3)
            :build();
    end,
    start = function ()
        run(action);
    end,
		what = function(bool, int, num, str)
			print(tostring(bool) .. ", " .. tostring(int) .. ", " .. tostring(num) .. ", " .. str);
		end
})