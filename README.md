# ImGuiSFML
A simple way to use ImGui with SFML

	#include <ImGuiSFML/SFML.h>

	#include <SFML/Graphics.hpp>
	#include <SFML/Window.hpp>

	#include <chrono>

	int main(int argc, char* argv[])
	{
		sf::RenderWindow sfWindow(sf::VideoMode(1024, 780), "Demo");

		// Create and initialize ImGui SFML
		Bousk::Core::ImGui::SFML mImguiHandler;
		mImguiHandler.Init(sfWindow);
		
		sf::Clock frameClock;
		while (sfWindow.isOpen())
		{
			// Start new frame
			mImguiHandler.OnFrameBegin();
			
			// Process SFML events
			sf::Event event;
			while (sfWindow.pollEvent(event))
			{
				// Forward events to ImGui
				mImguiHandler.OnEvent(event);
				// Close window: exit
				if (event.type == sf::Event::Closed)
					sfWindow.close();
			}
			using namespace std::chrono_literals;
			const sf::Time dt = frameClock.restart();
			std::chrono::milliseconds dtMs = dt.asMilliseconds() * 1ms;
			mImguiHandler.Update(dtMs);

			sfWindow.clear();

			// Render ImGui
			mImguiHandler.Draw(sfWindow);

			sfWindow.display();

			// Remove this when you have some actual code running and frame cannot be 0ms duration anymore
			std::this_thread::sleep_for(1ms);
		}

		return 0;
	}