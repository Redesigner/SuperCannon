extends Node3D


func _process(delta):
	position += delta * global_transform.basis.z * -20
