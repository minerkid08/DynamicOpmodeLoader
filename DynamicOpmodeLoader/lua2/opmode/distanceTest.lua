---@type Opmode
local opmode = { name = "distanceTest",
 type = 1};


function opmode.init()
    aprilTagProcessor.init(1920, 1080, 2, 255, 1.0)
end

function opmode.update()
local tag = aprilTagProcessor.getTag(20)

    if (tag:valid()) then
        TAGDIS = tag:getDist()
        vel = apirlDis(TAGDIS)
    telemetry.addDataf("vel", vel)
end

	return false;
end

addOpmode(opmode);