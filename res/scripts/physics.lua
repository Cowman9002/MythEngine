
function simulate(a, b)

	local collision = a:collide(b)

	if collision:hit() then
		local t = collision:normal()
		t = t:mul(collision:depth())
		a:translate(t)
	end

end