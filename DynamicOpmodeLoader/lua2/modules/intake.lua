---@enum IntakeState
IntakeState = {
	Forward = 1,
	Stopped = 0,
	Reverse = -1
}

---@class intake
---@field motor DcMotorEx
---@field state IntakeState
---@field speed number
intake = {
	speed = 1
}

function intake:init()
	self.motor = hardwareMap.dcmotorexGet("intake");
	self.state = IntakeState.Stopped;
end

function intake:forward()
	self.motor:setPower(self.speed);
	self.state = IntakeState.Forward;
end

function intake:reverse()
	self.motor:setPower(-self.speed);
	self.state = IntakeState.Reverse;
end

function intake:stop()
	self.motor:setPower(0);
	self.state = IntakeState.Stopped;
end