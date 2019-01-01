print("AND SO IT BEGINS...")
Display.pushScreen()
--[[comps = {}
numComps = 30
data = {
	visible = true
}
for i=1,numComps do
	comps[i] = Display.addComponent(Display.CT_SPRITE, numComps-i)
	Display.setCmptBaseData(-1, comps[i], data)
	print("Visible = ", Display.getCmptBaseData(-1, comps[i]).visible)
end
udata = {sx=3, sy=1, sw=4, sh=1}
Display.setCmptUserData(-1, comps[numComps], udata)
udata = Display.getCmptUserData(-1, comps[numComps])
for k,v in pairs(udata) do
	print(k, "=", v)
end
]]--