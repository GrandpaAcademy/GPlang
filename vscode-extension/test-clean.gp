# GPLANG Clean Test - Should have NO false errors
# This comment mentions func but should be ignored
# Another comment about func declarations

import std.time

# Valid functions (should have NO errors)
func test():
    print("Valid function")
    return 0

func main():
    print("Testing clean error checking")
    
    # This comment mentions for loops
    parallel for i in range(10):
    print(i)
    
    return 0

# This function is missing colon (SHOULD show error)
func broken_function():
    print("Missing colon")
    return 1
