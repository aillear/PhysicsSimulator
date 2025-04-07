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

  Controlled via the Boolean `enable` field:

  - `true`: Enables `Update`, `Render`, `PhysicsUpdate` (including children’s methods)
  - `false`: Disables the above methods



## `RenderSystem`

`RenderSystem` is a singleton.

 

# UI Part

## **`UIComponent`** (Base UI Class)

Inherits from `Object`, serves as the foundation for UI elements with core functionality:

### **1. Coordinate System**

All the position referred in UI family are relative to *SCREEN COORDINATION* !

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

  - margin x = 10(useless), margin y = 10; (margin was incorrectly marked as padding in the illustration below)

  - offset x = 10, offset y = 10;

    ![image-20250405215359629](https://aillear-picbed.oss-cn-fuzhou.aliyuncs.com/image-20250405215359629.png)

## **`UIMgr`** (singleton class to manage all UI elements)

The `UIMgr` serves as the centralized controller for all `UIComponent` instances, enforcing lifecycle management and hierarchical organization of UI elements.

### **1. `UIComponent `Management**

- **Registration Requirement**
  A `UIComponent` ​**must**​ be registered with `UIMgr` to become functional.
- **Core Methods**
  Use the following to modify the component hierarchy:
  - `AddUIComponent()`: Registers a component
  - `RemoveUIComponent()`: Marks a component for removal
- **Parent-Child Relationships**
  A component’s parent can be designated, but the parent ​**must**​ already be registered and managed by `UIMgr`.
  ​**Note**: Hierarchy changes take effect in the ​**next frame**.

### **2. Lifecycle Execution Flow**

The manager processes components in a strict per-frame sequence:

1. **Initialization Phase**
   - Processes components added in the **previous frame**
   - Invokes their `Init()` methods
2. **Destruction Phase**
   - Removes components marked for deletion
   - Triggers their `Destroy()` methods
3. **Update-Render Phase**
   - Executes `Update(float)` and `Render()` for all **active components**





