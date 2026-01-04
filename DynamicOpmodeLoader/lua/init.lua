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
    local files = io.list("/home/minerkid08");
    for k, v in ipairs(files) do
        print('\'' .. v.name .. "\', " .. tostring(v.file));
    end
        local e = getE();
    local f = e:printThing("heh")
        :add(1, 2);
    print(tostring(e));
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
