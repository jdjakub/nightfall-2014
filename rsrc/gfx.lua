local P = {}
Gfx = P
setmetatable(P, {__index = _G})
setfenv(1, P)

FONT_DEFAULT = {Rsrc.MAIN, "TerminalVector.ttf", 16}
FONT_HUGE = {Rsrc.MAIN, "TerminalVector.ttf", 90}

IMG_RED = {Rsrc.MAIN, "backg_red.png"}
IMG_UI = {Rsrc.MAIN, "ui.png"}

DIMS_MAX = -1

local surfaces = {}
local textures = {}
local atlases = {}
local screens = {}
local regions = {}

function getSurface(surf)
	if surfaces[surf] == nil then
		surfaces[surf] = Rsrc.getSurface(unpack(surf))
	end
	return surfaces[surf]
end

function getTexture(tex)
	if textures[tex] == nil then
		textures[tex] = Rsrc.getTexture(unpack(tex))
	end
	return textures[tex]
end

function getAtlas(font)
	if atlases[font] == nil then
		local data, size = Rsrc.getRawResourceData(font[1], font[2])
		local atlas = Display.createGlyphAtlas(data, size, font[3])
		
		Rsrc.freeRawResourceData(data)
		atlases[font] = atlas;
	end
	return atlases[font]
end

function getRegionDesc(img, rgn)
	if regions[rgn]
		local resfile, filename = unpack(img)
	
	
end

function dispatchInput(scr, groupId, cmptId, inData)
	local screen = screens[scr]
	
	screen:onInput(groupId, cmptId, inData)
end

Screen = Class.create()

function Screen:init(o)
	self._ptr = Display.newScreen()
	screens[self._ptr] = self
	local s = Display.getSettings()
	self._txtSurf = Display.createSurface(s.w, s.h)
	self._boxSurf = Display.createSurface(s.w, s.h)
	self._txtTex = Display.createTexture(self._txtSurf)
	self._boxTex = Display.createTexture(self._boxSurf)
	self._widgets = {}
	self._groupId = 1
end

function Screen:delete()
	-- HACK!! May need changing a bit if deleting an arbitary, unbound screen is to be supported
	Display.delScreen(0)
	Display.freeSurface(self._txtSurf)
	Display.freeTexture(self._txtTex)
end

-- TODO: RE-USE IDS!
function Screen:genGroupId()
	local id = self._groupId
	self._groupId = id + 1
	print("Generated new group ID: "..id..", new is "..self._groupId)
	return id
end

function Screen:addWidget(className, o)
	local w = {}
	w._scr = self
	w._groupId = self:genGroupId()
	w = className:new(w, o)
	self._widgets[w._groupId] = w
	return w;
end

function Screen:setBackground(tex)
	local settings = Display.getSettings()
	local backg = self:addWidget(Image, {
		layer = 0,
	})
	backg:setData({
		visible = true,
		x = 0, y = 0,
		w = settings.w, h = settings.h,
		sx = 0, sy = 0,
		sw = TEX_MAX, sh = TEX_MAX,
		tex = tex,
	})
	return backg
end

function Screen:setCurrent()
	Display.setScreen(0, self._ptr)
end

function Screen:getTextTexture()
	return self._txtTex
end

function Screen:getBoxTexture()
	return self._boxTex
end

function Screen:drawText(atlas, str, bounds)
	local h = Display.drawText(atlas, self._txtSurf, str, bounds)
	
	Display.updateTexture(self._txtTex, self._txtSurf)
	return h
end

function Screen:drawBox(src, desc,  bounds)
	local boxDesc = Display.drawBox(self._boxSurf, src, desc, bounds)
	
	Display.updateTexture(self._boxTex, self._boxSurf)
	return boxDesc
end

function Screen:onInput(groupId, cmptId, inData)
	self._widgets[groupId]:onInput(cmptId, inData)
end

Widget = Class.create()

function Widget:init(o)
	self.layer = o.layer
	self._cmpts = {}
end

