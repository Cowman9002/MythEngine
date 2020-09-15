Ball = {}

function Ball:new()
	self.__index = self
	
	return self
end

function Ball:update()

	local bricks = Material.get("brick_mat")
	bricks:uniformv3("uColorMix", vec3.new(math.max(1.0, math.tan(Myth.frame / 30.0))))
	--Material.uniformv3(bricks, "uColorMix", vec3.new())

	local e = Entity.getSelf()
	local t = Entity.getTransform(e)

	--t:setPos(vec3.new(math.sin(Myth.frame / 30.0), 4.0, -1.0))
	
	local cam = Camera.get()
	
	t:setRot(quat.lookat(t:getPos(), cam:getPos(), vec3.up()))
	
end

