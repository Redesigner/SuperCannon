extends VehicleBody3D

@export var maxVelocity: float
@export var acceleration: float
@export var brakingRate: float

var wheels: Array

@onready var right_wheel: Wheel
@onready var left_wheel: Wheel

func _input(event):
	if (event.is_action_pressed("ui_cancel")):
		get_tree().quit()
		
func _ready():
	for node in get_children():
		if (node is Wheel):
			wheels.append(node)
	
	right_wheel = get_node("RightFrontWheel")
	left_wheel = get_node("LeftFrontWheel")

func _physics_process(delta):
	apply_throttle_and_brakes(delta)
	turn(delta)
	
	if (position.y < -100):
		position = Vector3(0, 2, 0)
		rotation = Vector3(0, 0, 0)
		linear_velocity = Vector3(0, 0, 0)
		angular_velocity = Vector3(0, 0, 0)
	

func apply_throttle_and_brakes(_delta):
	var throttle: float = Input.get_action_strength("throttle")
	var brakeRate: float = Input.get_action_strength("brake")
	
	set_engine_force((throttle - brakeRate) * acceleration)
	for wheel in wheels:
		wheel.power(throttle - brakeRate)

func turn(_delta):	
	var turningDirection: float = Input.get_axis("turn_left", "turn_right")
	steering = lerp(steering, -turningDirection, .2)
	
	left_wheel.steer(-turningDirection)
	right_wheel.steer(-turningDirection)
