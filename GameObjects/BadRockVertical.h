#ifndef BADROCKVERTICAL_H
#define BADROCKVERTICAL_H

#include "GameObject.h"

class BadRockVertical : public GameObject
{
 public:
    BadRockVertical(const wchar_t *_NameXML);

    ~BadRockVertical() noexcept override final;

    ObjectAction Action() override final;

    ObjectType GetType() const noexcept override final;
    float GetRespawnTime(
        const std::string _ElementName) const noexcept override final;

    void SetIsActive(bool _IsActive, size_t _ElementNo) noexcept override final;

 private:

};

#endif	// BADROCKVERTICAL_H
