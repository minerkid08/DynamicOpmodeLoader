Direction = {
	Forward = 0,
	Reverse = 1
};

DcMotorRunMode = {
  RunWithoutEncoder = 0,
  StopAndResetEncoder = 1,
	RunToPosition = 2,
  RunUsingEncoder = 3
};

DcMotorZeroPowerBehavior = {
	Float = 0,
	Brake = 1
}

---@enum Team
Team = {
	Red = 0,
	Blue = 1
};

math.tau = math.pi * 2;
math.pi2 = math.pi / 2;
math.pi4 = math.pi / 4;