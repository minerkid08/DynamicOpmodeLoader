# FTC Dynamic Opmode Loader

A library for controlling your robot with the lua programming language allowing for significantly faster upload times.

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

Create an `OpmodeLoader` object.
```java
OpmodeLoader opmodeLoader = new OpmodeLoader();
```
Call init on the object to get a list of all the available opmode names.
```java
String[] opmodes = opmodeLoader.init();
```
Load your opmode of choice.
```java
opmodeloader.loadOpmode("testOpmode");
```
Wait for the opmode to be started.
Start the opmode.
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

### Lua

The java example wont do much on its own than throw an error.

The library runs `lua/init.lua` on init, in this file you can define your opmodes and add them with `addOpmode()`.

```lua
local testOpmode = {
    name = "testOpmode",
    init = function()
    end,
    start = function(recognitionId)
    end,
    update = function(deltaTime, elapsedTime)
    end
);

addOpmode(testOpmode);
```
Note that the init, start and update fields are optional.

## Adding more functions

To give the lua code the ability to interact with the robot we need to give it some functions.
The `FunctionBuilder` class exposes functions to the lua code.
After you init the `OpmodeLoader` object you can get a `FunctionBuilder` by calling `OpmodeLoader::getFunctionBuilder()`

With this object you can create either class functions or object functions.

Class functions are defined from a class and any object with that class can call the function.
Object functions are global functions that are pulled from an object that is used to call the function;

Defining class functions can be done with `FunctionBuilder.addClassFunction(Class<*> class, String name, LuaType returnType = LuaType.Void, List<LuaType> argTypes = null)`

Note: you will need to define an object function to get an instance of the object.
```java
class ExampleClassFunctionObject 
{
    public String doThing(int a, int b)
    {
        return String.format("%d", a + b);
    }
}

// in opmode

TestObject testObject = new TestObject();
FunctionBuilder builder = opmodeLoader.getFunctionBuilder();
builder.addClassFunction(ExampleClassFunctionObject.class, "doThing", LuaType.String, List.of(LuaType.Int, LuaType.Int));
```

Defining object functions is similar but requires you to set the object first with `<T> FunctionBuilder.SetCurrentObject(T object)` then add the functin with `FunctionBuilder.addObjectFunction(String name, LuaType returnType = LuaType.Void, List<LuaType> argTypes = null)`
```java
class ExampleObjectFunctionObject
{
   public ExampleClassFunctionObject getObject()
   {
      return new ExampleClassFunctionObject();
   }
}

// in opmode

ExampleObjectFunctionObject object = new ExampleObjectFunctionObject();
FunctionBuilder builder = opmodeLoader.getFunctionBuilder();
builder.setCurrentObject(object);
builder.addObjectFunction("getObject", LuaType.Object(ExampleClassFunctionObject.class));
```

On the lua side, calling these functions can be done like any other function
```lua
-- from ExampleObjectFunctionObject
local object = getObject();

-- from ExampleClassFunctionObject
object:doThing(3, 4);
-- note the colon that is used in place of the dot in java
-- this is so the object is passed as the first argument into the function as lua does not have classes like other languages
-- this is equivalent to
object.doThing(object, 3, 4);
```

### Callbacks

Sometimes you want you java code to call a function that is passed into it.
Callbacks can be defined with the `LuaType.Callback` argument type and the `LuaCallback` type as the function argument.
They can be called later with `LuaCallback.call(...)`
```java
class ExampleCallbackObject
{
    public void doThing(LuaCallback callback)
    {
        //do some stuff here

        callback.call();
    }
}

// in opmode

FunctionBuilder builder = opmodeLoader.getFunctionBuilder();
builder.addClassFunction(ExampleCallbackObject.class, "doThing", LuaType.Void, List.of(LuaType.Callback));
```

In lua you pass a function as one of the arguments.
```lua
function callback()
    print("heh");
end

doThing(callback);
```

# Uploading

There are two methods for uploading code remotely
1. A shell script that sends all of the files using adb. (~5 seconds)
2. A program that sends all of the files to the robot using a custom server (0 seconds) (windows and linux only)

### Adb

The lua quick start contains a `sync.sh` file that has to be run in the folder it lives in.
Either open the terminal and navigate to that location and run it with bash or sh, or run it with a launch configuration.
To create the launch configuration open the configuration editing window and add a new shell script configuration, set the script path to the path of the script file and the working directory to the directory the script file is in.

### Upload Program

The lua quick start contains a `pack` file for linux and a `pack.exe` for windows that has to be run in the folder it lives in.
Using the terminal and navigate to the path the file is in and run the file for your operating system.

For Mac users the source for that program is in the `uploadUtil` directory in the project root, note that it is untested on that platform.

# Api Docs
https://minerkid08.github.io/DynamicOpmodeLoader/
