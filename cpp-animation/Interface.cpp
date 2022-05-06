#include "Interface.h"

Interface::Interface(Settings* settings, CanvasManager* manager) : m_pSettings(settings), m_pManager(manager)
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

void Interface::renderTool()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints({240, 240}, {400, 400});
	ImGui::Begin("Tool");
	ImGui::ColorPicker4("Color", m_pSettings->color, ImGuiColorEditFlags_AlphaBar);

	if (ImGui::RadioButton("Brush", m_pSettings->tool == DrawTool::brush))
		m_pSettings->tool = DrawTool::brush;

	ImGui::SameLine();
	if (ImGui::RadioButton("Eraser", m_pSettings->tool == DrawTool::eraser))
		m_pSettings->tool = DrawTool::eraser;

	ImGui::End();
}

void Interface::renderFrames()
{
	Animation* animation = m_pManager->getAnimation();
	int currentFrame = animation->getCurrentFrame() + 1;
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	ImGui::Begin("Frames");
	ImGui::BeginDisabled(m_pSettings->isAnimating);
		if (ImGui::SliderInt("Frame", &currentFrame, 1, animation->getFramesCount()))
			animation->setCurrentFrame(currentFrame - 1);
		if (ImGui::Button("Create new frame"))
			animation->newFrame();

		bool previusEnabled = animation->getCurrentFrame() - 1 >= 0;
		bool nextEnabled = animation->getCurrentFrame() + 1 < animation->getFramesCount();
		// next/prev buttons
		ImGui::BeginDisabled(!previusEnabled);
		if (ImGui::Button("<< Prev") && previusEnabled)
			animation->prev();
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(!nextEnabled);
		if (ImGui::Button(">> Next") && nextEnabled)
			animation->next();
		ImGui::EndDisabled();

		ImGui::Checkbox("Enable previous map", &m_pSettings->previousMap);
		ImGui::Checkbox("Enable previous alpha", &m_pSettings->previousAlpha);

		ImGui::SliderInt("Prev alpha", &m_pSettings->alpha, 0, 255);

		if (ImGui::Button("Delete frame") && animation->getCurrentFrame() - 1 > 0)
			animation->deleteFrame();

	ImGui::EndDisabled();
	ImGui::End();
}

void Interface::renderAnimation()
{
	Animation* animation = m_pManager->getAnimation();

	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Animation");

	if (animation->getFramesCount() == 1)
		ImGui::Text("Can't animate only with 1 frame");

	ImGui::BeginDisabled(animation->getFramesCount() == 1);

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

	Animation* animation = m_pManager->getAnimation();

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
						m_pManager->reload(size, size);
					}
				}
				ImGui::EndMenu();
			}
			
			if (ImGui::MenuItem("Save"))
			{
				ImGui::SetNextWindowSize({ 400, 200 });
				ImGuiFileDialog::Instance()->OpenModal("SaveDialog", "Choose where to save image", ".png", ".");
			}
			if (ImGui::MenuItem("Load"))
			{
				ImGui::SetNextWindowSize({ 400, 200 });
				ImGuiFileDialog::Instance()->OpenModal("LoadDialog", "Choose what image to load", ".png", ".");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGuiFileDialog::Instance()->Display("SaveDialog"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
			animation->save(ImGuiFileDialog::Instance()->GetFilePathName(), m_pSaver);
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("LoadDialog"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_pManager->getRenderer()->reload(animation);  // to remove offset
			animation->load(ImGuiFileDialog::Instance()->GetFilePathName(), m_pSaver);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void Interface::render()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// start
	renderMenu();
	renderTool();
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
