#ifndef BADROCKHORIZONTAL_H
#define BADROCKHORIZONTAL_H

#include "GameObject.h"

class BadRockHorizontal : public GameObject
{
 public:
    BadRockHorizontal(const wchar_t *_NameXML);

    ~BadRockHorizontal() noexcept override final;

    ObjectAction Action() override final;

    ObjectType GetType() const noexcept override final;
    float GetRespawnTime(
        const std::string _ElementName) const noexcept override final;

    void SetIsActive(bool _IsActive, size_t _ElementNo) noexcept override final;

 private:

};

#endif  // BADROCKHORIZONTAL_H
