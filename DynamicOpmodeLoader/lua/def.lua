---@class Opmode
---@field name string
---@field init function?
---@field start function?
---@field update function?

---@param opmode Opmode
function addOpmode(opmode) end

---@class Action

---@class LuaTrajectoryBuilder
---@field setTangent fun(self: table, tangent: number): LuaTrajectoryBuilder
---@field lineToX fun(self: table, x: number): LuaTrajectoryBuilder
---@field lineToY fun(self: table, y: number): LuaTrajectoryBuilder
---@field splineToLinearHeading fun(self: table, x: number, y: number, h: number, t: number): LuaTrajectoryBuilder
---@field splineToConstantHeading fun(self: table, x: number, y: number, t: number): LuaTrajectoryBuilder
---@field build fun(self: table): Action
LuaTrajectoryBuilder = {};

---@param x number
---@param y number
---@param h number
---@return LuaTrajectoryBuilder
function trajectoryAction(x, y, h) end

---@param action Action
function run(action) end