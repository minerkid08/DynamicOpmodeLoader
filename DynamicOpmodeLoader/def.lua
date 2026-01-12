---@class E
E = {}
---@param a0 number
---@param a1 number
---@return number
function E:add(a0, a1) end
---@param a0 number
---@return string
function E:errorFun(a0) end
---@param a0 function
function E:doThing(a0) end
---@param a0 Enum
function E:enumThing(a0) end
---@param a0 string
---@return E
function E:printThing(a0) end

---@class Enum
Enum = {}

---@return E
getE = function() end
enum = {
---@type Enum
  fwd = {},
---@type Enum
  bck = {}
}
---@class Dir
---@field name string
---@field file boolean

---@param path string
---@return Dir[]
function io.list(path) end

---@enum OpmodeType
OpmodeType = {Telop = 0, Auto = 1};

---@class Opmode
---@field name string
---@field type OpmodeType
---@field group string
---@field order string

---@param opmode Opmode
function addOpmode(opmode) end