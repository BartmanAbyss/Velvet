#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include "MeshRenderer.hpp"

namespace Velvet
{
	using namespace std;

	class Actor
	{
	public:
		Actor();

		Actor(string _name);

		static shared_ptr<Actor> PrefabTriangle();

		static shared_ptr<Actor> PrefabCube();

		static shared_ptr<Actor> PrefabQuad();

		static shared_ptr<Actor> PrefabCamera();

		static shared_ptr<Actor> PrefabLight();

		void Start();

		void Update();

		void OnDestroy();

		void AddComponent(shared_ptr<Component> component);

		template <typename T>
		enable_if_t<is_base_of<Component, T>::value, T*> GetComponent()
		{
			T* result = nullptr;
			for (auto c : components)
			{
				result = dynamic_cast<T*>(c.get());
				if (result)
					return result;
			}
			return result;
		}

		shared_ptr<Transform> transform = make_shared<Transform>(Transform(this));
		vector<shared_ptr<Component>> components;

		string name;

	};

}