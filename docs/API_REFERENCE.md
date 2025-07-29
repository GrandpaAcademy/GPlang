# GPLANG API Reference

**Complete API documentation for GPLANG standard library**

## 📚 **Standard Library Modules**

### **std.core** - Core Functions

#### **Print Functions**
```gplang
func print(message: string)
func print_no_newline(message: string)
```

#### **Type Conversion**
```gplang
func str(value: any) -> string
func int(value: string) -> i32
func float(value: string) -> f64
func bool(value: string) -> bool
```

#### **Memory Management**
```gplang
func alloc(size: usize) -> *mut u8
func alloc_aligned(size: usize, alignment: usize) -> *mut u8
func free(ptr: *mut u8)
func realloc(ptr: *mut u8, new_size: usize) -> *mut u8
```

### **std.math** - Mathematical Functions

#### **Basic Math**
```gplang
func abs(x: f64) -> f64
func sqrt(x: f64) -> f64
func pow(base: f64, exp: f64) -> f64
func min(a: f64, b: f64) -> f64
func max(a: f64, b: f64) -> f64
```

#### **Trigonometric Functions**
```gplang
func sin(x: f64) -> f64
func cos(x: f64) -> f64
func tan(x: f64) -> f64
func asin(x: f64) -> f64
func acos(x: f64) -> f64
func atan(x: f64) -> f64
func atan2(y: f64, x: f64) -> f64
```

#### **Logarithmic Functions**
```gplang
func log(x: f64) -> f64          # Natural logarithm
func log10(x: f64) -> f64        # Base-10 logarithm
func log2(x: f64) -> f64         # Base-2 logarithm
func exp(x: f64) -> f64          # e^x
func exp2(x: f64) -> f64         # 2^x
```

#### **Rounding Functions**
```gplang
func floor(x: f64) -> f64
func ceil(x: f64) -> f64
func round(x: f64) -> f64
func trunc(x: f64) -> f64
```

#### **Constants**
```gplang
const PI: f64 = 3.14159265358979323846
const E: f64 = 2.71828182845904523536
const TAU: f64 = 6.28318530717958647692
```

### **std.simd** - SIMD Vector Operations

#### **Vector Types**
```gplang
struct Vec2:
    x: f32
    y: f32

struct Vec3:
    x: f32
    y: f32
    z: f32

struct Vec4:
    x: f32
    y: f32
    z: f32
    w: f32
```

#### **Vector Operations**
```gplang
# Vec2 operations
func Vec2.create(x: f32, y: f32) -> Vec2
func Vec2.add(a: Vec2, b: Vec2) -> Vec2
func Vec2.sub(a: Vec2, b: Vec2) -> Vec2
func Vec2.mul(v: Vec2, scalar: f32) -> Vec2
func Vec2.dot(a: Vec2, b: Vec2) -> f32
func Vec2.length(v: Vec2) -> f32
func Vec2.normalize(v: Vec2) -> Vec2

# Vec3 operations
func Vec3.create(x: f32, y: f32, z: f32) -> Vec3
func Vec3.add(a: Vec3, b: Vec3) -> Vec3
func Vec3.sub(a: Vec3, b: Vec3) -> Vec3
func Vec3.mul(v: Vec3, scalar: f32) -> Vec3
func Vec3.dot(a: Vec3, b: Vec3) -> f32
func Vec3.cross(a: Vec3, b: Vec3) -> Vec3
func Vec3.length(v: Vec3) -> f32
func Vec3.normalize(v: Vec3) -> Vec3

# Vec4 operations (SIMD optimized)
func Vec4.create(x: f32, y: f32, z: f32, w: f32) -> Vec4
func Vec4.add(a: Vec4, b: Vec4) -> Vec4
func Vec4.sub(a: Vec4, b: Vec4) -> Vec4
func Vec4.mul(v: Vec4, scalar: f32) -> Vec4
func Vec4.dot(a: Vec4, b: Vec4) -> f32
func Vec4.length(v: Vec4) -> f32
func Vec4.normalize(v: Vec4) -> Vec4
```

