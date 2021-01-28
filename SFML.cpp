#include <SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <backends/imgui_impl_opengl2.h>
#include <imgui.h>

GLuint ImGuiSFML::ImTextureToGLHandle(ImTextureID textureID)
{
	GLuint glTextureHandle;
	std::memcpy(&glTextureHandle, &textureID, sizeof(GLuint));
	return glTextureHandle;
};
ImTextureID ImGuiSFML::GLHandleToImTexture(GLuint handle)
{
	ImTextureID imTexId;
	std::memcpy(&imTexId, &handle, sizeof(ImTextureID));
	return imTexId;
};
void ImGuiSFML::DisplayTexture(const sf::Texture& texture)
{
	DisplayTexture(texture, sf::Rect<unsigned int>(sf::Vector2u(0, 0), texture.getSize()));
}
void ImGuiSFML::DisplayTexture(const sf::Texture& texture, const sf::Rect<unsigned int>& texturePart)
{
	DisplayTexture(texture, texturePart, sf::Vector2u(texturePart.width, texturePart.height));
}
void ImGuiSFML::DisplayTexture(const sf::Texture& texture, const sf::Rect<unsigned int>& texturePart, const sf::Vector2u& renderSize)
{
	const GLuint handle = texture.getNativeHandle();
	if (handle)
	{
		const ImTextureID imId = GLHandleToImTexture(handle);
		const ImVec2 size(static_cast<float>(renderSize.x), static_cast<float>(renderSize.y));
		const ImVec2 uv0(texturePart.left * 1.f / texture.getSize().x, texturePart.top * 1.f / texture.getSize().y);
		const ImVec2 uv1((texturePart.left + texturePart.width) * 1.f / texture.getSize().x, (texturePart.top + texturePart.height) * 1.f / texture.getSize().y);
		ImGui::Image(imId, size, uv0, uv1, ImVec4(1,1,1,1), ImColor(255,0,0));
	}
}
void ImGuiSFML::Init(const sf::RenderTarget& target)
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	//io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	io.DisplaySize = ImVec2(static_cast<float>(target.getSize().x), static_cast<float>(target.getSize().y));

	unsigned char* pixels;
	int width;
	int height;
	int bpp;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bpp);
	assert(width > 0 && height > 0);
	mFontTexture.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
	mFontTexture.update(pixels);
	io.Fonts->SetTexID(GLHandleToImTexture(mFontTexture.getNativeHandle()));

	// Initialize keys
	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
	io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Enter;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = sf::Keyboard::Enter;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;
}
void ImGuiSFML::OnFrameBegin()
{
	ImGuiIO& io = ImGui::GetIO();
	io.ClearInputCharacters();
	io.MouseWheel = 0;
	io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
	// TODO : Gamepad
}
void ImGuiSFML::OnEvent(const sf::Event& evt)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (evt.type)
	{
		case sf::Event::TextEntered:
		{
			io.AddInputCharacter(evt.text.unicode);
		} break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
		{
			io.KeysDown[evt.key.code] = (evt.type == sf::Event::KeyPressed);
			io.KeyAlt = evt.key.alt;
			io.KeyCtrl = evt.key.control;
			io.KeyShift = evt.key.shift;
			io.KeySuper = evt.key.system;
		} break;
		case sf::Event::MouseWheelScrolled:
		{
			io.MousePos = ImVec2(static_cast<float>(evt.mouseWheelScroll.x), static_cast<float>(evt.mouseWheelScroll.y));
			io.MouseWheel = evt.mouseWheelScroll.delta;
		} break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		{
			io.MousePos = ImVec2(static_cast<float>(evt.mouseButton.x), static_cast<float>(evt.mouseButton.y));
			if (evt.mouseButton.button == sf::Mouse::Left)
				io.MouseDown[0] = (evt.type == sf::Event::MouseButtonPressed);
			if (evt.mouseButton.button == sf::Mouse::Right)
				io.MouseDown[1] = (evt.type == sf::Event::MouseButtonPressed);
			if (evt.mouseButton.button == sf::Mouse::Middle)
				io.MouseDown[2] = (evt.type == sf::Event::MouseButtonPressed);
		} break;
		case sf::Event::MouseMoved:
		{
			io.MousePos = ImVec2(static_cast<float>(evt.mouseMove.x), static_cast<float>(evt.mouseMove.y));
		} break;
		case sf::Event::MouseEntered:
			break;
		case sf::Event::MouseLeft:
			break;
	}
}
void ImGuiSFML::Update(std::chrono::milliseconds dt)
{
	ImGuiIO& io = ImGui::GetIO();

	std::chrono::duration<float> fsec = dt;
	io.DeltaTime = fsec.count();

	ImGui::NewFrame();
}
void ImGuiSFML::Draw(sf::RenderTarget& target) const
{
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(static_cast<float>(target.getSize().x), static_cast<float>(target.getSize().y));
	
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiSFML::Release()
{
	ImGui::DestroyContext();
}