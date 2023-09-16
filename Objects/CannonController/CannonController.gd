extends RigidBody3D

@export var maxVelocity: float
@export var acceleration: float
@export var brakingRate: float
@export var turnSpeed: float

func _input(event):
	if (event.is_action_pressed("ui_cancel")):
		get_tree().quit()

func _integrate_forces(delta):
	apply_throttle_and_brakes(delta)
	turn(delta)
	

func apply_throttle_and_brakes(_delta):
	var throttle: float = Input.get_action_strength("throttle")
	var brake: float = Input.get_action_strength("brake")
	
	var forwardVector: Vector3 = basis.z
	
	apply_force(((throttle - brake) * acceleration) * -forwardVector)
	

func turn(_delta):	
	var turningDirection: float = Input.get_axis("turn_left", "turn_right")
	#rotation_degrees.y += -turningDirection * delta * turnSpeed
	apply_torque_impulse(Vector3(0.0, -turningDirection * turnSpeed, 0.0))
