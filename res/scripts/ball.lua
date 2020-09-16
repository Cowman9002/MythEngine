Ball = {}

function Ball:new()
	self.__index = self
	
	self.rot = 0
	
	return self
end

function Ball:update()

	local bricks = Material.get("brick_mat")
	bricks:uniformv3("uColorMix", vec3.new(math.max(1.0, math.tan(Myth.frame / 30.0))))
	--Material.uniformv3(bricks, "uColorMix", vec3.new())

	local e = Entity.getSelf()
	local t = e:getTransform()
	local sphere = e:getSphere()

	sphere:setPos(vec3.new(math.sin(Myth.frame / 60), 0.5, 0.0))
	sphere:setRadius(math.abs(math.cos(Myth.frame / 40)))

	self.rot = self.rot + Myth.delta
	
	t:setRot(quat.angleAxis(self.rot, vec3.up()))
	
end

