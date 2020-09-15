Player = {}

function Player:new()
	self.__index = self
	
	self.move_speed = 4.0
	self.cam_speed = 3.4
	self.cam_move_speed = 3.6
	self.cam_rot_speed = 10.0
	
	self.cam_pitch = 0
	self.cam_yaw = 0
	self.cam_dist = 2
	self.cam_pos = vec3.new()
	
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

	local translation = vec3.new(Input.getControllerAxis(0, 0, 0.1),
								 Input.getControllerAxis(0, 4, 0.0) -
								 Input.getControllerAxis(0, 5, 0.0),
								 Input.getControllerAxis(0, 1, 0.1))	
	
	--local translation = vec3.new(h_trans, 0.0, v_trans)

	local forward_rot = cam:getRot():getForward()
	forward_rot:sety(0.0)
	translation = quat.lookat(vec3.new(), forward_rot, vec3.up()):rotate(translation)

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

	local rot_rot = quat.angleAxis(self.cam_yaw, vec3.down())
	local rot_pos = rot_rot:rotate(vec3.new(0.0, cam_y, cam_x))

	self.cam_pos = vec3.lerp(self.cam_pos, t:getPos(), Myth.delta * self.cam_pos:dist(t:getPos()) * self.cam_move_speed)

	cam:setPos(self.cam_pos:add(rot_pos))

	local targ_rot = quat.lookat(cam:getPos(), t:getPos(), vec3.up())

	cam:setRot(quat.slerp(cam:getRot(), targ_rot, Myth.delta * self.cam_rot_speed))
end

