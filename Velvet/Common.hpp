#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

// Only initialize value on host. 
// Since CUDA doesn't allow dynamics initialization, 
// we use this macro to ignore initialization when compiling with NVCC.
#ifdef __CUDA_ARCH__
	#define HOST_INIT(val) 
#else
	#define HOST_INIT(val) = val
#endif

struct VtSimParams
{
	int numSubsteps			HOST_INIT(5);
	int numIterations		HOST_INIT(5);						//!< Number of solver iterations to perform per-substep
	int maxNumNeighbors		HOST_INIT(64);

	glm::vec3 gravity		HOST_INIT(glm::vec3(0, -9.8f, 0));	//!< Constant acceleration applied to all particles
	float bendCompliance	HOST_INIT(10.0f);
	float damping			HOST_INIT(0.25f);					//!< Viscous drag force, applies a force proportional, and opposite to the particle velocity
	float friction			HOST_INIT(0.1f);					//!< Coefficient of friction used when colliding against shapes
	float collisionMargin	HOST_INIT(0.06f);					//!< Distance particles maintain against shapes, note that for robust collision against triangle meshes this distance should be greater than zero

	unsigned int numParticles;									//!< Total number of particles 
	float particleDiameter;										//!< The maximum interaction radius for particles

	// === Future ===
	float maxSpeed;												//!< The magnitude of particle velocity will be clamped to this value at the end of each step
	//float wind[3];											//!< Constant acceleration applied to particles that belong to dynamic triangles, drag needs to be > 0 for wind to affect triangles
	int relaxationMode;											//!< How the relaxation is applied inside the solver
	float relaxationFactor;										//!< Control the convergence rate of the parallel solver, default: 1, values greater than 1 may lead to instability

	void OnGUI()
	{
		IMGUI_LEFT_LABEL(ImGui::SliderInt, "Num Substeps", &numSubsteps, 1, 20);
		IMGUI_LEFT_LABEL(ImGui::SliderInt, "Num Iterations", &numIterations, 1, 20);
		ImGui::Separator();
		IMGUI_LEFT_LABEL(ImGui::SliderFloat3, "Gravity", (float*)&gravity, -50, 50);
		IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Friction", &friction, 0, 1);
		IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Damping", &damping, 0, 1);
		IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Collision Margin", &collisionMargin, 0, 0.5);
		ImGui::Separator();
		IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Bend Compliance", &bendCompliance, 0, 100.0, "%.3f", ImGuiSliderFlags_Logarithmic);
	}
};

struct VtGameState
{
	bool step = false;
	bool pause = false;
	bool renderWireframe = false;
	bool drawParticles = false;
	bool hideGUI = false;
};

struct VtTimers
{
	float predict;				//!< Time spent in prediction
	float createCellIndices;	//!< Time spent creating grid indices
	float sortCellIndices;		//!< Time spent sorting grid indices
	float createGrid;			//!< Time spent creating grid
	float reorder;				//!< Time spent reordering particles
	float collideParticles;		//!< Time spent finding particle neighbors
	float collideTriangles;		//!< Time spent colliding triangle shapes
	float collideFields;		//!< Time spent colliding signed distance field shapes
	float solveSprings;			//!< Time spent solving distance constraints
	float solveContacts;		//!< Time spent solving contact constraints
	float applyDeltas;	        //!< Time spent adding position deltas to particles
	float updateTriangles;		//!< Time spent updating dynamic triangles
	float updateNormals;		//!< Time spent updating vertex normals
	float finalize;				//!< Time spent finalizing state
	float updateBounds;			//!< Time spent updating particle bounds
	float total;				//!< Sum of all timers above
};
