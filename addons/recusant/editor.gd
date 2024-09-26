@tool
extends EditorPlugin
class_name GameEditorPlugin

var selection = null

func _selection_changed() -> void:
	for i in selection.get_selected_nodes():
		if i is ZoneNode3D:
			i._on_shape_data_changed()

func _enter_tree() -> void:
	selection = get_editor_interface().get_selection()
	selection.selection_changed.connect(_selection_changed)

func _exit_tree() -> void:
	selection.selection_changed.disconnect(_selection_changed)
