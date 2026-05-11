# C++ Coding Standards

This document defines the coding standards and style rules used in the `opengldemo` project. Please adhere to these rules to maintain code consistency.

## General Rules

- **C++ Standard:** C++23.
- **Indentation:** 2 spaces, no tabs.
- **Encoding:** UTF-8.
- **Header Guards:** Use `#pragma once`.

## Types and Data Structures

### 1. Fixed-Width Integers
Always prefer fixed-width integers from `<cstdint>` to ensure cross-platform consistency. Avoid using plain `int`, `long`, or `short` unless required by an external API.
```cpp
uint32_t count = 0;
int64_t large_value = 0;
uint8_t byte_data = 0xFF;
```

### 2. Variadic-like Arguments (Collections)
For functions that take a variable number of arguments (similar to Go's `...type` syntax), prefer using `std::vector` or `std::span` over `std::initializer_list`.
```cpp
// Preferred
void processItems(const std::vector<std::string>& items);

// Usage
processItems({"item1", "item2", "item3"});
```

## Language and Documentation

### 1. English Only
All code elements, including variable names, function names, and class names, must be in English.
```cpp
// Bad
uint32_t sayac = 0;
// Good
uint32_t counter = 0;
```

### 2. Comments
All comments must be written in English. Use comments to explain the "why" rather than the "how" whenever possible.
```cpp
// Bad: Shader'i yeniden yukle
// Good: Reload the shader when the source file changes
void reloadShader();
```

## Naming Conventions

### 1. Classes and Structs (PascalCase)
Class and struct names start with an uppercase letter, and each new word starts with an uppercase letter.
```cpp
class AssetManager;
struct ShaderInfo;
```

### 2. Functions (camelCase)
Function and method names start with a lowercase letter, and subsequent words start with an uppercase letter.
```cpp
void update();
void reloadShader();
uint32_t getID() const;
```

### 3. Variables and Parameters (snake_case)
Local variables and function parameters are written in lowercase with words separated by underscores.
```cpp
int frame_count = 0;
void setPosition(float x_pos, float y_pos);
```

### 4. Class Member Variables (m_snake_case)
Private or protected member variables of a class must be prefixed with `m_`.
```cpp
class Shader {
private:
    uint32_t m_id;
    ShaderType m_type;
};
```

### 5. Constants and Enum Values (PascalCase)
Enum classes and constant values should use PascalCase.
```cpp
enum class ShaderType {
    Vertex,
    Fragment
};
```

## Memory Management

- Use `std::unique_ptr` and `std::shared_ptr` whenever possible.
- Use raw pointers (`*`) only for non-owning observers.
- Avoid `new` and `delete`; use `std::make_unique` or `std::make_shared` instead.

## Modern C++ Best Practices

- Use `auto` when the type is obvious (e.g., iterators, casts).
- Use C++ style casts (`static_cast`, `reinterpret_cast`) instead of C-style casts.
- Mark functions that do not modify state as `const`.
- Use `[[nodiscard]]` for functions where the return value should not be ignored.
