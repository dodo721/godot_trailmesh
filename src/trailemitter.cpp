#include "trailemitter.hpp"
#include "trailmesh.hpp"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void TrailEmitter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_material", "new_material"), &TrailEmitter::set_material);
	ClassDB::bind_method(D_METHOD("get_material"), &TrailEmitter::get_material);
	ClassDB::bind_method(D_METHOD("set_curve", "new_curve"), &TrailEmitter::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &TrailEmitter::get_curve);
	ClassDB::bind_method(D_METHOD("set_gradient", "new_gradient"), &TrailEmitter::set_gradient);
	ClassDB::bind_method(D_METHOD("get_gradient"), &TrailEmitter::get_gradient);
	ClassDB::bind_method(D_METHOD("set_geometry_root", "nodepath"), &TrailEmitter::set_geometry_root);
	ClassDB::bind_method(D_METHOD("get_geometry_root"), &TrailEmitter::get_geometry_root);
	ClassDB::bind_method(D_METHOD("set_num_points", "value"), &TrailEmitter::set_num_points);
	ClassDB::bind_method(D_METHOD("get_num_points"), &TrailEmitter::get_num_points);
	ClassDB::bind_method(D_METHOD("set_noise_scale", "value"), &TrailEmitter::set_noise_scale);
	ClassDB::bind_method(D_METHOD("get_noise_scale"), &TrailEmitter::get_noise_scale);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &TrailEmitter::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &TrailEmitter::get_size);
	ClassDB::bind_method(D_METHOD("set_uv_shift", "value"), &TrailEmitter::set_uv_shift);
	ClassDB::bind_method(D_METHOD("get_uv_shift"), &TrailEmitter::get_uv_shift);
	ClassDB::bind_method(D_METHOD("offset_mesh_points", "amount"), &TrailEmitter::offset_mesh_points);
	ClassDB::bind_method(D_METHOD("get_billboard"), &TrailEmitter::get_billboard);
	ClassDB::bind_method(D_METHOD("set_billboard", "value"), &TrailEmitter::set_billboard);
	ClassDB::bind_method(D_METHOD("get_emitting"), &TrailEmitter::get_emitting);
	ClassDB::bind_method(D_METHOD("set_emitting", "value"), &TrailEmitter::set_emitting);

	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_curve", "get_curve");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::OBJECT, "gradient", PROPERTY_HINT_RESOURCE_TYPE, "Gradient"), "set_gradient", "get_gradient");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::NODE_PATH, "geometry_nodepath"), "set_geometry_root", "get_geometry_root");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::INT, "num_points"), "set_num_points", "get_num_points");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::FLOAT, "noise_scale"), "set_noise_scale", "get_noise_scale");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::FLOAT, "size"), "set_size", "get_size");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::FLOAT, "uv_shift"), "set_uv_shift", "get_uv_shift");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::BOOL, "billboard"), "set_billboard", "get_billboard");
	ClassDB::add_property("TrailEmitter", PropertyInfo(Variant::BOOL, "emitting"), "set_emitting", "get_emitting");
}

TrailEmitter::TrailEmitter() {
	num_points = 200;
	size = 1.0;
	uv_shift = 0.0;
	noise_scale = 0.0;
	trail_mesh = nullptr;
}

TrailEmitter::~TrailEmitter() {
	if (trail_mesh) {
		trail_mesh->trail_emitter = nullptr;
	}
}

void TrailEmitter::set_geometry_root(NodePath nodepath) {
	geometry_root = nodepath;
}

NodePath TrailEmitter::get_geometry_root() const {
	return geometry_root;
}

void TrailEmitter::set_material(Ref<Material> new_material) {
	material = new_material;
	if (trail_mesh) {
		trail_mesh->set_material_override(material);
	}
}

Ref<Material> TrailEmitter::get_material() const {
	return material;
}

void TrailEmitter::set_curve(Ref<Curve> new_curve) {
	curve = new_curve;
}

Ref<Curve> TrailEmitter::get_curve() const {
	return curve;
}

void TrailEmitter::set_gradient(Ref<Gradient> new_gradient) {
	gradient = new_gradient;
}

Ref<Gradient> TrailEmitter::get_gradient() const {
	return gradient;
}

void TrailEmitter::set_num_points(int value) {
	num_points = value;
	if (trail_mesh) {
		trail_mesh->set_num_points(num_points);
	}
}

int TrailEmitter::get_num_points() const {
	return num_points;
}

void TrailEmitter::set_noise_scale(float value) {
	noise_scale = value;
	if (trail_mesh) {
		trail_mesh->noise_scale = noise_scale;
	}
}

float TrailEmitter::get_noise_scale() const {
	return noise_scale;
}

void TrailEmitter::set_size(float value) {
	size = value;
	if (trail_mesh && emitting) {
		trail_mesh->size = value;
	}
}

float TrailEmitter::get_size() const {
	return size;
}

void TrailEmitter::set_uv_shift(double value) {
	uv_shift = value;
	if (trail_mesh) {
		trail_mesh->uv_shift = uv_shift;
	}
}

double TrailEmitter::get_uv_shift() const {
	return uv_shift;
}

void TrailEmitter::set_billboard(bool value) {
	billboard = value;
	if (trail_mesh) {
		trail_mesh->billboard = billboard;
	}
}

bool TrailEmitter::get_billboard() const {
	return billboard;
}

void TrailEmitter::set_emitting(bool value) {
	emitting = value;
	if (trail_mesh) {
		trail_mesh->emitting = value;
	}
}

bool TrailEmitter::get_emitting() const {
	return emitting;
}

void TrailEmitter::_ready() {
	Node *geometry_root_node = get_node_or_null(geometry_root);
	if (!geometry_root_node) {
		geometry_root_node = this;
	}

	trail_mesh = memnew(TrailMesh);
	trail_mesh->num_points = num_points;
	trail_mesh->trail_emitter = this;
	trail_mesh->size = size;
	trail_mesh->noise_scale = noise_scale;
	trail_mesh->uv_shift = uv_shift;
	trail_mesh->billboard = billboard;
	trail_mesh->emitting = emitting;
	trail_mesh->set_material_override(material);

	if (curve.is_valid()) {
		curve->bake();
		trail_mesh->set_curve(curve);
	}

	if (gradient.is_valid()) {
		trail_mesh->set_gradient(gradient);
	}

	trail_mesh->set_as_top_level(true);
	geometry_root_node->call_deferred("add_child", trail_mesh);
}

void TrailEmitter::offset_mesh_points(Vector3 offset) {
	trail_mesh->offset_mesh_points(offset);
}
