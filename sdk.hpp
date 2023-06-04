#pragma once

namespace sdk
{
	struct vec3_t
	{
		float x, y, z;

		vec3_t operator+(vec3_t v) { return { x + v.x, y + v.y, z + v.z }; }
		vec3_t operator-(vec3_t v) { return { x - v.x, y - v.y, z - v.z }; }

	};

	struct vec2_t
	{
		float x, y;

		vec2_t operator+(vec2_t v) { return { x + v.x, y + v.y }; }
		vec2_t operator-(vec2_t v) { return { x - v.x, y - v.y }; }
	};

	struct color_t
	{
		int r, g, b, a;
	};

	struct matrix_t 
	{
		float matrix[4][4];

		float* operator[](const int& i) { return matrix[i]; }
	};

	class c_class
	{
	public:

		std::uintptr_t get_address()
		{
			return reinterpret_cast<std::uintptr_t>(this);
		}
		bool is_valid()
		{
			return mem.is_valid(get_address());
		}

	};

	class c_player : public c_class
	{
	public:

		int team()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iTeamNum);
		}

		int health()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iHealth);
		}

		vec3_t origin()
		{
			return mem.read<vec3_t>(get_address() + hazedumper::netvars::m_vecOrigin);
		}

		int life_state()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_lifeState);
		}

		int glow_index()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iGlowIndex);
		}

		vec3_t velocity()
		{
			return mem.read<vec3_t>(get_address() + hazedumper::netvars::m_vecVelocity);
		}

		static c_player* find(int idx)
		{
			return mem.read<c_player*>(g_client_base + hazedumper::signatures::dwEntityList + (idx * 0x10));
		}
	};


	struct glow_entry_t
	{
		glow_entry_t(color_t col, bool _occluded, bool _unoccluded, bool _bloom)
		{
			memset(this, 0, sizeof(glow_entry_t));
			r = col.r;
			g = col.g;
			b = col.b;
			a = col.a;
			occluded = _occluded;
			unoccluded = _unoccluded;
			bloom = _bloom;
		}
		float r, g, b, a;
		char padding_0x8[0x8];
		float unknown;
		char padding_0x4[0x4];
		bool occluded;
		bool unoccluded;
		bool bloom;
	};
	class c_glow_manager : public c_class
	{
	public:
		void update_index(int glow_index, glow_entry_t entry)
		{
			mem.write<glow_entry_t>(get_address() + 0x4 + (glow_index * 0x38), entry);
		}
	};

	c_player* get_local_player()
	{
		return mem.read<c_player*>(g_client_base + hazedumper::signatures::dwLocalPlayer);
	}

	matrix_t get_view_matrix()
	{
		return mem.read<matrix_t>(g_client_base + hazedumper::signatures::dwViewMatrix);
	}

	static c_glow_manager* get_glow_manager()
	{
		return mem.read<c_glow_manager*>(g_client_base + hazedumper::signatures::dwGlowObjectManager);
	}

	vec2_t get_screen_size()
	{
		RECT rect;
		GetWindowRect(FindWindow("Valve001", 0), &rect);

		return 
		{ 
			float(rect.right - rect.left), 
			float(rect.bottom - rect.top) 
		};
	}

	vec2_t w2s(const vec3_t& pos)
	{
		auto matrix = get_view_matrix();
		auto screen_size = get_screen_size();
		auto result = vec2_t();

		float z = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

		if (z <= 0.01f)
			return result;

		result.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
		result.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];
		result.x *= 1.f / z;
		result.y *= 1.f / z;

		float x = screen_size.x * .5f;
		float y = screen_size.y * .5f;

		x += 0.5f * result.x * screen_size.x + 0.5f;
		y -= 0.5f * result.y * screen_size.y + 0.5f;
 

		return { x, y };
	}
}