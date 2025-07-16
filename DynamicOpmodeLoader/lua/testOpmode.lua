local action;

function test(a)
  print("a is " .. tostring(a));
end

addOpmode({
  name = "testOpmode",
  init = function()
    doThing(test);
    local builder = sequentalAction()
    builder:add(sleepAction(4));
    builder:add(sleepAction(4))
    builder:add(markerAction("m1"))
    builder:add(sleepAction(6))
    builder:add(markerAction("m2"))
    builder:add(sleepAction(8))
    builder:add(markerAction("m3"))
    action = builder:build();
  end,
  start = function()
    runTimer(action, "testOpmode.txt");
  end
})
