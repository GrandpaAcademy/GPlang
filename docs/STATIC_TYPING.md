# GPLANG Static Typing + Compile-Time Checks

**Catch errors at compile time, not runtime - for bulletproof software**

## 🔍 **Static Typing System**

GPLANG uses a powerful static type system that catches errors before your code ever runs.

### **🎯 Core Principles**
- **Type safety**: No type errors at runtime
- **Type inference**: Write less, get more safety
- **Zero overhead**: Types are compile-time only
- **Expressive**: Rich type system for complex programs

## 🛡️ **Compile-Time Safety Checks**

### **1. Type Checking**
```gplang
# Type mismatches caught at compile time
var number: i32 = 42
var text: string = "hello"

# ❌ Compiler error: type mismatch
# number = text  # Cannot assign string to i32

# ✅ Correct usage
number = 100    # i32 to i32 - OK
text = "world"  # string to string - OK
```

### **2. Null Safety**
```gplang
# No null pointer exceptions!
var maybe_value: Option<i32> = Some(42)

# ❌ Compiler error: cannot use Option<i32> as i32
# var direct_use = maybe_value + 10

# ✅ Safe pattern matching required
match maybe_value:
    case Some(value):
        var result = value + 10  # Safe to use
    case None:
        print("No value available")
```

### **3. Array Bounds Checking**
```gplang
# Array access is bounds-checked
var array: [i32; 5] = [1, 2, 3, 4, 5]

# ✅ Safe access
var first = array[0]    # OK
var last = array[4]     # OK

# ❌ Compiler error: index out of bounds
# var invalid = array[10]  # Caught at compile time!

# Dynamic bounds checking for runtime indices
var index = get_user_input()
if index < array.len():
    var safe_value = array[index]  # Runtime check required
```

### **4. Memory Safety**
```gplang
# Use-after-free prevention
func unsafe_example():
    var data = alloc(100)
    free(data)
    # ❌ Compiler error: use after free
    # print(data[0])

# Double-free prevention
func safe_example():
    var data = alloc(100)
    free(data)
    # ❌ Compiler error: double free
    # free(data)
```

### **5. Ownership and Borrowing**
```gplang
# Ownership rules enforced at compile time
func ownership_example():
    var data = create_data()
    var moved = move(data)
    
    # ❌ Compiler error: use of moved value
    # process(data)
    
    # ✅ Use the moved value
    process(moved)

# Borrowing rules
func borrowing_example():
    var data = create_data()
    var borrowed = &data
    
    # ❌ Compiler error: cannot move while borrowed
    # var moved = move(data)
    
    use_borrowed(borrowed)
    # Now safe to move
    var moved = move(data)
```

## 🧠 **Type Inference**

GPLANG infers types automatically while maintaining safety:

```gplang
# Type inference in action
var number = 42        # Inferred as i32
var pi = 3.14159      # Inferred as f64
var text = "hello"    # Inferred as string
var flag = true       # Inferred as bool

# Complex type inference
var numbers = [1, 2, 3, 4, 5]  # Inferred as [i32; 5]
var pairs = [(1, "a"), (2, "b")]  # Inferred as [(i32, string); 2]

# Function return type inference
func calculate():  # Return type inferred as i32
    return 42 * 2

# Generic type inference
var max_value = max(10, 20)      # max<i32> inferred
var max_float = max(3.14, 2.71)  # max<f64> inferred
```

## 🔧 **Advanced Type Features**

### **1. Generic Types**
```gplang
# Generic data structures
struct Vec<T>:
    data: *mut T
    length: usize
    capacity: usize

# Generic functions with constraints
func sort<T>(array: &mut [T]) where T: Ord:
    // Sorting implementation

# Usage with type inference
var numbers = Vec.new()  # Vec<i32> inferred from usage
numbers.push(1)
numbers.push(2)
```

### **2. Algebraic Data Types**
```gplang
# Sum types (enums)
enum Result<T, E>:
    Ok(T)
    Err(E)

enum Option<T>:
    Some(T)
    None

# Pattern matching ensures exhaustiveness
func handle_result(result: Result<i32, string>):
    match result:
        case Ok(value):
            print("Success: " + str(value))
        case Err(error):
            print("Error: " + error)
        # ❌ Compiler error if any case is missing!
```

### **3. Trait System**
```gplang
# Define behavior contracts
trait Display:
    func display(self) -> string

trait Comparable:
    func compare(self, other: Self) -> i32

# Implement traits for types
impl Display for i32:
    func display(self) -> string:
        return str(self)

# Generic functions with trait bounds
func print_all<T>(items: &[T]) where T: Display:
    for item in items:
        print(item.display())
```

