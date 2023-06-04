#pragma comment(lib, "overlay.lib")
 
namespace overlay
{
	void create();
	void set_rect(float l, float t, float r, float b);
	void set_visible(bool v);
	void draw_line(float x1, float y1, float x2, float y2, float* col);
	void draw_rect(float x1, float y1, float x2, float y2, float* col);
	void draw_rect_filled(float x1, float y1, float x2, float y2, float* col);
	void clear();
	void render();

	void line(sdk::vec2_t from, sdk::vec2_t to, sdk::color_t col)
	{
		draw_line(from.x, from.y, to.x, to.y, new float[4](col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f));
	}

	void rect(sdk::vec2_t from, sdk::vec2_t to, sdk::color_t col)
	{
		draw_rect(from.x, from.y, to.x, to.y, new float[4](col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f));
	}

	void rect_filled(sdk::vec2_t from, sdk::vec2_t to, sdk::color_t col)
	{
		draw_rect_filled(from.x, from.y, to.x, to.y, new float[4](col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f));
	}

}