function Widget:onInput(cmptId, inData)
	print("Received input on cmpt ", cmptId)
end

Image = Class.create(nil, Widget)

function Image:init(o)
	Widget.init(self, o)
	
	local s = self._scr._ptr
	self._cmpts = {
		Display.addComponent(s, Display.CT_SPRITE, self.layer)
	}
	Display.setCmptBaseData(s, self._cmpts[1], {groupId = self._groupId})
end

function Image:getData()
	local s = self._scr._ptr
	local data = Display.getCmptBaseData(s, self._cmpts[1])
	local udata = Display.getCmptUserData(s, self._cmpts[1])
	
	for k,v in pairs(udata) do data[k] = v end
	return data
end

function Image:setData(o)
	local s = self._scr._ptr
	
	o.tex = getTexture(o.tex)
	if o.sw == DIMS_MAX or o.sh == DIMS_MAX then -- WON'T WORK ALL TIME
		o.sw, o.sh = Display.getTextureDims(o.tex)
	end
	o.groupId = self._groupId
	Display.setCmptBaseData(s, self._cmpts[1], o)
	Display.setCmptUserData(s, self._cmpts[1], o)
end

Text = Class.create(nil, Image)

function Text:init(o)
	Image.init(self, o)
	
	local s = self._scr
	Display.setCmptUserData(s._ptr, self._cmpts[1], {tex = s:getTextTexture()})
end

function Text:setData(o)
	local s = self._scr
	local h = 0
	
	if o.str ~= nil then
		local data = self:getData()
		
		h = s:drawText(getAtlas(o.font), o.str, {
			x = o.x or data.x, y = o.y or data.y,
			w = o.w or data.w, h = o.h
		})
	end
	h = o.h or h
	
	o.tex = nil
	o.h = h
	o.sx = o.x
	o.sy = o.y
	o.sw = o.w
	o.sh = o.h
	Display.setCmptBaseData(s._ptr, self._cmpts[1], o)
	Display.setCmptUserData(s._ptr, self._cmpts[1], o)
end

Box = Class.create(nil, Widget)

function Box:init(o)
	Widget.init(self, o)
	
	local s = self._scr
	self._cmpts = {	
		Display.addComponent(s._ptr, Display.CT_SPRITE, self.layer)
		Display.addComponent(s._ptr, Display.CT_SPRITE, self.layer+1)
	}
	local data = {groupId = self._groupId, tex = s:getBoxTexture()}
	Display.setCmptBaseData(s, self._cmpts[1], data)
	Display.setCmptUserData(s, self._cmpts[1], data)
	Display.setCmptBaseData(s, self._cmpts[2], data)
	Display.setCmptUserData(s, self._cmpts[2], data)
end

function Box:setData(o)
	local s = self._scr
	
	if o.rgn ~= nil then
		local boxDesc = getRegionDesc(o.img, o.rgn)
		local data = self:getData()
		
		boxDesc = s:drawBox(getSurface(o.img), boxDesc, {
			x = o.x or data.x, y = o.y or data.y,
			w = o.w or data.w, h = o.h or data.h
		})
		
		-- Set data for centre sprite
		o.tex = nil
		o.x = boxDesc.centre.x	o.y = boxDesc.centre.y
		o.w = boxDesc.centre.w	o.h = boxDesc.centre.h
		o.sx = o.x				o.sy = o.y
		o.sw = o.w				o.sh = o.h
		Display.setCmptBaseData(s._ptr, self._cmpts[2], o)
		Display.setCmptUserData(s._ptr, self._cmpts[2], o)
		
		-- Now the border
		o.col = nil
		o.x = boxDesc.tl.x		o.y = boxDesc.tl.y
		o.w = o.w or data.w		o.h = o.h or data.h
		o.sx = o.x				o.sy = o.y
		o.sw = o.w				o.sh = o.h
		Display.setCmptBaseData(s._ptr, self._cmpts[1], o)
		Display.setCmptUserData(s._ptr, self._cmpts[1], o)
	end
end