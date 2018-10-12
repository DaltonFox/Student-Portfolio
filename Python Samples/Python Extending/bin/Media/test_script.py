import ssuge
ssuge.log("Testing the ssuge c-python module log function.")
ssuge.createGroup("swaggers")
for i in range(5):
	boi = ssuge.GameObject("swaggers", "swag-boi" + str(i))
	boi.setScale(0.1, 0.1, 0.1)
	boi.setPosition((i - 2) * 9, 10, 0)
	boi.createMeshComponent("ogrehead.mesh")
	
camera = ssuge.GameObject("swaggers", "camera")
camera.setPosition(0, 10, 50)
camera.rotate(-10, 1, 0, 0)
camera.createCameraComponent(0)