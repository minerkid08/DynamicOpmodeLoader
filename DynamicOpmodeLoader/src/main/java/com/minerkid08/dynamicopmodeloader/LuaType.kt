package com.minerkid08.dynamicopmodeloader

abstract class LuaType(val clazz: Class<*>)
{
    companion object
    {
        @JvmField
        val Void = VoidT();

        @JvmField
        val Bool = BoolT();

        @JvmField
        val Double = DoubleT();

        @JvmField
        val Int = IntT();

        @JvmField
        val Float = FloatT();

        @JvmField
        val String = StringT();

        @JvmField
        val Builder = BuilderT();

        @JvmField
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
