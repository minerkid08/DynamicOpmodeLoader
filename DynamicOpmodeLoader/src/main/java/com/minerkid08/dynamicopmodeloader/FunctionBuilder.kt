package com.minerkid08.dynamicopmodeloader

private enum class LuaTypeId(val id: kotlin.Int)
{
    None(-1),
    Nil(0),
    Bool(1),
    LightUserData(2),
    Number(3),
    String(4),
    Table(5),
    Function(6),
    UserData(7),
    Thread(8),
    Float(9),
    Int(10),
    Builder(11)
}

class LuaCallback(private val id: Int)
{
    external fun call(vararg args: Any?);
}

abstract class LuaType(val clazz: Class<*>)
{
    companion object
    {
        val Void = VoidT();
        val Bool = BoolT();
        val Double = DoubleT();
        val Int = IntT();
        val Float = FloatT();
        val String = StringT();
        val Builder = BuilderT();
        val Callback = CallbackT();
    }

    class Object(clazz: Class<*>) : LuaType(clazz);
    class DoubleT : LuaType(kotlin.Double::class.java);
    class IntT : LuaType(kotlin.Int::class.java);
    class FloatT : LuaType(kotlin.Float::class.java);
    class BoolT : LuaType(Boolean::class.java);
    class StringT : LuaType(kotlin.String::class.java);
    class VoidT : LuaType(Any::class.java);
    class BuilderT : LuaType(Any::class.java);
    class CallbackT : LuaType(LuaCallback::class.java);
}

class FunctionBuilder
{
    /**
     * sets the object to pull object functions from
     */
    external fun <T> setCurrentObject(thing: T);

    /**
     * creates a class for a lua function to return
     * does not need to be called if a function is created for that class
     */
    external fun createClass(name: String);

    /**
     * adds a function from a java object to lua as a global
     */
    fun addObjectFunction(
        name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType>? = null
    )
    {
        val sig = generateSignature(rtnType, argTypes);
        val argc = argTypes?.size ?: 0;
        addFunction(name, sig, typeToInt(rtnType), argc);
    }

    /**
     * adds a function from a java class to a class table in lua
     */
    fun addClassFunction(
        clazz: Class<*>,
        name: String,
        rtnType: LuaType = LuaType.Void,
        argTypes: List<LuaType>? = null
    )
    {
        val sig = generateSignature(rtnType, argTypes);
        val argc = argTypes?.size ?: 0;
        addFunctionc(clazz, name, sig, typeToInt(rtnType), argc);
    }

    private external fun addFunction(name: String, funSignature: String, rtnType: Int, argc: Int);

    private external fun addFunctionc(
        clazz: Class<*>, name: String, funSignature: String, rtnType: Int, argc: Int
    );

    private fun generateSignature(rtnType: LuaType, argTypes: List<LuaType>?): String
    {
        return generateSignature(typeToStr(rtnType), argTypes);
    }

    private fun generateSignature(rtnType: String, argTypes: List<LuaType>?): String
    {
        var funSignature = "(";
        if (argTypes != null)
        {
            for (type in argTypes)
            {
                if (type is LuaType.VoidT) throw LuaError("Void is not a valid argument type");
                if (type is LuaType.BuilderT) throw LuaError("Builder is not a valid argument type");
                funSignature += typeToStr(type);
            }
        }
        funSignature += ')';
        funSignature += rtnType;

        funSignature = funSignature.replace('.', '/');

        return funSignature;
    }

    private fun typeToStr(type: LuaType): String
    {
        return when (type)
        {
            is LuaType.DoubleT   -> "D";
            is LuaType.FloatT    -> "F";
            is LuaType.IntT      -> "I";
            is LuaType.BoolT     -> "Z";
            is LuaType.VoidT     -> "V";
            is LuaType.BuilderT  -> "V";
            is LuaType.StringT   -> "Ljava/lang/String;";
            is LuaType.CallbackT -> "L${LuaCallback::class.java.name.replace('.', '/')};";
            else                 -> "L${type.clazz.name.replace('.', '/')};";
        }
    }

    private fun typeToInt(type: LuaType): Int
    {
        return when (type)
        {
            is LuaType.DoubleT   -> LuaTypeId.Number.id;
            is LuaType.FloatT    -> LuaTypeId.Float.id;
            is LuaType.IntT      -> LuaTypeId.Int.id;
            is LuaType.BoolT     -> LuaTypeId.Bool.id;
            is LuaType.VoidT     -> LuaTypeId.Nil.id;
            is LuaType.StringT   -> LuaTypeId.String.id;
            is LuaType.Object    -> LuaTypeId.Table.id;
            is LuaType.BuilderT  -> LuaTypeId.Builder.id;
            is LuaType.CallbackT -> LuaTypeId.Function.id;
            else                 -> LuaTypeId.Nil.id;
        }
    }
}