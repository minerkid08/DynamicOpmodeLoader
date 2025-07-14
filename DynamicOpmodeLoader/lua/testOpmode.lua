local action;

addOpmode({
	name = "testOpmode",
    init = function()
	  doThing(4, 4.2);
		action = sequentalAction()
				:add(sleepAction(4))
				:add(markerAction("m1"))
				:add(sleepAction(6))
				:add(markerAction("m2"))
				:add(sleepAction(8))
				:add(markerAction("m3"))
				:build();
	end,
	start = function ()
		runTimer(action, "testOpmode.txt");
	end
})