#include "stdafxGameEngine.h"

#include <ElkTools/Debug/Assertion.h>

#include "ElkGameEngine/Objects/Components/Behaviours/ModelRenderer.h"

using namespace ElkGameEngine::Objects::Components::Behaviours;

ModelRenderer::ModelRenderer()
{
	SetName("ModelRenderer");
}

ElkRendering::Resources::Model& ModelRenderer::GetModel()
{
	ElkTools::Debug::Assert(!HasModel(), "[Cannot dereference a nullptr] You tried to get a model (Model&) that has never been set (nullptr)", __FILE_INFO__);

	return *m_model;
}

ElkRendering::Data::Material & ModelRenderer::GetMaterial()
{
	return m_material;
}

void ModelRenderer::SetModel(ElkRendering::Resources::Model * p_newModel)
{
	m_model = p_newModel;
}

bool ModelRenderer::HasModel() const
{
	return m_model != nullptr;
}
