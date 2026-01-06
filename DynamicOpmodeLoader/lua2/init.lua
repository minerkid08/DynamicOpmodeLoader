SCRIPTDIR = SCRIPTDIR or "lua";

IMPORTDIR = { "opmode" };

--DISABLE_ROBOT = true;

require("opmode.auto");

for j, dir in pairs(IMPORTDIR) do
	local files = io.list(SCRIPTDIR .. '/' .. dir);

	local dirname = dir:gsub('/', '.');

	for i, file in ipairs(files) do
		if (file.file) then
			local ind = file.name:find("%.") - 1;
			local name = file.name:sub(1, ind);
			local ext = file.name:sub(ind + 2);

			if (ext == "lua") then
				require(dirname .. '.' .. name);
			end
		end
	end
end