#### **SIMD Array Operations**
```gplang
func simd_add_arrays(a: &[f32], b: &[f32], result: &mut [f32])
func simd_mul_arrays(a: &[f32], b: &[f32], result: &mut [f32])
func simd_scale_array(input: &[f32], scalar: f32, result: &mut [f32])
func simd_dot_product(a: &[f32], b: &[f32]) -> f32
```

### **std.string** - String Operations

#### **String Type**
```gplang
struct String:
    data: *mut u8
    length: usize
    capacity: usize
```

#### **String Functions**
```gplang
func String.create(s: &str) -> String
func String.from_chars(chars: &[char]) -> String
func String.length(s: &String) -> usize
func String.is_empty(s: &String) -> bool
func String.clear(s: &mut String)
func String.push(s: &mut String, ch: char)
func String.push_str(s: &mut String, other: &str)
func String.pop(s: &mut String) -> Option<char>
```

#### **String Manipulation**
```gplang
func String.concat(a: &String, b: &String) -> String
func String.substring(s: &String, start: usize, end: usize) -> String
func String.split(s: &String, delimiter: &str) -> [String]
func String.join(strings: &[String], separator: &str) -> String
func String.replace(s: &String, from: &str, to: &str) -> String
func String.trim(s: &String) -> String
func String.trim_start(s: &String) -> String
func String.trim_end(s: &String) -> String
```

#### **String Queries**
```gplang
func String.contains(s: &String, pattern: &str) -> bool
func String.starts_with(s: &String, prefix: &str) -> bool
func String.ends_with(s: &String, suffix: &str) -> bool
func String.find(s: &String, pattern: &str) -> Option<usize>
func String.rfind(s: &String, pattern: &str) -> Option<usize>
```

#### **Case Conversion**
```gplang
func String.to_upper(s: &String) -> String
func String.to_lower(s: &String) -> String
func String.to_title_case(s: &String) -> String
```

### **std.time** - Time and Date

#### **Time Type**
```gplang
struct Time:
    seconds: i64
    nanoseconds: i64
```

#### **Time Functions**
```gplang
func Time.now() -> Time
func Time.unix_timestamp(t: &Time) -> i64
func Time.to_seconds(t: &Time) -> f64
func Time.to_milliseconds(t: &Time) -> i64
func Time.to_microseconds(t: &Time) -> i64
func Time.to_nanoseconds(t: &Time) -> i64
```

#### **Time Arithmetic**
```gplang
func Time.add(a: Time, b: Time) -> Time
func Time.sub(a: Time, b: Time) -> Time
func Time.diff(end: Time, start: Time) -> Time
```

#### **Formatting**
```gplang
func Time.format(t: &Time, format: &str) -> String
func Time.parse(s: &str, format: &str) -> Result<Time, String>
```

### **std.collections** - Data Structures

#### **Array Type**
```gplang
struct Array<T>:
    data: *mut T
    length: usize
    capacity: usize
```

#### **Array Operations**
```gplang
func Array.create<T>() -> Array<T>
func Array.with_capacity<T>(capacity: usize) -> Array<T>
func Array.push<T>(arr: &mut Array<T>, item: T)
func Array.pop<T>(arr: &mut Array<T>) -> Option<T>
func Array.get<T>(arr: &Array<T>, index: usize) -> Option<&T>
func Array.set<T>(arr: &mut Array<T>, index: usize, value: T)
func Array.length<T>(arr: &Array<T>) -> usize
func Array.is_empty<T>(arr: &Array<T>) -> bool
func Array.clear<T>(arr: &mut Array<T>)
```

#### **HashMap Type**
```gplang
struct HashMap<K, V>:
    buckets: Array<Bucket<K, V>>
    size: usize
    capacity: usize
```

#### **HashMap Operations**
```gplang
func HashMap.create<K, V>() -> HashMap<K, V>
func HashMap.insert<K, V>(map: &mut HashMap<K, V>, key: K, value: V)
func HashMap.get<K, V>(map: &HashMap<K, V>, key: &K) -> Option<&V>
func HashMap.remove<K, V>(map: &mut HashMap<K, V>, key: &K) -> Option<V>
func HashMap.contains_key<K, V>(map: &HashMap<K, V>, key: &K) -> bool
func HashMap.keys<K, V>(map: &HashMap<K, V>) -> Array<K>
func HashMap.values<K, V>(map: &HashMap<K, V>) -> Array<V>
func HashMap.size<K, V>(map: &HashMap<K, V>) -> usize
```

