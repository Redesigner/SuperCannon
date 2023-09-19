extends Camera3D

@onready var target = get_node("../CannonController")

var offset

func _ready():
	offset = position

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	position.z = target.position.z
	#position.y = target.position.y + offset.y
	position.x = target.position.x + offset.x
