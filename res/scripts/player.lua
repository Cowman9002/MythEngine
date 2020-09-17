Player = {}

function Player:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.cam_speed = 3.4
	self.cam_move_speed = 3.6
	self.cam_rot_speed = 15.0
	
	self.cam_pitch = 0
	self.cam_yaw = 0
	self.cam_dist = 2
	self.cam_minpower = 1.0
	self.cam_pos = Vec3.new()
	
	return self
end

function Player:update()

	local cam = Camera.get()
	local e = Entity.getSelf()
	local t = Entity.getTransform(e)

	--local h_trans = 0
	--if Input.getKey(Input.KeyA) then h_trans = h_trans - 1 end
	--if Input.getKey(Input.KeyD) then h_trans = h_trans + 1 end
	
	--local v_trans = 0
	--if Input.getKey(Input.KeyW) then v_trans = v_trans - 1 end
	--if Input.getKey(Input.KeyS) then v_trans = v_trans + 1 end

	local translation = Vec3.new(Input.getControllerAxis(0, 0, 0.1),
								 Input.getControllerAxis(0, 4, 0.0) -
								 Input.getControllerAxis(0, 5, 0.0),
								 Input.getControllerAxis(0, 1, 0.1))	
	
	--local translation = Vec3.new(h_trans, 0.0, v_trans)

	local forward_rot = cam:getRot():getForward()
	forward_rot:sety(0.0)
	translation = Quat.lookat(Vec3.new(), forward_rot, Vec3.up()):rotate(translation)

	t:setPos(t:getPos():add(translation:mul(Myth.delta * self.move_speed)))

	--local h_cam = 0
	--if Input.getKey(Input.KeyLeft) then h_cam = h_cam - 1 end
	--if Input.getKey(Input.KeyRight) then h_cam = h_cam + 1 end
	
	--local v_cam = 0
	--if Input.getKey(Input.KeyDown) then v_cam = v_cam - 1 end
	--if Input.getKey(Input.KeyUp) then v_cam = v_cam + 1 end

	self.cam_pitch = self.cam_pitch - Input.getControllerAxis(0, 3, 0.1) * Myth.delta * self.cam_speed
	self.cam_yaw = self.cam_yaw + Input.getControllerAxis(0, 2, 0.1) * Myth.delta * self.cam_speed
	--self.cam_pitch = self.cam_pitch + v_cam * Myth.delta
	--self.cam_yaw = self.cam_yaw - h_cam * Myth.delta
	
	self.cam_pitch = math.min(1, math.max(-1, self.cam_pitch))

	local cam_x = math.cos(self.cam_pitch) * self.cam_dist
	local cam_y = math.sin(self.cam_pitch) * self.cam_dist

	local rot_rot = Quat.angleAxis(self.cam_yaw, Vec3.down())
	local rot_pos = rot_rot:rotate(Vec3.new(0.0, cam_y, cam_x))

	local spring_factor = math.max(self.cam_minpower, self.cam_pos:dist(t:getPos()) / self.cam_dist)

	self.cam_pos = Vec3.lerp(self.cam_pos, t:getPos(), Myth.delta * spring_factor * self.cam_move_speed)

	cam:setPos(self.cam_pos:add(rot_pos))

	local targ_rot = Quat.lookat(cam:getPos(), t:getPos(), Vec3.up())

	cam:setRot(Quat.slerp(cam:getRot(), targ_rot, Myth.delta * self.cam_rot_speed))
end

