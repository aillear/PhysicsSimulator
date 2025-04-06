# Basic Part

## `object`

The `Object` class serves as the foundational entity in this application, offering three core functionalities:

### 1. Lifecycle Management

- **Base Methods** (protected access)
  Derived classes may override the following lifecycle methods:
  - `Init()`
  - `Render()`
  - `Update(float)`
  - `PhysicsUpdate(float)`
  - `HandleEvent()`
  - `Destroy()`
- **Invocation Mechanism**
  Lifecycle methods are triggered ​**only**​ when the object is managed by a dedicated system (e.g., `UIMgr`). Parent objects recursively invoke lifecycle methods across their children.
- **Extended Callbacks**
  Use `AddxxxCallback(callback)` to attach shared logic (applicable to this class and derived classes) to lifecycle functions. These callbacks execute ​**before**​ the corresponding lifecycle method.

### 2. Hierarchy Management

- **Tree Structure**
  Maintains:
  - A `vector` of child objects
  - A pointer to its parent (`father`)
- **Recursive Propagation**
  Parent objects recursively dispatch lifecycle calls to children. Root nodes are owned and managed by external systems.

### 3. Availability Control

- Toggle Field

  Controlled via the boolean `enable` field:

  - `true`: Enables `Update`, `Render`, `PhysicsUpdate` (including children’s methods)
  - `false`: Disables the above methods



## `RenderSystem`

`RenderSystem` is a singleton.

 

# UI Part

## **`UIComponent`** (Base UI Class)

Inherits from `Object`, serves as the foundation for UI elements with core functionality:

### **1. Coordinate System**

​	All the position referred in UI family are relative to *SCREEN COORDINATION* !

- Relative Positioning `topLeftPos` to define position relative to parent's top-left corner
- Absolute Positioning `GetScreenPos()` recursively calculates screen coordinates by summing ancestor positions



### **2. Lifecycle Management**

Lifecycle is managed by `UIMgr`

- `Init()`     → Initializes when first added to `UIMgr `(`Init()` will be called in next frame before `Update()`.)
- `Update()`  → Performs frame updates and it's children's `Update()`method recursively.
- `Render()`  → Handles visual rendering
- `Destroy()` → Cleans up resources upon removal from `UIMgr`(`Destroy()`will be called in next frame before `Update()`.)



### **3. Layout System**

- Supports X/Y axis alignment modes (start/center/end)

- Support margin to it's parent's boundary(padding doesn't work in center alignment mode).

- Support extra offset in both x and y directions.

- Example Layout (X-center+ Y-start):

  - margin x = 10(useless), margin y = 10; (margin was incorrectly marked as padding)

  - offset x = 10, offset y = 10;

    ![image-20250405215359629](https://aillear-picbed.oss-cn-fuzhou.aliyuncs.com/image-20250405215359629.png)













