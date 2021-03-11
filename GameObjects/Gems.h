#ifndef GEMS_H
#define GEMS_H

#include "GameObject.h"

class Gems : public GameObject
{
 public:
    Gems(const wchar_t *_NameXML);

    ~Gems() noexcept override final;

    ObjectAction Action() noexcept override final;

    ObjectType GetType() const noexcept override final;
    float GetRespawnTime(
        const std::string _ElementName) const noexcept override final;

    void SetIsActive(bool _IsActive, size_t _ElementNo) noexcept override final;

 private:

};

#endif  // GEMS_H
