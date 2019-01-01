local P = {}
Display = P
setmetatable(P, {__index = _G})
setfenv(1, P)

CT_SPRITE = 0

--TODO: DISPLAY OPTIONS!
--[[
GLUE API:
=========
getSettings(settings) : settings
setSettings(settings)
newScreen() : screen
setScreen(idx, screen)
delScreen(idx)
addComponent(screen, type, layer) : id
delComponent(screen, id)
getCmptBaseData(screen, id) : data
setCmptBaseData(screen, id, data)
getCmptUserData(screen, id) : data
setCmptUserData(screen, id, data)
getTextureDims(tex) : w, h
createGlyphAtlas(fontData, dataSize, pxSize): atlas
createSurface(w, h) : surf
createTexture(surf) : tex
freeSurface(surf)
freeTexture(tex)
updateTexture(tex, surf)
drawText(atlas, surf, str, bounds) : pxHeight
drawBox(dest, src, desc, bounds)
]]--