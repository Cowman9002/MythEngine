Ball = {}

function Ball:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.rot_speed = 4.0
	
	return self
end

function Ball:update()

	local e = entity.getSelf()
	local t = entity.getTransform(e)

	t:setPos(vec3.new(math.sin(Myth.frame / 30.0), 4.0, -1.0))

end

