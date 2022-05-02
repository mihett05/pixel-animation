#include "Interface.h"

Interface::Interface(Settings* settings, CanvasRenderer* renderer) : m_pSettings(settings), m_pRenderer(renderer)
{
	m_pSaver = new Saver();
}

void Interface::init(SDL_Window* win, SDL_Renderer* renderer, int w, int h)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize.x = w;
	io.DisplaySize.y = h;
	io.WantCaptureMouse = true;
	io.WantCaptureKeyboard = true;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(win, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);
}

void Interface::renderBrush()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Brush");
	ImGui::ColorPicker3("Color", m_pSettings->color);
	ImGui::End();
}

void Interface::renderFrames()
{
	int currentFrame = m_pRenderer->m_currentFrame + 1;
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Frames");
	ImGui::BeginDisabled(m_pSettings->isAnimating);

	if (ImGui::SliderInt("Frame", &currentFrame, 1, m_pRenderer->getFramesCount()))
		m_pRenderer->m_currentFrame = currentFrame - 1;
	if (ImGui::Button("Create new frame"))
		m_pRenderer->newFrame();

	bool previusEnabled = m_pRenderer->m_currentFrame - 1 >= 0;
	bool nextEnabled = m_pRenderer->m_currentFrame + 1 < m_pRenderer->getFramesCount();


	// next/prev buttons
	ImGui::BeginDisabled(!previusEnabled);
	if (ImGui::Button("<< Prev") && previusEnabled)
		--m_pRenderer->m_currentFrame;
	ImGui::EndDisabled();

	ImGui::SameLine();

	ImGui::BeginDisabled(!nextEnabled);
	if (ImGui::Button(">> Next") && nextEnabled)
		++m_pRenderer->m_currentFrame;
	ImGui::EndDisabled();

	ImGui::Checkbox("Enable previous map", &m_pSettings->previousMap);
	ImGui::Checkbox("Enable previous alpha", &m_pSettings->previousAlpha);

	ImGui::SliderInt("Prev alpha", &m_pSettings->alpha, 0, 255);

	ImGui::EndDisabled();
	ImGui::End();
}

void Interface::renderAnimation()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Animation");

	if (m_pRenderer->getFramesCount() == 1)
		ImGui::Text("Can't animate only with 1 frame");

	ImGui::BeginDisabled(m_pRenderer->getFramesCount() == 1);

	ImGui::SliderInt("Delay", &m_pSettings->delay, 1, 10);

	if (ImGui::Button("Start/Stop"))
	{
		m_pSettings->isAnimating = !m_pSettings->isAnimating;
		pushAnimation(m_pSettings);
	}

	ImGui::EndDisabled();

	ImGui::End();
}

void Interface::renderMenu()
{
	static string sizes[] = {"8x8", "16x16", "24x24", "32x32", "48x48", "64x64"};

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("New"))
			{
				for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
				{
					if (ImGui::MenuItem(sizes[i].c_str()))
					{
						int size = stoi(sizes[i].substr(0, sizes[i].length() / 2));
						m_pRenderer->recreate(size, size);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save"))
				m_pRenderer->save(m_pSaver);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Interface::render()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// start
	renderMenu();
	renderBrush();
	renderFrames();
	renderAnimation();
	// end

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void Interface::update(SDL_Event& e)
{
	
	render();
}
