local P = {}
Display = P
setmetatable(P, {__index = _G})
setfenv(1, P)
print("Hello, World! From display.lua")

CT_SPRITE = 0