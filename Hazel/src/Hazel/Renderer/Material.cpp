#include "hzpch.h"

#include "Material.h"


namespace Hazel {

    Ref<Material> Material::Create(Ref<Shader>)
    {
        return Ref<Material>();
    }

    Ref<MaterialInstance> MaterialInstance::Create(Ref<Material>)
    {
        return Ref<MaterialInstance>();
    }

    void MaterialInstance::SetFlag(MaterialFlag flag, bool value)
    {
    }

    void MaterialInstance::SetFlag(const std::string& name, const Ref<TextureCube>& skybox)
    {
    }

    void MaterialInstance::Set(const std::string& name, float value)
    {

    }

    void MaterialInstance::Set(const std::string& name, glm::mat4 value)
    {
    }

    void MaterialInstance::Set(const std::string& name, glm::vec3 value)
    {
    }

    void MaterialInstance::Set(const std::string& name, Ref<TextureCube> textureCube)
    {
    }

    Ref<Shader> MaterialInstance::GetShader()
    {
        return Ref<Shader>();
    }

}
