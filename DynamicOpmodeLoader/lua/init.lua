function a()
  b();
end

function c()
  d();
end

function e()
  print("heh");
end

function d()
  e();
end

function b()
  c();
end

local c3 = 0;

addOpmode({
  name = ":)",
  type = OpmodeType.Telop,
  group = "help",
  order = "a",
    init = function()
        require("telemetry");
        actionPane:addLine("test");
        robotPane:addLine("testttttttttttttttttttttttttttttttttt");
        currentPane:addLine("test2");
        aprilTagPane:addLine("test3");
        TelemPaneManager:update();
    error("done :)");
  end,
  update = function()
    c3 = c3 + 1;
    if (c3 > 1000) then
      return true;
    end
    return false;
  end,
  stop = function()
    print("opmode stop");
  end
})
