Ball = {}

function Ball:new()
	self.__index = self
	
	self.rot = 0
	
	return self
end

function Ball:update()

	local bricks = Material.get("brick_mat")
	bricks:uniformv3("uColorMix", Vec3.new(math.max(1.0, math.tan(Myth.frame / 30.0))))
	--Material.uniformv3(bricks, "uColorMix", Vec3.new())

	local e = Entity.getSelf()
	local t = e:getTransform()
	local sphere = e:getSphere()
	local aabb = e:getAABB()

	--sphere:setPos(Vec3.new(math.sin(Myth.frame / 60), 3.0, 0.0))
	--sphere:setRadius(math.abs(math.cos(Myth.frame / 40)) + 0.5)
	
	--aabb:setPos(Vec3.new(0.0, 0.0, math.sin(Myth.frame / 60) * 0.5))
	--aabb:setExtents(Vec3.new(math.abs(math.cos(Myth.frame / 120))))

	self.rot = self.rot + Myth.delta
	
	t:setRot(Quat.angleAxis(self.rot, Vec3.up()))
	
end

