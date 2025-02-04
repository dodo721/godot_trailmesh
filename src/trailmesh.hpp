#ifndef TRAILMESH_H
#define TRAILMESH_H

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/variant/packed_color_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

#include <trailemitter.hpp>
#include <trailpoint.hpp>

namespace godot {

class TrailMesh : public MeshInstance3D {
	GDCLASS(TrailMesh, MeshInstance3D)

	friend class TrailEmitter;

private:
	int num_points;
	//double elapsed;
	double total_elapsed;
	//double update_interval;
	double uv_shift;
	int fade_frame_count;
	float size;
	float noise_scale;
	bool billboard;
	bool emitting;
	bool emitted_last_frame;

	Array geometry;
	TrailEmitter *trail_emitter;
	Transform3D emitter_transform;
	Vector3 direction_vector;

	PackedVector3Array vertex_buffer;
	PackedVector3Array normal_buffer;
	PackedFloat32Array tangent_buffer;
	PackedVector2Array uv_buffer;
	PackedColorArray color_buffer;

	TrailPoint *trail_points;

	Ref<Curve> curve;
	Ref<Gradient> gradient;

	void update_transform();

protected:
	static void _bind_methods();
	void initialize_arrays();
	void clear_arrays();
	void offset_mesh_points(Vector3 offset);

public:
	TrailMesh();
	~TrailMesh();

	void _ready() override;
	void _process(double delta) override;
	void _init_trail();

	void update_mesh(float num_vertices);

	int get_num_points() const;
	void set_num_points(int value);

	Ref<Curve> get_curve() const;
	void set_curve(const Ref<Curve> new_curve);

	Ref<Gradient> get_gradient() const;
	void set_gradient(const Ref<Gradient> new_gradient);
};
} //namespace godot

#endif
