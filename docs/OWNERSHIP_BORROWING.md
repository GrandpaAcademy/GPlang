# GPLANG Ownership and Borrowing System

**Deterministic memory management without garbage collection - inspired by Rust**

## 🔒 **Ownership System**

GPLANG uses an ownership system that ensures memory safety at compile time with zero runtime overhead.

### **🎯 Core Principles**
1. **Each value has exactly one owner**
2. **When the owner goes out of scope, the value is freed**
3. **Ownership can be transferred (moved)**
4. **References can be borrowed temporarily**

## 📋 **Ownership Rules**

### **Rule 1: Single Ownership**
```gplang
func basic_ownership():
    var data = alloc(1024)  # data owns the memory
    process(data)           # data is still the owner
    # data automatically freed when function ends
```

### **Rule 2: Move Semantics**
```gplang
func move_example():
    var original = create_large_object()
    var moved = move(original)  # Ownership transferred
    
    # ❌ Compiler error: use of moved value
    # process(original)
    
    # ✅ Use the new owner
    process(moved)  # moved is now responsible for cleanup
```

### **Rule 3: Scope-Based Cleanup**
```gplang
func scope_example():
    {
        var local_data = alloc(512)
        process(local_data)
    }  # local_data automatically freed here
    
    # ❌ Compiler error: local_data no longer exists
    # process(local_data)
```

## 🔗 **Borrowing System**

Borrowing allows temporary access to data without taking ownership.

### **Immutable Borrowing**
```gplang
func read_data(data: &[u8]):  # Immutable borrow
    print("Data size: " + str(data.len()))
    # Cannot modify data, only read

func immutable_borrow_example():
    var buffer = alloc(1024)
    
    read_data(&buffer)    # Borrow buffer immutably
    read_data(&buffer)    # Multiple immutable borrows OK
    
    free(buffer)  # Owner still responsible for cleanup
```

### **Mutable Borrowing**
```gplang
func modify_data(data: &mut [u8]):  # Mutable borrow
    data[0] = 42  # Can modify the data

func mutable_borrow_example():
    var buffer = alloc(1024)
    
    modify_data(&mut buffer)  # Borrow buffer mutably
    
    # ❌ Compiler error: cannot have multiple mutable borrows
    # modify_data(&mut buffer)
    
    free(buffer)
```

### **Borrowing Rules**
```gplang
func borrowing_rules():
    var data = create_data()
    
    # ✅ Multiple immutable borrows allowed
    var ref1 = &data
    var ref2 = &data
    var ref3 = &data
    
    # ✅ One mutable borrow allowed (when no immutable borrows)
    var mut_ref = &mut data
    
    # ❌ Compiler error: cannot mix mutable and immutable borrows
    # var another_ref = &data  # Error!
```

## 🛡️ **Memory Safety Guarantees**

### **1. No Use-After-Free**
```gplang
func prevent_use_after_free():
    var data = alloc(100)
    free(data)
    
    # ❌ Compiler error: use after free
    # print(data[0])
```

### **2. No Double-Free**
```gplang
func prevent_double_free():
    var data = alloc(100)
    free(data)
    
    # ❌ Compiler error: double free
    # free(data)
```

### **3. No Memory Leaks**
```gplang
func prevent_memory_leaks():
    var data = alloc(100)
    
    if condition:
        return  # ❌ Compiler error: data not freed on this path
    
    free(data)  # ✅ All paths must free the data
```

### **4. No Dangling Pointers**
```gplang
func prevent_dangling_pointers() -> &i32:
    var local = 42
    
    # ❌ Compiler error: returning reference to local variable
    # return &local
    
    # ✅ Must return owned data or static reference
    var heap_data = alloc(sizeof(i32))
    *heap_data = 42
    return heap_data  # Ownership transferred to caller
```

## 🚀 **Advanced Ownership Patterns**

### **1. RAII (Resource Acquisition Is Initialization)**
```gplang
struct File:
    handle: *mut FileHandle

impl File:
    func open(path: string) -> File:
        var handle = fopen(path.c_str(), "r")
        return File { handle: handle }
    
    func read(self: &File) -> string:
        # Read from file
        return read_all(self.handle)
    
    func close(self: File):  # Takes ownership, consumes self
        fclose(self.handle)

func raii_example():
    var file = File.open("data.txt")  # Resource acquired
    var content = file.read()
    file.close()  # Resource explicitly released
    # file is now invalid - compiler prevents further use
```

### **2. Smart Pointers**
```gplang
# Box: Heap allocation with automatic cleanup
struct Box<T>:
    data: *mut T

impl<T> Box<T>:
    func new(value: T) -> Box<T>:
        var ptr = alloc(sizeof(T))
        *ptr = value
        return Box { data: ptr }
    
    func get(self: &Box<T>) -> &T:
        return self.data
    
    func drop(self: Box<T>):  # Automatic cleanup
        free(self.data)

func smart_pointer_example():
    var boxed = Box.new(42)  # Heap allocated
    print(str(*boxed.get()))
    # boxed automatically freed when out of scope
```

