   	  _____                          _____
  	 / ____|                        / ____|_     _
  	| |  __  __ _ _ __ ___   ___   | |   _| |_ _| |_
   	| | |_ |/ _` | '_ ` _ \ / _ \  | |  |_   _|_   _|
    | |__| | (_| | | | | | |  __/  | |____|_|   |_|
     \_____|\__,_|_| |_| |_|\___|   \_____|
![Windows](https://img.shields.io/badge/Windows-003399?style=for-the-badge&logo=windows&logoColor=white)
![Windows](https://img.shields.io/badge/Visual_Studio_2015-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
[![MEGA](https://img.shields.io/badge/Mega%20-%23D90007.svg?&style=for-the-badge&logo=Mega&logoColor=white)](https://mega.nz/folder/VYVlEQIS#vsmpFWdtuSUKq3fnIego9A)
# Game C++
:video_game:Creation of game engine.

## :city_sunset: Скриншот:
![Скриншот](https://user-images.githubusercontent.com/17510024/110648378-b7faab00-81c9-11eb-92b5-2cbbf8e2d9ed.jpg)

## :scroll: Особенности:
- C++11, C++14
- WinApi
- XML DOM
- Создание игрового интерфейса через тайлсеты
- Взаимодействие объектов в реальном времени
## :computer_mouse: Управление:
- Движение `WASD`
- Сбор кристаллов `E`
- Показать коллайдер `C`

## :computer: Игровой процесс:
![Gameplay](https://user-images.githubusercontent.com/17510024/110643895-9bf50a80-81c5-11eb-9c4c-e00696cbae1e.gif)

## :hammer_and_wrench: Игровая логика:
![Вызов](https://github.com/AlekseyShashkov/game-cpp/blob/main/Main.cpp) методов игровой логики и перерисовки происходит по таймеру
```c++
::SetTimer(hWnd, ID_TIMER, 40, nullptr);
```
```c++
case WM_PAINT:
    hDC = ::BeginPaint(hWnd, &ps);
    
    list->UpdateGame();
    display->DisplayGame(hDC);

    ::EndPaint(hWnd, &ps);
    return 0;

case WM_TIMER:
    ::InvalidateRect(hWnd, NULL, FALSE);
    ::UpdateWindow(hWnd);
    return 0;
```
____
![Взаимодействие](https://github.com/AlekseyShashkov/game-cpp/blob/main/GameObjects/ObjectList.cpp) игровых объектов
```c++
void ObjectList::UpdateGame() noexcept
```
- Очищаем игровую область
    ```c++
    m_PlaygroundPtr->CleanScreen();
    ```
- Вначале проверяем, является ли объект (группа элементов, далее - группа) видимым (под видимостью группы подразумеваем видимость квадрата, координаты вершин которого являются предельными значениями координат элементов). Видимость - попадание в область камеры.
    ```c++
    CheckIsVisibleGroup(i);
    ```
- Проверяем видимость отдельных элементов группы
    ```c++
    CheckIsVisibleElements(i);
    ```
- Далее в зависимости от типа группы:
   - `ENEMY`
      - :ghost:Проверяем возможность воскрешения элементов
      ```c++
      CheckRespawn(i, lv_TimeIntervalFromStart);
      ```
      - :crossed_swords:Выполняем действие
      ```c++
      i->Action();
      ```
   - `RESOURCE`
      - :ghost:Проверяем возможность воскрешения элементов
      ```c++
      CheckRespawn(i, lv_TimeIntervalFromStart);
      ```
   - `PLAYER`
      - :crossed_swords:Выполняем действие
      ```c++
      lv_PlayerAction = i->Action();
      ```
      - :o:Создаём активные зоны вокруг игрока
      ```c++
      InitializeRegion(i->GetElementGUI(), 160, lv_RangeRegion);
      InitializeRegion(i->GetElementGUI(), 16, lv_MeleeRegion);
      ```
   - `BARRIER`
- Далее в зависимости от типа группы:
   - `ENEMY`
      - :collision:Проверяем столкновения элементов с элементами других групп
      ```c++
      CheckCollision(i);
      ```
      - :x:Устанавливаем новые координаты
      ```c++
      i->SetNewCoords();
      ```
      - :black_square_button:Устанавливаем область видимости группы
      ```c++
      i->TakeSizeLocationArea();
      ```
      - :dart:Проверяем на попадание в активную зону (изменение статуса на активен)
      ```c++
      CheckRegion(i, lv_RangeRegion);
      ```
   - `RESOURCE`
      - :dart:Проверяем на попадание в активную зону (изменение статуса на активен)
      ```c++
      CheckRegion(i, lv_MeleeRegion);
      ```
      - :hand:Если игрок нажал `E` и элемент активен - собираем его
      ```c++
      if (lv_PlayerAction 
          == GameObject::ObjectAction::PRESSED_E) {
      CollectElement(i, lv_TimeIntervalFromStart);
      }
      ```
   - `PLAYER`
      - :collision:Проверяем столкновения элементов с элементами других групп
      ```c++
      CheckCollision(i);
      ```
      - :x:Устанавливаем новые координаты
      ```c++
      i->SetNewCoords();
      ```
      - :black_square_button:Устанавливаем область видимости группы
      ```c++
      i->TakeSizeLocationArea();
      ```
      - :movie_camera:Изменяем видимую камерой область
      ```c++
      m_PlaygroundPtr->Camera(
          i->GetNewCoordsX()[0], i->GetNewCoordsY()[0]);
      ```
   - `BARRIER`
- Если игрок нажал `C` - разрешаем рисование коллайдеров элементов
    ```c++
    if (lv_PlayerAction == GameObject::ObjectAction::PRESSED_C) {
        if (m_PlaygroundPtr->GetColliderIsVisible()) {
            m_PlaygroundPtr->SetColliderIsVisible(false);
        } else {
            m_PlaygroundPtr->SetColliderIsVisible(true);
        }
    }
    ```
- Рисуем элементы
    ```c++

    m_PlaygroundPtr->DrawElementGUI(i->GetElementGUI(),
                                    i->GetCollider(),
                                    i->GetIsVisibles(),
                                    i->GetIsAlives());
    ```
