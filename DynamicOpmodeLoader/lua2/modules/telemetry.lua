require("modules.telemPanes");

TelemPaneManager:reset("actions");

local aPane = TelemPaneManager.rootPane;
---@cast aPane TelemPane
actionPane = aPane;
actionPane.autoReset = false;
robotPane = actionPane:vsplit("robot", 5);
currentPane = robotPane:hsplit("current");
aprilTagPane = currentPane:vsplit("aprilTag", 8);