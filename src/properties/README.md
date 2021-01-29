# bsc::properties

## Purpose

bsc::properties is a C++20 application properties parser. Properties can be loaded from file or string.

## Usage

After loading properties context, properties could be acquired using Property<"property.id"> type. You can use it directly by assigning it
to specific variable or indirectly by declaring variable of type Property<"property.id",Type> and passing required type as second argument.
You can get the value by using () operator. Default value could be passed as an argument to the constructor.

Property id maps into properties hierarchy, splitting on dots.

```cpp
int a1 = Property<"a">();
Property<"a",int> a2;
a1 == a2();
```

Let's assume you have following yaml text:

```yaml
sample-property: 5
nested:
  property: 7
  text: text with spaces
stringSequence:
  - aaa
  - bbb
  - ccc
structSequence:
  - fieldA: a1
    fieldB: b1
  - fieldA: a2
    fieldB: b2
  - fieldA: a3
    fieldB: b3
```

You can use properties like this:

```cpp
#include <properties/PropertyTextLoader.h>

int main() {

    //load properties to thread local context:
    //you can use PropertyTextLoader to pass text with properties directly or PropertyFileLoader to load properties from file.
    PropertyFileLoader loader("application-properties.yaml");
    
    //examples of direct initialization of variables
    
    int sampleProperty = Property<"sample-property">();
    assert(sampleProperty == 5);
    
    int defaultProperty = Property<"invalid.key">(8);
    assert(defaultProperty == 8);
    
    int nestedProperty = Property<"nested.property">();
    assert(nestedProperty == 7);
    
    //examples of variables of Property type
    Property<"nested.property",std::string> nestedString;
    assert(nestedString() == "7");
    
    Property<"nested.property",float> nestedFloat;
    assert(nestedFloat() == 7.0f);
    
    //examples of struct mapping
    struct Nested {
        int property = Property<"property">();
        std::string text = Property<"text">();
    };
    
    Property<"nested",Nested> nested{};
    assert(nested().property == 7);
    assert(nested().text == "text with spaces");

    Nested nested2 = Property<"nested">();
    assert(nested2.property == 7);
    assert(nested2.text == "text with spaces");
    
    //examples of sequence mapping
    std::vector<std::string> stringList = Property<"stringSequence">();
    assert(stringList.size() == 3);
    assert(stringList[0] == "aaa");
    assert(stringList[1] == "bbb");
    assert(stringList[2] == "ccc");
    
    struct Struct {
        std::string fieldA = Property<"fieldA">();
        Property<"fieldB",std::string> fieldB;
    };
    
    //example of sequence and struct mapping
    std::list<Struct> structSequence = Property<"structSequence">();
    assert(structSequence.size() == 3);
    for (int count = 1; const auto& item : structSequence) {
        assert(item.fieldA == "a" + std::to_string(count));
        assert(item.fieldB() == "b" + std::to_string(count));
        count++;
    }
}


```

You can declare properties as fields of a class, even nest them or use container (mapped to sequences in yaml); 
