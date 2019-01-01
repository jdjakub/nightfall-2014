local P = {}
Rsrc = P
setmetatable(P, {__index = _G})
setfenv(1, P)
MAIN = "rsrc/main.zip"

--[[
GLUE API:
=========
loadResFile(filename) : resFile
unloadResFile(resFile)
getResData(resId) : resData, dataSize
freeResData(resData)
resToTexture(resData, dataSize) : tex
resToString(resData, dataSize) : string
]]--

local openFiles = {}

-- Returns the C light user data for the file requested
local function getFile(filename)
	local file = openFiles[filename]
	if file == nil then
		openFiles[filename] = loadFile(filename)
	end
	return openFiles[filename]
end

function loadFile(filename)
	local file = openFiles[filename]
	if file ~= nil then
		unloadResFile(file)
	end
	openFiles[filename] = loadResFile(filename)
	return openFiles[filename]
end

function unloadFile(filename)
	local file = openFiles[filename]
	if file ~= nil then
		unloadResFile(file)
	end
end

function getRawResourceData(filename, resName)
	return getResData(getFile(filename), resName)
end

function freeRawResourceData(data)
	freeResData(data);
end

function getTexture(filename, texName)
	local raw, size = getRawResourceData(filename, texName)
	local tex = resToTexture(raw, size)
	freeResData(raw)
	return tex
end

function execScript(filename, scriptName, fenv)
	local file = getFile(filename)
	local raw, size = getResData(file, scriptName);
	local script = resToString(raw, size)
	freeResData(raw)
	local chk, err = loadstring(script)
	if chk == nil then
		error(err)
	else
		if fenv ~= nil then
			setfenv(chk, fenv)
		end
		chk()
	end
end