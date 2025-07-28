# GPLANG JSON Module Demo
# Demonstrates JSON parsing and manipulation

import json

func main():
    print("📄 GPLANG JSON Module Demo")
    print("==========================")
    print("")
    
    # Create JSON Object
    print("🏗️  Creating JSON Objects:")
    var person = json.object()
    
    json.object_set_string(person, "name", "Alice Johnson")
    json.object_set_number(person, "age", 28)
    json.object_set_bool(person, "is_developer", true)
    json.object_set_string(person, "city", "San Francisco")
    json.object_set_number(person, "salary", 95000.50)
    
    var person_json = json.stringify(person)
    print("Person JSON: " + person_json)
    print("")
    
    # Create JSON Array
    print("📋 Creating JSON Arrays:")
    var skills = json.array()
    
    json.array_append_string(skills, "Python")
    json.array_append_string(skills, "JavaScript")
    json.array_append_string(skills, "GPLANG")
    json.array_append_string(skills, "Go")
    json.array_append_string(skills, "Rust")
    
    var skills_json = json.stringify(skills)
    print("Skills JSON: " + skills_json)
    
    # Add skills array to person object
    json.object_set_array(person, "skills", skills)
    
    var complete_person_json = json.stringify(person)
    print("Complete Person JSON: " + complete_person_json)
    print("")
    
    # Reading JSON Values
    print("📖 Reading JSON Values:")
    var name = json.object_get_string(person, "name")
    var age = json.object_get_number(person, "age")
    var is_dev = json.object_get_bool(person, "is_developer")
    var city = json.object_get_string(person, "city")
    var salary = json.object_get_number(person, "salary")
    
    print("Name: " + name)
    print("Age: " + str(age))
    print("Is Developer: " + str(is_dev))
    print("City: " + city)
    print("Salary: $" + str(salary))
    print("")
    
    # Working with Arrays
    print("🔢 Working with Arrays:")
    var skills_array = json.object_get_array(person, "skills")
    if skills_array:
        var skills_count = json.array_size(skills_array)
        print("Number of skills: " + str(skills_count))
        
        print("Skills list:")
        for i in range(0, skills_count):
            var skill = json.array_get(skills_array, i)
            if skill:
                var skill_name = json.get_string(skill)
                print("  " + str(i + 1) + ". " + skill_name)
    
    print("")
    
    # Complex JSON Structure
    print("🏢 Complex JSON Structure:")
    var company = json.object()
    
    json.object_set_string(company, "name", "TechCorp Inc.")
    json.object_set_string(company, "industry", "Software Development")
    json.object_set_number(company, "founded", 2015)
    json.object_set_number(company, "employees", 250)
    
    # Create departments array
    var departments = json.array()
    
    # Engineering department
    var engineering = json.object()
    json.object_set_string(engineering, "name", "Engineering")
    json.object_set_number(engineering, "employees", 120)
    json.object_set_string(engineering, "manager", "Bob Smith")
    json.array_append_object(departments, engineering)
    
    # Marketing department
    var marketing = json.object()
    json.object_set_string(marketing, "name", "Marketing")
    json.object_set_number(marketing, "employees", 45)
    json.object_set_string(marketing, "manager", "Carol Davis")
    json.array_append_object(departments, marketing)
    
    # Sales department
    var sales = json.object()
    json.object_set_string(sales, "name", "Sales")
    json.object_set_number(sales, "employees", 85)
    json.object_set_string(sales, "manager", "David Wilson")
    json.array_append_object(departments, sales)
    
    json.object_set_array(company, "departments", departments)
    
    var company_json = json.stringify(company)
    print("Company JSON: " + company_json)
    print("")
    
    # JSON Parsing
    print("🔍 JSON Parsing:")
    var json_string = '{"product": "GPLANG Compiler", "version": "1.0.0", "features": ["fast", "safe", "simple"], "price": 0}'
    
    var parsed = json.parse(json_string)
    if parsed:
        print("Parsed JSON successfully!")
        
        var product = json.object_get_string(parsed, "product")
        var version = json.object_get_string(parsed, "version")
        var price = json.object_get_number(parsed, "price")
        
        print("Product: " + product)
        print("Version: " + version)
        print("Price: $" + str(price))
        
        var features_array = json.object_get_array(parsed, "features")
        if features_array:
            var feature_count = json.array_size(features_array)
            print("Features (" + str(feature_count) + "):")
            
            for i in range(0, feature_count):
                var feature = json.array_get(features_array, i)
                if feature:
                    var feature_name = json.get_string(feature)
                    print("  - " + feature_name)
        
        json.free(parsed)
    else:
        print("❌ Failed to parse JSON")
    
    print("")
    
    # JSON Validation
    print("✅ JSON Validation:")
    var valid_json = '{"name": "test", "value": 123}'
    var invalid_json = '{"name": "test", "value": 123'
    
    var is_valid1 = json.validate(valid_json)
    var is_valid2 = json.validate(invalid_json)
    
    print("Valid JSON '" + valid_json + "': " + str(is_valid1))
    print("Invalid JSON '" + invalid_json + "': " + str(is_valid2))
    print("")
    
    # Configuration Example
    print("⚙️  Configuration Example:")
    var config = json.object()
    
    json.object_set_string(config, "app_name", "GPLANG App")
    json.object_set_string(config, "version", "1.0.0")
    json.object_set_bool(config, "debug_mode", true)
    json.object_set_number(config, "max_connections", 100)
    
    var database = json.object()
    json.object_set_string(database, "host", "localhost")
    json.object_set_number(database, "port", 5432)
    json.object_set_string(database, "name", "gplang_db")
    json.object_set_bool(database, "ssl", true)
    
    json.object_set_object(config, "database", database)
    
    var config_json = json.stringify(config)
    print("Configuration JSON: " + config_json)
    
    # Save configuration to file
    var config_file = "gplang_config.json"
    var write_result = files.write(config_file, config_json)
    if write_result == 0:
        print("✅ Configuration saved to: " + config_file)
        
        # Read and parse configuration
        var config_content = files.read(config_file)
        if config_content:
            var loaded_config = json.parse(config_content)
            if loaded_config:
                var app_name = json.object_get_string(loaded_config, "app_name")
                var debug_mode = json.object_get_bool(loaded_config, "debug_mode")
                
                print("Loaded app name: " + app_name)
                print("Debug mode: " + str(debug_mode))
                
                json.free(loaded_config)
            
        # Cleanup
        files.delete(config_file)
        print("✅ Cleaned up configuration file")
    
    print("")
    
    # Cleanup
    json.free(person)
    json.free(company)
    json.free(config)
    
    print("✅ JSON Module Demo Complete!")
    
    return 0
