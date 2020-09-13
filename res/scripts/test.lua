Player = {}

function Player:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.rot_speed = 4.0
	self.pos = vec3.new()
	self.rot = quat.angleAxis(0, vec3.new())
	
	self.pos:setx(0)
	self.pos:sety(2)
	self.pos:setz(3)
	
	return self
end

function Player:update()

	local cam = camera:get()
	
	local rotation = vec3.new(Input.getControllerAxis(0, 2, 0.1), -Input.getControllerAxis(0, 3, 0.1), 0.0):mul(0.016 * self.rot_speed)

	self.rot = quat.angleAxis(rotation:getx(), vec3.down()):mul(quat.angleAxis(rotation:gety(), self.rot:getRight())):mul(self.rot)

	local translation = vec3.new(Input.getControllerAxis(0, 0, 0.1),
								 Input.getControllerAxis(0, 4, 0.0) -
								 Input.getControllerAxis(0, 5, 0.0),
								 Input.getControllerAxis(0, 1, 0.1))
	
	translation = self.rot:rotate(translation)

	self.pos = self.pos:add(translation:mul(0.016 * self.move_speed))

	cam:setPos(self.pos)
	cam:setRot(self.rot)
end

