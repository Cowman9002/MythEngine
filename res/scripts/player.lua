Player = {}

function Player:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.rot_speed = 4.0
	
	return self
end

function Player:update()

	--local cam = camera:get()
	
	--local rotation = vec3.new(Input.getControllerAxis(0, 2, 0.1), -Input.getControllerAxis(0, 3, 0.1), 0.0):mul(0.016 * self.rot_speed)

	--self.rot = quat.angleAxis(rotation:getx(), vec3.down()):mul(quat.angleAxis(rotation:gety(), self.rot:getRight())):mul(self.rot)

	local e = entity.getSelf()
	local t = entity.getTransform(e)

	local translation = vec3.new(Input.getControllerAxis(0, 0, 0.1),
								 Input.getControllerAxis(0, 4, 0.0) -
								 Input.getControllerAxis(0, 5, 0.0),
								Input.getControllerAxis(0, 1, 0.1))
	
	--translation = self.rot:rotate(translation)

	t:setPos(t:getPos():add(translation:mul(0.016 * self.move_speed)))

	--cam:setPos(self.pos:add(vec3.new(0.0, 0.0, 3.0)))
	--cam:setRot(self.rot)
end

