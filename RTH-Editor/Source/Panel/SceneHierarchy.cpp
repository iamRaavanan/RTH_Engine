#include "SceneHierarchy.h"
#include "imgui.h"
#include "RTH/Scene/Components.h"
namespace RTH
{
	SceneHierarchy::SceneHierarchy(const Ref<Scene>& worldContext)
	{
	}

	void SceneHierarchy::SetWorldContext(const Ref<Scene>& context)
	{
		mContext = context;
	}

	void SceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		mContext->mRegistry.view<TagComponent>().each([&](entt::entity entityId, TagComponent& tc)
		{
			ImGui::Text("%s", tc.tag.c_str());
		});
		ImGui::End();
	}
}