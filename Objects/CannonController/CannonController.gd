extends VehicleBody3D

@export var maxVelocity: float
@export var acceleration: float
@export var brakingRate: float

var parts: Array

@onready var right_wheel: Wheel
@onready var left_wheel: Wheel

func _input(event):
	if (event.is_action_pressed("ui_cancel")):
		get_tree().quit()
		return
	
	if (event.is_action_pressed("activate")):
		for part in parts:
			part.activate()


func _ready():
	for node in get_children():
		if (node is Part):
			parts.append(node)
	
	right_wheel = get_node("RightFrontWheel")
	left_wheel = get_node("LeftFrontWheel")

func _physics_process(delta):
	apply_throttle_and_brakes(delta)
	turn(delta)
	control(delta)
	
	if (position.y < -100):
		position = Vector3(0, 2, 0)
		rotation = Vector3(0, 0, 0)
		linear_velocity = Vector3(0, 0, 0)
		angular_velocity = Vector3(0, 0, 0)
	

func apply_throttle_and_brakes(_delta):
	var throttle: float = Input.get_action_strength("throttle")
	var brakeRate: float = Input.get_action_strength("brake")
	
	for part in parts:
		part.power(-throttle + brakeRate)

func turn(_delta):	
	var turningDirection: float = Input.get_axis("turn_left", "turn_right")	
	left_wheel.steer(-turningDirection)
	right_wheel.steer(-turningDirection)
	
func control(_delta):
	var input: Vector2 = Input.get_vector("control_right", "control_left", "control_down", "control_up")
	for part in parts:
		part.control(input);
