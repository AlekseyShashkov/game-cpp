#ifndef DEVILS_H
#define DEVILS_H

#include "GameObject.h"

class Devils : public GameObject
{
 public:
     Devils(const wchar_t *_NameXML);

    ~Devils() noexcept override final;

    ObjectAction Action() override final;

    ObjectType GetType() const noexcept override final;
    float GetRespawnTime(
        const std::string _ElementName) const noexcept override final;

    void SetIsActive(bool _IsActive, size_t _ElementNo) noexcept override final;

 private:

};

#endif	// DEVILS_H
