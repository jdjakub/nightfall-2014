State.HOME = {}

function State.HOME:onEnter()
	local homeScr = Gfx.Screen:new()
	local backg = homeScr:setBackground(Gfx.TEX_RED)
	local titleTxt = homeScr:addWidget(Gfx.Text, {layer=5})
	titleTxt:setData({
		visible = true,
		font = Gfx.FONT_HUGE,
		str = "NIGHTFALL",
		x = 64, y = 64,
		w = 600,
		a = 255
	})
	data = titleTxt:getData() print("Title - groupID is ", data.groupId)

	local subTxt = homeScr:addWidget(Gfx.Text, {layer=5})
	subTxt:setData({
		visible = true,
		font = Gfx.FONT_DEFAULT,
		str = "[If there was a term for a pre-alpha stage, it would be that]",
		x = 64, y = data.y + data.h + 16,
		w = 700,
		a = 127
	})
	data = subTxt:getData() print("Subtitle - groupID is ", data.groupId)

	local netmapBtn = homeScr:addWidget(Gfx.Text, {layer=5})
	netmapBtn:setData({
		visible = true,
		font = Gfx.FONT_DEFAULT,
		str = "NETMAP",
		x = 500, y = 500,
		w = 300
	})
	function netmapBtn:onInput(cmptId, inData)
		State.switchTo(State.NETMAP)
	end

	homeScr:setCurrent()
	
	self.scr =  homeScr
	self.backg = backg
	self.titleTxt = titleTxt
	self.subTxt = subTxt
	self.netmapBtn = netmapBtn
end

function State.HOME:onLeave()
	self.scr:delete()
end