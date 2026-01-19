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
			Entity entity{ entityId, mContext.get() };
			DrawEntityNode(entity);
		});
		ImGui::End();
	}
	void SceneHierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = ((mSelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			mSelectedContext = entity;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
}