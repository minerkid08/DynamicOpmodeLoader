# FTC Dynamic Opmode Loader

A library for controling your robot with the lua programming language allowing for significantly faster upload times.

# Installation

1. Download `dynamicopmodeloader.aar` from the latest release
2. Place this file in the root of your project
3. Open the build.gradle file for the TeamCode module and add this line to the dependencies block
    ```gradle
    implementation files(getProjectDir().parent + "/dynamicopmodeloader.aar")
    ```
4. Download the lua quickstart from the latest release
5. Place it in the teamcode folder
6. In android studio install the SumnekoLua plugin and restart the IDE

# Usage

### Java

Create a new opmode class and create an `OpmodeLoader` object.
```java
OpmodeLoader opmodeLoader = new OpmodeLoader();
```
Call init on the object to get a list of all the available opmode names.
```java
String[] opmodes = opmodeLoader.init();
```
Load your opmode of choice.
```java
opmodeLoader.loadOpmode("testOpmode");
```
Wait for the opmode to be started.
Start the opmode with the recognition id.
```java
opmodeLoader.start(0);
```
While the opmode is running call update.
```java
opmodeLoader.update(deltaTime, elapsedTime);
```
Combining all of this into one example gives us:
```java
OpmodeLoader opmodeLoader = new OpmodeLoader();
String[] opmodes = opmodeLoader.init();
opmodeLoader.loadOpmode("testOpmode");

waitForStart();

opmodeLoader.start(0);

ElapsedTime elapsedTime = new ElapsedTime();
elapsedTime.reset();
double lastTickTime = 0.0;
while(opModeIsActive())
{
    double now = elapsedTime.seconds();
    opmodeLoader.update(now - lastTickTime, now);
    lastTickTime = now;
}
```

To give the lua code the ability to interact with the robot we need to give it some functions.
The `FunctionBuilder` class exposes functions to the lua code.
After you init the `OpmodeLoader` object you can get a `FunctionBuilder` by calling `OpmodeLoader::getFunctionBuilder()`

With this object you can create either object functions or global functions.

For global functions set the object the functions will come from with `FunctionBuilder::setCurrentObject(object)` and add the functions with `FunctionBuilder::addObjectFunction(String name, LuaType returnType, List<LuaType> argTypes)`

```java
class TestObject
{
    public String doThing(double a, double b)
    {
        return String.format("%d", a + b);
    }
}

// in opmode

TestObject testObject = new TestObject();
FunctionBuilder builder = opmodeLoader.getFunctionBuilder();
builder.setCurrentObject(testObject);
builder.addObjectFunction("doThing", LuaType.String, Arrays.asList(LuaType.Number, LuaType.Number));
```

For object functions you need to call `FunctionBuilder::addClassFunction(Class<*> clazz, String name, LuaType returnType, List<LuaType> argTypes)`
Note you will have to add a global function to get an instance of the object.

```java
class TestObject
{
    public String doThing(double a, double b)
    {
        return String.format("%d", a + b);
    }
}

class TestObjectGetter
{
    public TestObject getTestObject()
    {
        return new TestObject();
    }
}

// in opmode

FunctionBuilder builder = opmodeLoader.getFunctionBuilder();
builder.addClassFunction(TestObject, "doThing", LuaType.String, Arrays.asList(LuaType.Number, LuaType.Number));

TestObjectBuilder testObject = new TestObjectBuilder();
builder.addObjectFunction(testObjectBuilder, "getTestObject", LuaType.Object(TestObject));
```
### Lua

Add your opmodes with `addOpmode()`
Note that the init, start and update fields are optional

```lua
addOpmode({
    name = "testOpmode",
    init = function()
    end,
    start = function(recognitionId)
    end,
    update = function(deltaTime, elapsedTime)
    end
})
```

upload these 