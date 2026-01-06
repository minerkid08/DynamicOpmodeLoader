---@type Opmode
local opmode = { name = "TransferTest",
 type = 0};

---@type DcMotor
local m1;

---@type DcMotor
local m2;

function opmode.init()
	m1 = hardwareMap.dcmotorGet("motor1");
	m2 = hardwareMap.dcmotorGet("motor2");
end

function opmode.start()
	m1:setPower(-1);
	m2:setPower(-0.5);
end

addOpmode(opmode);