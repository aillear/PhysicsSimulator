# UI Part

## **`UIComponent`** (Base UI Class)

Inherits from `Object`, serves as the foundation for UI elements with core functionality:

**1. Coordinate System**

​	All the position referred in UI family are relative to *SCREEN COORDINATION* !

- Relative Positioning `topLeftPos` to define position relative to parent's top-left corner
- Absolute Positioning `GetScreenPos()` recursively calculates screen coordinates by summing ancestor positions



**2. Lifecycle Management**

- `Init()`     → Initializes when first added to `UIMgr `(`Init()` will be called in next frame before `Update()`.)
- `Update()`  → Performs frame updates and it's children's `Update()`method recursively.
- `Render()`  → Handles visual rendering
- `Destroy()` → Cleans up resources upon removal from `UIMgr`(`Destroy()`will be called in next frame before `Update()`.)



**3. Layout System**

- Supports X/Y axis alignment modes (start/center/end)

- Support margin to it's parent's boundary(padding doesn't work in center alignment mode).

- Support extra offset in both x and y directions.

- Example Layout (X-center+ Y-start):

  - margin x = 10(useless), margin y = 10; (margin was incorrectly marked as padding)

  - offset x = 10, offset y = 10;

    ![image-20250405215359629](https://aillear-picbed.oss-cn-fuzhou.aliyuncs.com/image-20250405215359629.png)


**4. Core Methods**

```C++
glm::vec2 GetScreenPos() const;

void OnMouseMove(SDL_Event &event) override { ; }
void OnMouseDown(SDL_Event &event) override { ; }
void OnMouseUp(SDL_Event &event) override { ; }

void Render() override = 0;
void Update(float dt) override = 0;
void HandleEvent(SDL_Event &event) override;
```















