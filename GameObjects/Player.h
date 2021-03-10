#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
 public:
    Player(const wchar_t *_NameXML);

    ~Player() noexcept override final;
    
    ObjectAction Action() noexcept override final;

    ObjectType GetType() const noexcept override final;
    float GetRespawnTime(
        const std::string _ElementName) const noexcept override final;

    void SetIsActive(bool _IsActive, size_t _ElementNo) noexcept override final;

 private:
    unsigned short m_Speed;
};

#endif  // PLAYER_H
