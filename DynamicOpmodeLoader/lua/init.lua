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

local Opmode = {
  telop = 0,
  auto = 1
}

addOpmode({
  name = ":)",
  type = Opmode.telop,
  group = "help",
  order = "a",
    init = function()
    --local files = io.list("/home/minerkid08");
    --for k, v in ipairs(files) do
    --  print('\'' .. v.name .. "\', " .. tostring(v.file));
    --end
        local e = getE();

        local status, err = pcall(e.errorFun, e, 1);
        if(status) then
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