### **3. Reference Counting (Optional)**
```gplang
# Rc: Reference counted smart pointer for shared ownership
struct Rc<T>:
    data: *mut RcInner<T>

struct RcInner<T>:
    value: T
    ref_count: usize

impl<T> Rc<T>:
    func new(value: T) -> Rc<T>:
        var inner = alloc(sizeof(RcInner<T>))
        inner.value = value
        inner.ref_count = 1
        return Rc { data: inner }
    
    func clone(self: &Rc<T>) -> Rc<T>:
        self.data.ref_count += 1
        return Rc { data: self.data }
    
    func drop(self: Rc<T>):
        self.data.ref_count -= 1
        if self.data.ref_count == 0:
            free(self.data)

func shared_ownership_example():
    var shared = Rc.new("shared data")
    var clone1 = shared.clone()  # Ref count: 2
    var clone2 = shared.clone()  # Ref count: 3
    
    # All clones automatically cleaned up when out of scope
```

## 🔧 **Lifetime Management**

### **Explicit Lifetimes**
```gplang
# Lifetime annotations ensure reference validity
func longest<'a>(x: &'a str, y: &'a str) -> &'a str:
    if x.len() > y.len():
        return x
    else:
        return y

func lifetime_example():
    var string1 = "hello"
    var string2 = "world"
    var result = longest(&string1, &string2)
    print(result)  # Safe: both strings live long enough
```

### **Lifetime Elision**
```gplang
# Simple cases don't need explicit lifetimes
func first_word(s: &str) -> &str:  # Lifetime inferred
    var bytes = s.as_bytes()
    
    for i in range(bytes.len()):
        if bytes[i] == ' ':
            return s.substring(0, i)
    
    return s
```

## 📊 **Performance Benefits**

### **Zero Runtime Overhead**
```gplang
# All ownership checking happens at compile time
var data = Box.new(expensive_object())
process(data.get())
# No runtime reference counting or GC overhead!

# Compiles to optimal assembly:
# call malloc
# call expensive_object_constructor
# call process
# call free
```

### **Optimal Memory Layout**
```gplang
# Ownership enables optimal memory layout
struct Point:
    x: f32
    y: f32

struct Line:
    start: Point  # Embedded, not pointer
    end: Point    # Embedded, not pointer

# Memory layout: [start.x][start.y][end.x][end.y]
# Cache-friendly, no indirection!
```

### **Compile-Time Optimizations**
```gplang
# Move semantics enable copy elision
func create_large_object() -> LargeObject:
    var obj = LargeObject.new()
    obj.initialize()
    return obj  # Move, not copy!

var result = create_large_object()  # No copying!
```

## 🎯 **Real-World Examples**

### **Game Engine Entity System**
```gplang
struct Entity:
    id: u32
    components: Vec<Box<Component>>

impl Entity:
    func add_component<T>(self: &mut Entity, component: T) where T: Component:
        self.components.push(Box.new(component))
    
    func get_component<T>(self: &Entity) -> Option<&T> where T: Component:
        for component in &self.components:
            if let Some(comp) = component.downcast_ref::<T>():
                return Some(comp)
        return None

# Memory safe, zero overhead component system!
```

### **Network Protocol Parser**
```gplang
struct PacketParser:
    buffer: Vec<u8>
    position: usize

impl PacketParser:
    func parse_header(self: &mut PacketParser) -> Result<Header, ParseError>:
        if self.position + sizeof(Header) > self.buffer.len():
            return Err(ParseError.InsufficientData)
        
        var header = Header.from_bytes(&self.buffer[self.position..])
        self.position += sizeof(Header)
        return Ok(header)

# Safe parsing with zero-copy when possible
```

## 🏆 **Comparison with Other Approaches**

```
Memory Management Comparison:
┌─────────────────┬─────────────┬─────────────┬─────────────┬─────────────┐
│ Approach        │ Safety      │ Performance │ Complexity  │ Determinism │
├─────────────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│ Manual (C)      │ ❌ Unsafe   │ ✅ Fast     │ ✅ Simple   │ ✅ Yes      │
│ GC (Java/Go)    │ ✅ Safe     │ ⚠️ Pauses   │ ✅ Simple   │ ❌ No       │
│ RC (Python)     │ ⚠️ Cycles   │ ⚠️ Overhead │ ✅ Simple   │ ⚠️ Mostly   │
│ Ownership (Rust)│ ✅ Safe     │ ✅ Fast     │ ⚠️ Complex  │ ✅ Yes      │
│ GPLANG          │ ✅ Safe     │ ✅ Fast     │ ⚠️ Learning │ ✅ Yes      │
└─────────────────┴─────────────┴─────────────┴─────────────┴─────────────┘

GPLANG provides the best combination of safety and performance!
```

## 🚀 **Conclusion**

GPLANG's ownership and borrowing system provides:

### **✅ Memory Safety**
- No use-after-free bugs
- No double-free errors
- No memory leaks
- No dangling pointers

### **✅ Zero Runtime Overhead**
- All checks at compile time
- No garbage collection pauses
- No reference counting overhead
- Optimal memory layout

### **✅ Deterministic Performance**
- Predictable memory usage
- No GC pauses
- Real-time system compatible
- Perfect for embedded systems

**GPLANG: Memory safety without the performance cost!** 🚀

*Rust-like safety, C-like performance, Python-like syntax*
