require("settings.lua")
require("states.lua")

playerData = rsrc.loadDataFile(rsrc.DATA_PLAYER)

state:switchTo(state.NETMAP);