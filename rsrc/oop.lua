Class = {
	__index = Class,
	
	create = function(body, super)
		body = body or {}
		super = super or Class
		setmetatable(body, super)
		super.__index = super
		return body
	end,
	
	new = function(self, o, p)
		o = o or {}
		setmetatable(o, self)
		self.__index = self
		o:init(p)
		return o
	end,
	
	init = function(self) end,
}
--[[
Animal = Class.create()
function Animal:speak()
	print("...")
end

floyd = Animal:new{name="Floyd"}
floyd:speak()

Cat = Class.create(Animal)
function Cat:speak()
	print("Meow! My name is "..self.name)
end

billy = Cat:new{name="Billy"}
billy:speak()

nellie = Cat:new{name="Nellie"}
function nellie:speak()
	print("Mew! Hello everybody!")
end

nellie:speak()
]]--