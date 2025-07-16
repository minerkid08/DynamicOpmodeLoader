--Colors = {
--	RED = 1,
--	BLUE = 2,
--	YELLOW = 3,
--	NONE = 4,
--	UNKNOWN = 5
--};
--
--require("blueMiddle");
--
--require("redBasket");
--require("blueBasket");
--
--require("redRight");
--require("blueLeft");
--
--require("testOpmode");
--require("visionTest");
function a()
  b();
end

function c()
  d();
end

function e()
print(4);
end

function d()
  e();
end

function b()
  c();
end

addOpmode({
  name = ":)",
  init = function()
    a();
  end
})