### **4. Lifetime Annotations**
```gplang
# Explicit lifetime management
func longest<'a>(x: &'a str, y: &'a str) -> &'a str:
    if x.len() > y.len():
        return x
    else:
        return y

# Compiler ensures returned reference is valid
var result = longest("hello", "world")  # Safe!
```

## 📊 **Compile-Time Error Examples**

### **Type Mismatch**
```gplang
var number: i32 = "hello"
```
```
Error: Type mismatch
  --> example.gp:1:19
   |
1  | var number: i32 = "hello"
   |                   ^^^^^^^ expected i32, found string
```

### **Use After Move**
```gplang
var data = create_data()
var moved = move(data)
process(data)
```
```
Error: Use of moved value
  --> example.gp:3:9
   |
2  | var moved = move(data)
   |                  ---- value moved here
3  | process(data)
   |         ^^^^ value used here after move
```

### **Borrow Checker**
```gplang
var data = create_data()
var borrowed = &data
var moved = move(data)
```
```
Error: Cannot move out of borrowed value
  --> example.gp:3:17
   |
2  | var borrowed = &data
   |                ----- borrow of data occurs here
3  | var moved = move(data)
   |                 ^^^^ move out of data occurs here
```

## 🚀 **Performance Benefits**

### **Zero Runtime Type Checking**
```gplang
# All type information erased at runtime
var value: i32 = 42
process(value)  # No runtime type tag needed!

# Compiles to optimal assembly:
# mov eax, 42
# call process
```

### **Optimal Code Generation**
```gplang
# Generic code generates specialized versions
func add<T>(a: T, b: T) -> T where T: Add:
    return a + b

var int_result = add(1, 2)        # Specialized for i32
var float_result = add(1.0, 2.0)  # Specialized for f64

# No runtime dispatch - direct function calls!
```

### **Compile-Time Optimizations**
```gplang
# Dead code elimination
if false:
    expensive_operation()  # Completely removed

# Constant folding
var result = 2 * 3 * 4  # Becomes: var result = 24

# Inlining
func square(x: i32) -> i32:
    return x * x

var area = square(5)  # Becomes: var area = 25
```

## 🎯 **Real-World Examples**

### **Web API with Type Safety**
```gplang
# Type-safe JSON handling
struct User:
    id: i32
    name: string
    email: string

func handle_user_request(json: JsonValue) -> Result<User, string>:
    var user = User {
        id: json.get("id")?.as_i32()?,
        name: json.get("name")?.as_string()?,
        email: json.get("email")?.as_string()?
    }
    return Ok(user)

# Compiler ensures all error cases are handled!
```

### **Game Engine with Performance**
```gplang
# Type-safe entity component system
struct Transform:
    position: Vec3
    rotation: Quat
    scale: Vec3

struct Velocity:
    value: Vec3

# Type-safe system iteration
func update_movement(transforms: &mut [Transform], velocities: &[Velocity], dt: f32):
    for (transform, velocity) in zip(transforms, velocities):
        transform.position += velocity.value * dt
```

## 🏆 **Comparison with Other Languages**

```
Static Typing Comparison:
┌─────────────────┬─────────────┬─────────────┬─────────────┬─────────────┐
│ Feature         │ C           │ C++         │ Rust        │ GPLANG      │
├─────────────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│ Type Safety     │ ⚠️ Weak     │ ⚠️ Weak     │ ✅ Strong   │ ✅ Strong   │
│ Null Safety     │ ❌ No       │ ❌ No       │ ✅ Yes      │ ✅ Yes      │
│ Memory Safety   │ ❌ No       │ ❌ No       │ ✅ Yes      │ ✅ Yes      │
│ Type Inference  │ ❌ No       │ ⚠️ Limited  │ ✅ Yes      │ ✅ Yes      │
│ Pattern Match   │ ❌ No       │ ❌ No       │ ✅ Yes      │ ✅ Yes      │
│ Zero Cost       │ ✅ Yes      │ ✅ Yes      │ ✅ Yes      │ ✅ Yes      │
└─────────────────┴─────────────┴─────────────┴─────────────┴─────────────┘

GPLANG provides the strongest type safety with zero overhead!
```

## 🚀 **Conclusion**

GPLANG's static typing system provides:

### **✅ Safety**
- Catch errors at compile time
- Prevent runtime crashes
- Memory safety guarantees
- Thread safety enforcement

### **✅ Performance**
- Zero runtime type overhead
- Optimal code generation
- Compile-time optimizations
- No runtime type checking

### **✅ Productivity**
- Type inference reduces boilerplate
- Rich error messages
- IDE support with autocomplete
- Refactoring safety

**GPLANG: Type safety without the performance tax!** 🚀

*Compile-time safety, runtime speed*
