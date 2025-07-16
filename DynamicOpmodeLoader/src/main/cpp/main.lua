function a()
  b();
end
function c()
  d();
end
function e()
  print(g());
end
function d()
  e();
end
function b()
  c();
end

a();
