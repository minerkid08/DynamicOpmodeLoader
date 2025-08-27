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
local c3 = 0;
addOpmode({
  name = ":)",
  init = function()
    local e = getE();
    local f = e:printThing("heh")
        :add(1, 2);
    print(tostring(f));
    --e:doThing(a);
  end,
  update = function()
    c3 = c3 + 1;
    if (c3 > 1000) then
      return true;
    end
    return false;
  end
})
