//Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "ball.h"
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <math.h>

//---------------------------------------------------------------

std::vector<Point> points = {
	{{200, 400}, 10}};
//----------- system dynamic parameters --------------------

// Length of the linkage OA
float a = 1.0;

// Position of fixed joints O and C
float Ox = 0.0;
float Oy = 0.0;
float Cx = 0.0;
float Cy = -2.0 * a;

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>> simulation()
{

    std::vector<float> x;
    std::vector<float> y;
    float dt = M_PI / 360.0;
    for (float t = 0.0; t < 2 * M_PI; t = t + dt)
    {

        float dist = a * std::sqrt(5.0 + 4.0 * std::cos(t));

        float alfa = (float)std::acos(dist * dist + 3.0 * a * a) / (4.0 * a * dist);

        float cos_alfa = (float)(dist * dist + 3.0 * a * a) / (4.0 * a * dist);

        float sin_alfa = ((0 <= t) && (t <= M_PI)) ? std::sqrt(1 - cos_alfa * cos_alfa) : -std::sqrt(1 - cos_alfa * cos_alfa);
        int d = ((0 <= t) && (t <= M_PI)) ? 1 : 0;
        std::cout << "dec : "<< d << "\n";

        float Ax = -a * std::sin(t);
        float Ay = a * std::cos(t);

        float Bx = Cx - 4.0 * a * cos_alfa * sin_alfa;

        float By = Cy + 2.0 * a * (2 * cos_alfa * cos_alfa - 1);

        float Px = 4.0 * Ax - 3.0 * Bx;
        float Py = 4.0 * Ay - 3.0 * By;

        std::cout << Px << " : " << Py << "\n";
        x.push_back(Px);
        y.push_back(Py);
    }
    return std::make_tuple(x, y);
}


int main(int argc, char const *argv[])
{

	int w = 1280;
	int h = 720;
	float scale_x = 50.0;
	float scale_y = 50.0;
	std::string title = "Bouncing ball";
	initImgui(w, h, title);

	// simulation state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	// compute ball path
	auto sim = simulation();

	// Main loop
	int ii = 0;
	bool flag = true;
	std::vector<ImVec2> tail;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		//----------------------------------------

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		ImVec2 ball = {(float)points[0].position.x + std::get<0>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<1>(sim)[ii] * scale_y};

		std::cout << "t= " << ii << " x= " << std::get<0>(sim)[ii] << " y= " << std::get<1>(sim)[ii] << "\n";

		float radius = 5;
		float size_line = 3;

		draw_list->AddCircleFilled(ball, radius, ImColor(ray_color));

		tail.push_back(ball);



		for (auto &ii : tail)
		{

			
			draw_list->AddCircleFilled(ii, 0.5, ImColor(ray_color));
		}

		ii++;
		if (ii >= std::get<0>(sim).size())
		{
			flag = false;
		}

		ImGui::End();
		//----------------------------------------

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
