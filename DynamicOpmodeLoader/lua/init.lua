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
    --local files = io.list("/home/minerkid08");
    --for k, v in ipairs(files) do
    --  print('\'' .. v.name .. "\', " .. tostring(v.file));
    --end
    local e = getE();

    local status, err = pcall(e.errorFun, e, 1);
    if (status) then
      print(err);
    else
      print("call failed with error '" .. err .. "'");
    end

    local f = e:printThing("heh")
        :add(1, 2);
    print(tostring(e));
        print(tostring(f));
    e:enumThing(enum.fwd);
    e:enumThing(enum.bck);
        e:doThing(a);
    e:err();
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
