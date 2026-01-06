require("modules.utils")

---@enum shooterState
shooterState = {
	Open = 0,
	Close = 1,
	Wait = 2
}
---@class shooter
---@field motorL DcMotorEx
---@field motorR DcMotorEx
---@field gate Servo
---@field gateClosed number
---@field gateOpen number
---@field time number
---@field vel number
---@field count number
---@field state shooterState
---@field openDelay number
---@field closeDelay number
---@field delay number
---@field waitForReady boolean
---@field velocityDataPoints number[]
---@field running boolean
shooter = {
	gateOpen = 0.3,
	gateClosed = 0,
	openDelay = 0.16,
	openDelayEnd = 0.16,
	closeDelay = 0.35,
	state = shooterState.Close,
	waitForReady = false,
	velocityDataPoints = { 0, 0, 0, 0, 0 },
	running = false
}

function shooter:init()
	self.gate = hardwareMap.servoGet("transfer");
	self.motorL = hardwareMap.dcmotorexGet("flywheelLeft");
	self.motorR = hardwareMap.dcmotorexGet("flywheelRight");
	self.motorL:setMode(DcMotorRunMode.RunUsingEncoder);
	self.motorR:setMode(DcMotorRunMode.RunUsingEncoder);
	self.motorL:setDirection(Direction.Reverse);
end

---@param vel number
function shooter:start(vel)
	self.vel = vel;
	self.motorL:setVelocity(vel);
	self.motorR:setVelocity(vel);
end

---@param x number
---@param y number
---@param dist number
function shooter:updateVelocity(x, y, dist)
	if (x < 0) then
		x = -x;
	end

	local vel = 0;
	if (x > 12 * 6) then
		if (y > 48) then
			vel = 940;
		else
			vel = 1200;
		end
	else
		if (y > 48) then
			vel = 920;
		else
			vel = 1160;
		end
	end

	--if (dist >= 20000) then
	--	vel = 1160;
	--elseif (dist >= 15000) then
	--	vel = 1080;
	--elseif (dist >= 14000) then
	--	vel = 1080;
	--elseif (dist >= 8000) then
	--	vel = 940;
	--elseif (dist >= 6000) then
	--	vel = 880;
	--elseif (dist >= 2000) then
	--	vel = 880;
	--else
	--	vel = 880;
	--end
	if (not self.running) then
		vel = 0;
	end
	if (vel ~= self.vel) then
		self:start(vel);
	end
end

function shooter:stop()
	self.motorL:setPower(0);
	self.motorR:setPower(0);
end

---@param et number
function shooter:shoot(et)
	self.state = shooterState.Open;
	self.count = 1;
	self.gate:setPosition(self.gateOpen);
	self.time = et;
	self.delay = self.openDelay;
end

---@param et number
---@param count number
function shooter:shootNum(et, count)
	self.state = shooterState.Open;
	self.count = count;
	if (logFile ~= nil) then
		logFile:write(("gate open: %f - %d"):format(et, self.motorL:getVelocity()));
	end
	actionPane:addData("open gate", et);
	self.gate:setPosition(self.gateOpen);
	self.time = et;
	self.delay = self.openDelay;
end

---@param et number
---@return boolean
function shooter:update(et)
	if (self.time == nil) then
		return false;
	end

	if (self.time + self.delay <= et) then
		if (self.state == shooterState.Open) then
			self.gate:setPosition(self.gateClosed);
			if (logFile ~= nil) then
				logFile:write(("gate closed: %f - %d"):format(et, self.motorL:getVelocity()));
			end
			self.time = et;
			self.state = shooterState.Close;
			self.delay = self.closeDelay;
		else
			self.state = shooterState.Open;
			if (self.count == 1) then
				self.count = 0;
				self.time = nil;
				return true;
			else
				self:shootNum(et, self.count - 1);
				if (self.count == 1) then
					self.delay = self.openDelayEnd;
				else
					self.delay = self.openDelay;
				end
			end
		end
	end
	return false;
end

function shooter:ready()
	local vel = self.motorL:getVelocity();
	self.velocityDataPoints[5] = self.velocityDataPoints[4];
	self.velocityDataPoints[4] = self.velocityDataPoints[3];
	self.velocityDataPoints[3] = self.velocityDataPoints[2];
	self.velocityDataPoints[2] = self.velocityDataPoints[1];
	self.velocityDataPoints[1] = vel;
	local sum = self.velocityDataPoints[2] - self.velocityDataPoints[1];
	sum = sum + self.velocityDataPoints[3] - self.velocityDataPoints[2];
	sum = sum + self.velocityDataPoints[4] - self.velocityDataPoints[3];
	sum = sum + self.velocityDataPoints[5] - self.velocityDataPoints[4];
	local slope = sum / 5;
	return vel >= self.vel - 40 and vel <= self.vel + 40 --and slope >= -40 and slope <= 40;
end

function shooter:close()
	self.gate:setPosition(self.gateClosed);
end

function shooter:telem()
	robotPane:addData("shooterCurL", shooter.motorL:getCurrent());
	robotPane:addData("shooterVelL", shooter.motorL:getVelocity());
	robotPane:addData("shooterCurR", shooter.motorR:getCurrent());
	robotPane:addData("shooterVelR", shooter.motorR:getVelocity());
end