Player = {}

function Player:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.rot_speed = 7.0
	self.cam_speed = 3.4
	self.cam_move_speed = 3.6
	self.cam_rot_speed = 15.0
	
	self.cam_pitch = 0
	self.cam_yaw = 0
	self.cam_dist = 3
	
	self.button_down = false
	
	self.translation = Vec3.new()
	self.cam_position = Vec3.new()
	
	return self
end

function Player:start()
	self.entity = Entity.getSelf()
	self.transform = self.entity:getTransform()
	self.body = self.entity:getRigidBody()
	self.camera = Camera.get()
end

function Player:update()

	if not Input.getControllerButton(0, Input.ControllerSouth) then
		button_down = false
	end

	if Input.getControllerButton(0, Input.ControllerSouth) and not button_down then
		self.body:setVelocity(Vec3.new(self.body:getVelocity():getx(), 5.0, self.body:getVelocity():getz()))
		button_down = true
	end

	local move_dir = Vec3.new(Input.getControllerAxis(0, Input.AxisLeftX, 0.1), 0.0,
								 Input.getControllerAxis(0, Input.AxisLeftY, 0.1))	

	local forward_rot = self.camera:getRot():getForward()
	forward_rot:sety(0.0)
	move_dir = Quat.lookat(Vec3.new(), forward_rot, Vec3.up()):rotate(move_dir)
	
	self.translation = move_dir:mul(self.move_speed)

	--	CAMERA

	self.cam_pitch = self.cam_pitch - Input.getControllerAxis(0, Input.AxisRightY, 0.1) * Myth.delta * self.cam_speed
	self.cam_yaw = self.cam_yaw + Input.getControllerAxis(0, Input.AxisRightX, 0.1) * Myth.delta * self.cam_speed
	
	self.cam_pitch = math.min(1.0, math.max(-1.0, self.cam_pitch))
	
	local cam_rotation = Quat.angleAxis(self.cam_yaw, Vec3.down()):mul(Quat.angleAxis(self.cam_pitch, Vec3.right()))
	cam_position = cam_rotation:rotate(Vec3.new(0.0, 0.0, self.cam_dist))

end

function Player:fixedUpdate()

	self.body:setVelocity(Vec3.new(self.translation:getx(), self.body:getVelocity():gety(), self.translation:getz()))
	--self.transform:translate(self.translation:mul(Myth.fixedDelta))
	
	if not self.translation:equals(Vec3.new()) then
		local targRot = Quat.lookat(Vec3.new(), self.translation:normalized(), Vec3.up())
		self.transform:setRot(Quat.slerp(self.transform:getRot(), targRot, Myth.fixedDelta * self.rot_speed))
	end
	
	self.camera:setPos(self.transform:getPos():add(cam_position))
	self.camera:setRot(Quat.lookat(self.camera:getPos(), self.transform:getPos(), Vec3.up()))
end