### **std.fs** - File System

#### **File Operations**
```gplang
func read_file(path: &str) -> Result<String, String>
func write_file(path: &str, content: &str) -> Result<(), String>
func append_file(path: &str, content: &str) -> Result<(), String>
func copy_file(from: &str, to: &str) -> Result<(), String>
func move_file(from: &str, to: &str) -> Result<(), String>
func delete_file(path: &str) -> Result<(), String>
func file_exists(path: &str) -> bool
func file_size(path: &str) -> Result<usize, String>
```

#### **Directory Operations**
```gplang
func create_dir(path: &str) -> Result<(), String>
func create_dir_all(path: &str) -> Result<(), String>
func remove_dir(path: &str) -> Result<(), String>
func remove_dir_all(path: &str) -> Result<(), String>
func list_dir(path: &str) -> Result<Array<String>, String>
func dir_exists(path: &str) -> bool
```

#### **Path Operations**
```gplang
func join_path(parts: &[String]) -> String
func parent_dir(path: &str) -> Option<String>
func file_name(path: &str) -> Option<String>
func file_extension(path: &str) -> Option<String>
func absolute_path(path: &str) -> Result<String, String>
```

### **std.net** - Networking

#### **HTTP Client**
```gplang
struct HttpClient:
    timeout: Time
    headers: HashMap<String, String>

func HttpClient.create() -> HttpClient
func HttpClient.get(client: &HttpClient, url: &str) -> Result<HttpResponse, String>
func HttpClient.post(client: &HttpClient, url: &str, body: &str) -> Result<HttpResponse, String>
func HttpClient.put(client: &HttpClient, url: &str, body: &str) -> Result<HttpResponse, String>
func HttpClient.delete(client: &HttpClient, url: &str) -> Result<HttpResponse, String>
```

#### **HTTP Server**
```gplang
struct HttpServer:
    address: String
    port: u16

func HttpServer.create(address: &str, port: u16) -> HttpServer
func HttpServer.start(server: &HttpServer, handler: fn(&HttpRequest) -> HttpResponse)
func HttpServer.stop(server: &HttpServer)
```

### **std.json** - JSON Processing

#### **JSON Functions**
```gplang
func parse_json(json_str: &str) -> Result<JsonValue, String>
func stringify_json(value: &JsonValue) -> String
func pretty_print_json(value: &JsonValue) -> String
```

#### **JSON Types**
```gplang
enum JsonValue:
    Null
    Bool(bool)
    Number(f64)
    String(String)
    Array(Array<JsonValue>)
    Object(HashMap<String, JsonValue>)
```

### **std.crypto** - Cryptography

#### **Hashing**
```gplang
func sha256(data: &[u8]) -> [u8; 32]
func sha512(data: &[u8]) -> [u8; 64]
func md5(data: &[u8]) -> [u8; 16]
func blake3(data: &[u8]) -> [u8; 32]
```

#### **Encryption**
```gplang
func aes_encrypt(data: &[u8], key: &[u8; 32]) -> Result<Array<u8>, String>
func aes_decrypt(encrypted: &[u8], key: &[u8; 32]) -> Result<Array<u8>, String>
```

#### **Random Number Generation**
```gplang
func random_bytes(count: usize) -> Array<u8>
func random_int(min: i32, max: i32) -> i32
func random_float() -> f64
func random_string(length: usize) -> String
```

## 🔧 **Error Handling**

### **Result Type**
```gplang
enum Result<T, E>:
    Ok(T)
    Err(E)
```

### **Option Type**
```gplang
enum Option<T>:
    Some(T)
    None
```

### **Pattern Matching**
```gplang
match result:
    case Ok(value):
        print("Success: " + str(value))
    case Err(error):
        print("Error: " + error)

match option:
    case Some(value):
        print("Value: " + str(value))
    case None:
        print("No value")
```

---

**📚 This API reference covers the complete GPLANG standard library**

*For more examples and tutorials, see the [Complete Guide](COMPLETE_GUIDE.md)*
