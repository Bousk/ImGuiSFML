#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/OpenGL.hpp>

#include <imgui.h>

#include <chrono>

namespace sf {
	class Event;
	class RenderTarget;
}

class ImGuiSFML
{
public:
	// Initialize ImGui with the existing window
	void Init(const sf::RenderTarget& target);
	// Call this at the beginning of each frame to reset ImGui state
	void OnFrameBegin();
	// Forward SFML events to ImGui
	void OnEvent(const sf::Event& evt);
	// Call to start a new ImGui frame
	void Update(std::chrono::milliseconds dt);
	// Render ImGui commands submitted
	void Draw(sf::RenderTarget& target) const;
	// Destroy ImGui context
	void Release();

	static GLuint ImTextureToGLHandle(ImTextureID textureID);
	static ImTextureID GLHandleToImTexture(GLuint handle);

private:
	sf::Texture mFontTexture;
};
