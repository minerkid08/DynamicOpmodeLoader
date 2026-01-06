--Uses silver bot instead of main bot for ONLY HDrive (no imu) and camera
--Please use this opmode rather than main telop for turret and distance
require("modules.hdrive");
require("modules.telemPanes");

---@type Opmode
local opmode = { name = "aprilTelop",
 type = 0};

---@type HDrive
local drive;

---@integer
--Use nums between 20 (Blue goal) and 24 (Red goal)
local id = 20

function opmode.init()
    require("modules.telemetry");
    --For accurate distances, the camera resolution MUST be exact to the resolution calibrated at
    aprilTagProcessor.init(1920, 1080, 2, 255, 1.0)

    drive = HDrive.new();
    drive.imu = hardwareMap.imuGet()
end

--[[function opmode.start()

end]] --

function opmode.update()
    --Drive the bot
    local forward = gamepad.getLeftStickY();
    local right = gamepad.getLeftStickX();
    local rotate = gamepad.getRightStickX();
    drive:driveFr(forward, right, rotate);

    --Obtain the blue goal april tag
    local tag = aprilTagProcessor.getTag(id)

    aprilTagPane:addData("is valid", tag:valid())
    aprilTagPane:addData("target id", id)
    --get distance is exact to the pythagorean theorem; if its inaccurate, then the x/y is inaccurate
		--aprilTagPane:addData("distance", tag:getDist())
    aprilTagPane:addData("angle", tag:bearing())
    TelemPaneManager:update();
    return false;
end

addOpmode(opmode);
