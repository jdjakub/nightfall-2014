local P = {}
State = P
setmetatable(P, {__index = _G})
setfenv(1, P)

local currState = {
	onLeave = function(self) end
}

function switchTo(newState)
	currState:onLeave();
	currState = newState;
	currState:onEnter();
end