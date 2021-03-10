#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GUI\Playground.h"
#include <windows.h>
#include <vector>
#include <memory>

using PlaygroundPtr = std::shared_ptr<Playground>;
using UnsignedInts  = std::vector<size_t>;
using Floats        = std::vector<float>;
using Bools         = std::vector<bool>;

class GameObject
{
 public:
    enum class ObjectType : unsigned {
        PLAYER,
        ENEMY,
        RESOURCE,
        BARRIER
    };

    enum class ObjectAction : unsigned {
        NOTHING,
        PRESSED_C,
        PRESSED_E
    };

    GameObject() = default;

    virtual ~GameObject() = default;

    void InitializeElementGUI() noexcept;
    void TakeSizeLocationArea() noexcept;
    
    RECT GetCollider() const noexcept;
    Bools GetIsAlives() const noexcept;
    Bools GetIsActives() const noexcept;
    Bools GetIsVisibles() const noexcept;
    RECT GetLocationArea() const noexcept;
    Floats GetRespawnTimes() const noexcept;
    ElementGUI GetElementGUI() const noexcept;
    UnsignedInts GetNewCoordsX() const noexcept;
    UnsignedInts GetNewCoordsY() const noexcept;

    void SetNewCoords() noexcept;
    void SetCoord(size_t _ElementNo) noexcept;
    void SetIsAlive(bool _IsAlive, size_t _ElementNo) noexcept;
    void SetRespawnTime(float _Time, size_t _ElementNo) noexcept;
    void SetIsVisible(bool _IsVisible, size_t _ElementNo) noexcept;

    virtual ObjectAction Action() = 0;
    virtual ObjectType GetType() const = 0;
    virtual void SetIsActive(bool _IsActive, size_t _ElementNo) = 0;
    virtual float GetRespawnTime(const std::string) const noexcept = 0;

 protected:
    Floats m_RespawnTimes;
    UnsignedInts m_NewCoordsX;
    UnsignedInts m_NewCoordsY;
    Bools m_IsVisibles;
    Bools m_IsActives;
    Bools m_IsAlives;
    RECT m_Collider;
    RECT m_LocationArea;
    ElementGUI m_ElementGUI;
    PlaygroundPtr m_PlaygroundPtr;
};

#endif  // GAMEOBJECT_H
